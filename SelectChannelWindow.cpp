/*
 *
 *  This file is part of Elan_v2.
 *
 *  Elan_v2 is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Elan_v2 is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Elan_v2.  If not, see <https://www.gnu.org/licenses/>.
 *
 */


/***************
 * A simple dialog box to select which chanels to display.
 *
 * Author: Thibaut Bertrand
 * thibaut.bertrand@inserm.fr
 ***************/


#include "SelectChannelWindow.h"
#include "ui_SelectChannelWindow.h"


/***************
 * Constructor
 *-------------------------
 * PlotData* plot: the data container of interest for this class.
 ***************/
SelectChannelWindow::SelectChannelWindow(PlotData* plot, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectChannelWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("Channel management");

    plotData = plot;


    // Create file label references (top of the window)
    //--------------------
    for (int i_file = 0; i_file < plotData->eegFileNb; i_file++)
    {
        QLabel* fileLabel = new QLabel;

        QString filename = plotData->eegDataStorage[i_file]->eegFilename;
        fileLabel->setText("File " + QString::number(i_file) + ": " + filename);
        fileLabel->setFont(font);
        fileLabel->setMaximumHeight(20);

        filenameList.append(filename);

        this->ui->filenameLayout->addWidget(fileLabel);
    }

    this->ui->filenameLayout->addStretch();


    // Create file layout
    //--------------------
    QHBoxLayout* fileLayout = new QHBoxLayout;

    // empty correspond to the place taken by the channel label. It's used to
    // have a correct alignement of the file label and the channel checkboxes,
    // and also to let space for the two button (select/unselect all file for a
    // specific channel).
    for (int i_empty = 0; i_empty < 2; i_empty++)
    {
        QLabel* emptyLabel = new QLabel;
        setMinMaxSize(emptyLabel, smallLabelSize, smallLabelSize);
        fileLayout->addWidget(emptyLabel);
    }

    for (int i_file = 0; i_file < plotData->eegFileNb; i_file++)
    {
        QLabel* fileLabel = new QLabel;

        fileLabel->setText("File " + QString::number(i_file));
        fileLabel->setFont(font);
        fileLabel->setAlignment(Qt::AlignCenter);
        setMinMaxSize(fileLabel, smallLabelSize, smallLabelSize);

        fileLayout->addWidget(fileLabel);
    }

    fileLayout->addStretch();

    this->ui->channelsLayout->addLayout(fileLayout);


    // Create channel layouts
    //--------------------
    for (int i_chan = 0; i_chan < plotData->uniqueChannelLabelList.length(); i_chan++)
    {
        QHBoxLayout* channelLayout = new QHBoxLayout;

        Channel* channel = new Channel;


        // Channel label
        QLabel* channelLabel = new QLabel;

        channelLabel->setText(plotData->uniqueChannelLabelList[i_chan]);
        channelLabel->setFont(font);
        setMinMaxSize(channelLabel, smallLabelSize, smallLabelSize);

        channel->label = channelLabel;

        channelLayout->addWidget(channelLabel);


        // Select/unselect all file buttons
        QVBoxLayout* buttonLayout = new QVBoxLayout;

        ButtonID* selectButton = new ButtonID(i_chan);
        ButtonID* unselectButton = new ButtonID(i_chan);

        selectButton->setText("Select all");
        selectButton->setFont(QFont("Arial", 7));
        unselectButton->setText("Unsel. all");
        unselectButton->setFont(QFont("Arial", 7));

        setMinMaxSize(selectButton, QSize(55, 14), QSize(55, 14));
        setMinMaxSize(unselectButton, QSize(55, 14), QSize(55, 14));

        connect(selectButton, SIGNAL(sendID(int, int)),
                this, SLOT(selectAllFileForChan(int)));

        connect(unselectButton, SIGNAL(sendID(int, int)),
                this, SLOT(unselectAllFileForChan(int)));

        channel->selectAllFileButton = selectButton;
        channel->unselectAllFileButton = unselectButton;

        buttonLayout->addWidget(selectButton);
        buttonLayout->addWidget(unselectButton);

        buttonLayout->setSpacing(0);
        buttonLayout->addStretch();

        channelLayout->addLayout(buttonLayout);


        // Channel checkboxes
        for (int j_file = 0; j_file < plotData->eegFileNb; j_file++)
        {
            CheckboxID* channelCheckbox = new CheckboxID(j_file, i_chan);


            // Size here must be the same as the file label for them to be aligned
            setMinMaxSize(channelCheckbox, smallLabelSize, smallLabelSize);


            // Check if the file have this specific channel
            channelCheckbox->setEnabled(false);

            for (int k_present = 0; k_present < plotData->eegDataStorage[j_file]->channels.length(); k_present++)
            {
                if (plotData->eegDataStorage[j_file]->channels[k_present]->label == channelLabel->text())
                {
                    channelCheckbox->setEnabled(true);
                    channelCheckbox->setChecked(true);

                    if (!plotData->eegDataStorage[j_file]->channels[k_present]->isDisplayed)
                    {
                        channelCheckbox->setChecked(false);
                    }

                    break;
                }
            }


            // Connect
            if (channelCheckbox->isEnabled())
            {
                connect(channelCheckbox, SIGNAL(sendInfoShort(int, int)),
                        this, SLOT(trackLastInteraction(int, int)));

                connect(this, SIGNAL(controlPress()),
                        channelCheckbox, SLOT(controlPress()));

                connect(this, SIGNAL(controlRelease()),
                        channelCheckbox, SLOT(controlRelease()));
            }

            channel->fileCheckboxes.append(channelCheckbox);

            channelLayout->addWidget(channelCheckbox);
        }

        channelList.append(channel);

        channelLayout->addStretch();

        this->ui->channelsLayout->addLayout(channelLayout);
    }

    this->ui->channelsLayout->setSpacing(15);
    this->ui->channelsLayout->addStretch();
}


SelectChannelWindow::~SelectChannelWindow()
{
    delete ui;
}



//      %%%%%%%%%%%%%%%%%%%%%%%%%%%%
//      %%%% METHODS (override) %%%%
//      %%%%%%%%%%%%%%%%%%%%%%%%%%%%


/***************
 ***************/
void SelectChannelWindow::closeEvent(QCloseEvent *e)
{
    if (!isChangeApplied)
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("You didn't saved your changes");
        msgBox.setText("Closing the window before applying the changes will"
                       "discard them.");
        msgBox.setInformativeText("Are you sure you want to quit?");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        int ret = msgBox.exec();

        if (ret == QMessageBox::Yes)
        {
            // Discard Changes
            e->accept();
        }
        else if (ret == QMessageBox::No)
        {
            // Return to Event Window
            msgBox.close();
            e->ignore();
        }
    }
}


/***************
 * cf Qt documentation.
 ***************/
void SelectChannelWindow::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Shift)
    {
        isShiftPressed = true; // For multiple box selection with shift
    }

    if (e->key() == Qt::Key_Control)
    {
        emit(controlPress()); // For multiple box selection with mouse drag
    }
}


/***************
 * cf Qt documentation.
 ***************/
void SelectChannelWindow::keyReleaseEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Shift)
    {
        isShiftPressed = false; // For multiple box selection with shift
    }

    if (e->key() == Qt::Key_Control)
    {
        emit(controlRelease()); // For multiple box selection with mouse drag
    }
}


//      %%%%%%%%%%%%%%%%%%%%%%%%%
//      %%%% METHODS private %%%%
//      %%%%%%%%%%%%%%%%%%%%%%%%%


//      ====================
//      %%%% Checkboxes %%%%
//      ====================


/***************
 * This methods is used to detect if the user want to (un)check multiple
 * checkboxes at the same time (when shift-clicking). This method make use of
 * the two QVector lastInteraction and secondToLastInteraction.
 ***************/
void SelectChannelWindow::checkboxMultipleStateChange()
{
    if (isShiftPressed)
    {
        // Get the checkstate that need to be applied to other checkboxes.
        bool checkStateToApply = channelList[lastInteraction[1]]->fileCheckboxes[lastInteraction[0]]->isChecked();

        // If the last interacted channel is below the second to last
        if (lastInteraction[1] > secondToLastInteraction[1])
        {
            for (int i_chan = secondToLastInteraction[1]; i_chan < lastInteraction[1]; i_chan++)
            {
                if (channelList[i_chan]->fileCheckboxes[lastInteraction[0]]->isEnabled())
                {
                    channelList[i_chan]->fileCheckboxes[lastInteraction[0]]->setChecked(checkStateToApply);
                }
            }
        }

        else
        {
            for (int i_chan = lastInteraction[1]; i_chan < secondToLastInteraction[1] + 1; i_chan++)
            {
                if (channelList[i_chan]->fileCheckboxes[lastInteraction[0]]->isEnabled())
                {
                    channelList[i_chan]->fileCheckboxes[lastInteraction[0]]->setChecked(checkStateToApply);
                }
            }
        }
    }
}


//      =====================
//      %%%% Load / Save %%%%
//      =====================


/***************
 * Read the specified .xml file containing which channel to display.
 * The file does not need to have the exact same channels as the actually loaded
 * .eeg file. It will check which channels are in common between the one
 * actually present and the one this method will try to load, and only keep the
 * channel that are similar.
 *-------------------------
 * QString filename: the file save to laod (.xml format).
 * int fileIndex: the index of the loaded file that will receive the changes.
 * Note that 0 is for all file, so the real index is actually incremented by 1
 * here.
 ***************/
void SelectChannelWindow::xmlRead(QString filename, int fileIndex)
{
    // For info, here's the final structure of the save file in .xml format:

    /*
     *  <ROOT> main node
     *
     *      <Channel IsDisplayed = "bool" Label = "string"/> first list node
     *      ...
     *      ...
     *      ...
     *
     *  </ROOT>
     */


    // Create a document to read XML
    //--------------------
    QDomDocument document;


    // Open a file for reading
    //--------------------
    QFile file(filename);
    document.setContent(&file);


    // Getting root element
    //--------------------
    QDomElement root = document.firstChildElement("ROOT");


    // Reading the file
    //--------------------
    QDomNodeList listNode = root.childNodes(); // -> Channel

    // For each channel to load
    for (int i_load = 0; i_load < listNode.count(); i_load++)
    {
        // For each actually existing channel
        for (int j_chan = 0; j_chan < channelList.length(); j_chan++)
        {
            // If they correspond
            if (listNode.at(i_load).attributes().namedItem("Label").nodeValue()
                    == channelList[j_chan]->label->text())
            {
                int state = listNode.at(i_load).attributes().namedItem("IsDisplayed").nodeValue().toInt();
                bool isChecked = false;

                if (state == 1)
                {
                    isChecked = true;
                }

                // If only one file is changed
                if (fileIndex > 0)
                {
                    channelList[j_chan]->fileCheckboxes[fileIndex - 1]->setChecked(isChecked);
                }

                else
                {
                    for (int k_file = 0; k_file < channelList[j_chan]->fileCheckboxes.length(); k_file++)
                    {
                        channelList[j_chan]->fileCheckboxes[k_file]->setChecked(isChecked);
                    }
                }
            }
        }
    }
}


/***************
 * Save the list of channel and their states (displayed or not) in a .xml file.
 * Only the config of a single data file can be saved. The aim is to use this
 * config on other files. If we save multiple config at the same time, that mean
 * that we'll have to have the same files loaded to apply them (in that case,
 * better to save the entire project).
 *-------------------------
 * QString saveFilename: the path of the future save file.
 * int targetFileIndex: the index of the data file from which the information
 * are taken to save.
 ***************/
void SelectChannelWindow::xmlWrite(QString saveFilename, int fileIndex)
{
    // For info, here's the final structure of the save file in .xml format:

    /*
     *  <ROOT> main node
     *
     *      <Channel IsDisplayed = "bool" Label = "string"/> first list node
     *      ...
     *      ...
     *      ...
     *
     *  </ROOT>
     */


    // Create a document to write XML
    QDomDocument saveDocument;


    // Root element (must be unique! All other nodes are its child!)
    //--------------------
    QDomElement saveFile = saveDocument.createElement("ROOT");
    saveDocument.appendChild(saveFile);


    // Get the channels
    //--------------------
    for (int i_chan = 0; i_chan < channelList.length(); i_chan++)
    {
        QDomElement channel = saveDocument.createElement("Channel");
        channel.setAttribute("Label", channelList[i_chan]->label->text());

        if (channelList[i_chan]->fileCheckboxes[fileIndex]->isChecked())
        {
            channel.setAttribute("IsDisplayed", true);
        }

        else
        {
            channel.setAttribute("IsDisplayed", false);
        }

        saveFile.appendChild(channel);
    }


    // Writing to a file
    //--------------------
    QFile file(saveFilename);

    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "Open the file for writing failed";
    }

    else
    {
        QTextStream stream(&file);
        stream << saveDocument.toString();
        file.close();
        qDebug() << "Save file writing done.";
    }
}


//      ================
//      %%%% Others %%%%
//      ================


void SelectChannelWindow::setMinMaxSize(QWidget *widget,
                                        QSize minSize, QSize maxSize)
{
    widget->setMinimumSize(minSize);
    widget->setMaximumSize(maxSize);
}


//      %%%%%%%%%%%%%%%%%%%%%%%
//      %%%% SLOTS private %%%%
//      %%%%%%%%%%%%%%%%%%%%%%%


//      ====================
//      %%%% Checkboxes %%%%
//      ====================


/***************
 * Simply select all file checkboxes for the specified channel.
 *-------------------------
 * int chanIndex: the index of the targeted channel.
 ***************/
void SelectChannelWindow::selectAllFileForChan(int chanIndex)
{
    isChangeApplied = false;

    for (int i_file = 0; i_file < channelList[chanIndex]->fileCheckboxes.length(); i_file++)
    {
        if (channelList[chanIndex]->fileCheckboxes[i_file]->isEnabled())
        {
            channelList[chanIndex]->fileCheckboxes[i_file]->setChecked(true);
        }
    }
}


/***************
 * Keep track of the lasts checkboxes interacted by the user. Used when the user
 * want to (de)select multiple checboxes at the same time using shift + click.
 *-------------------------
 * int fileID: the file to which is associated the targeted channel.
 * int chanID: the channel to which is associated the checkbox.
 ***************/
void SelectChannelWindow::trackLastInteraction(int fileID, int chanID)
{
    isChangeApplied = false;

    if (lastInteraction[0] == -1 && lastInteraction[1] == -1)
    {
        lastInteraction = {fileID, chanID};
    }

    else
    {
        secondToLastInteraction = lastInteraction;
        lastInteraction = {fileID, chanID};
    }


    if (lastInteraction[0] == secondToLastInteraction[0]
            && lastInteraction[1] != secondToLastInteraction[1])
    {
        checkboxMultipleStateChange();
    }
}


/***************
 * Simply unselect all file checkboxes for the specified channel.
 *-------------------------
 * int chanIndex: the index of the targeted channel.
 ***************/
void SelectChannelWindow::unselectAllFileForChan(int chanIndex)
{
    isChangeApplied = false;

    for (int i_file = 0; i_file < channelList[chanIndex]->fileCheckboxes.length(); i_file++)
    {
        if (channelList[chanIndex]->fileCheckboxes[i_file]->isEnabled())
        {
            channelList[chanIndex]->fileCheckboxes[i_file]->setChecked(false);
        }
    }
}

/***************
 * Ask the user which file to laod, then call the loading method.
 *-------------------------
 * int fileIndex: the data file index target of the channel configuration save..
 ***************/
void SelectChannelWindow::requestLoad(int fileIndex)
{
    // Get the save file name
    //--------------------
    QFileDialog nameDialog;
    QString filename = QFileDialog::getOpenFileName(this,"Choose a file",
                                                    QDir::currentPath(),
                                                    "xml(*.xml)");

    xmlRead(filename, fileIndex);
}


/***************
 * Ask the user where to save the file, give it a name, then call the saving
 * method.
 *-------------------------
 * int fileIndex: the data file index target of the channel configuration save..
 ***************/
void SelectChannelWindow::requestSave(int fileIndex)
{
    // Get the save file name
    //--------------------
    QString filename;

    QFileDialog nameDialog;
    filename = nameDialog.getSaveFileName(this, tr("Choose save location"),
                                          QDir::currentPath() + "/untitled.xml",
                                          tr(".xml"));

    xmlWrite(filename, fileIndex);
}


//      %%%%%%%%%%%%%%%%%%
//      %%%% UI SLOTS %%%%
//      %%%%%%%%%%%%%%%%%%


//      ====================
//      %%%% Checkboxes %%%%
//      ====================


/***************
 * Simply select/unselect all channels.
 *
 * TODO: open a new window to allow to choose groups of channels, and specified
 * one or more file. Like a mass selection manager or smthg.
 ***************/
void SelectChannelWindow::on_selectAllButton_clicked()
{
    isChangeApplied = false;

    for (int i_chan = 0; i_chan < channelList.length(); i_chan++)
    {
        for (int j_file = 0; j_file < channelList[i_chan]->fileCheckboxes.length(); j_file++)
        {
            if (channelList[i_chan]->fileCheckboxes[j_file]->isEnabled())
            {
                if (selectAll)
                {
                    channelList[i_chan]->fileCheckboxes[j_file]->setChecked(true);
                    this->ui->selectAllButton->setText("Unselect All");
                }

                else
                {
                    channelList[i_chan]->fileCheckboxes[j_file]->setChecked(false);
                    this->ui->selectAllButton->setText("Select All");
                }
            }
        }
    }

    selectAll = !selectAll;
}


//      =====================
//      %%%% Load / Save %%%%
//      =====================


/***************
 * The user can save a particular selection of channels in order to load it for
 * another file.
 ***************/
void SelectChannelWindow::on_loadConfigurationButton_clicked()
{
    // Get the target file index
    //--------------------
    QString message = "Choose the file that will receive the modifications (can be all of them):";

    QVector<QString> fileList = filenameList;
    fileList.insert(0, "All File");

    chooseFile = new ChooseFileWindow(fileList, message);

    connect(chooseFile, SIGNAL(sendFileIndex(int)),
            this, SLOT(requestLoad(int)));

    chooseFile->show();
}


/***************
 * Save in a .xml file which channel are displayed. It does not save which are
 * not displayed because it's implicit.
 ***************/
void SelectChannelWindow::on_saveConfigurationButton_clicked()
{
    // Get the target file index
    //--------------------
    QString message = "Choose which file to save the channel configuration of:";

    chooseFile = new ChooseFileWindow(filenameList, message);

    connect(chooseFile, SIGNAL(sendFileIndex(int)),
            this, SLOT(requestSave(int)));

    chooseFile->show();
}


//      ================
//      %%%% Others %%%%
//      ================


/***************
 * Apply the change made by the user by sending which channel to display in the
 * CentralMiddleWidget.
 ***************/
void SelectChannelWindow::on_applyButton_clicked()
{
    isChangeApplied = true;

    QVector<QVector<QString>> presentChannels;
    QVector<QVector<bool>> newChannelStates;

    presentChannels.resize(plotData->eegFileNb);
    newChannelStates.resize(plotData->eegFileNb);

    for (int i_file = 0; i_file < plotData->eegFileNb; i_file++)
    {
        for (int j_chan = 0; j_chan < channelList.length(); j_chan++)
        {
            if (channelList[j_chan]->fileCheckboxes[i_file]->isEnabled())
            {
                presentChannels[i_file].append(channelList[j_chan]->label->text());
                newChannelStates[i_file].append(channelList[j_chan]->fileCheckboxes[i_file]->isChecked());
            }
        }
    }

    plotData->changeAllChannelStates(presentChannels, newChannelStates);
}


/***************
 * Close the window even if changes were made, no questions asked.
 ***************/
void SelectChannelWindow::on_cancelButton_clicked()
{
    this->~SelectChannelWindow();
}


/***************
 * Close the window. If the user made changed without applying them, it will ask
 * if he want to really quit not.
 ***************/
void SelectChannelWindow::on_closeButton_clicked()
{
    if (!isChangeApplied)
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("You didn't saved your changes");
        msgBox.setText("Closing the window before applying the changes will"
                       "discard them.");
        msgBox.setInformativeText("Are you sure you want to quit?");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        int ret = msgBox.exec();

        if (ret == QMessageBox::Yes)
        {
            // Discard Changes
            this->~SelectChannelWindow();
        }
        else if (ret == QMessageBox::No)
        {
            // Return to Event Window
            msgBox.close();
        }
    }

    else
    {
        this->~SelectChannelWindow();
    }
}
