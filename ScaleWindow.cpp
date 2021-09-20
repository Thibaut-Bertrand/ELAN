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
 * Create and manage the Scale Window, which is used to modify the scale of
 * graphs' x- (time) and y-axis (amplitude), displayed in the Central Widget.
 *
 * Author: Thibaut Bertrand
 * thibaut.bertrand@inserm.fr
 ***************/


#include "ScaleWindow.h"
#include "ui_ScaleWindow.h"


/***************
 * Constructor
 *-------------------------
 * PageData* page: the dataContainer of interest for this class.
 * PlotData* plot: the dataContainer of interest for this class.
***************/
ScaleWindow::ScaleWindow(PageData* page, PlotData* plot, QWidget* parent) :
    QDialog(parent),
    ui(new Ui::ScaleWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("Scale management");

    pageData = page;
    plotData = plot;


    //==============================
    // Time Scale
    //==============================

    // The time scale is in milliseconds
    ui->timeScaleEdit->setText(QString::number(pageData->timeScale));
    ui->timeScaleEdit->setValidator(intValidator);


    //==============================
    // Amplitude scales
    //==============================


    // File references layout
    //--------------------
    for (int i_file = 0; i_file < plotData->eegFileNb; i_file++)
    {
        QString filename = plotData->eegDataStorage[i_file]->eegFilename;
        QLabel* fileLabel = new QLabel("File " + QString::number(i_file) + ": " + filename);

        filenameList.append(filename);

        this->ui->fileReferenceLayout->addWidget(fileLabel);
    }

    this->ui->fileReferenceLayout->addStretch();


    // Channel labels layout
    //--------------------
    QVBoxLayout* channelLabelLayout = new QVBoxLayout;

    QLabel* empty = new QLabel("");
    empty->setFont(font);
    empty->setAlignment(Qt::AlignCenter);
    setMinMaxSize(empty, labelSize, labelSize);

    channelLabelLayout->addWidget(empty);

    for (int i_chan = 0; i_chan < plotData->uniqueChannelNb; i_chan++)
    {
        QString label = plotData->uniqueChannelLabelList[i_chan];
        channelLabels.append(label);

        QLabel* channelLabel = new QLabel(label);
        channelLabel->setFont(font);
        channelLabel->setAlignment(Qt::AlignCenter);
        setMinMaxSize(channelLabel, labelSize, labelSize);

        channelLabelLayout->addWidget(channelLabel);
    }

    channelLabelLayout->addStretch();
    channelLabelLayout->setSpacing(layoutSpacing);
    this->ui->amplitudeScaleLayout->addLayout(channelLabelLayout);


    // File line edit layout
    //--------------------
    files.clear();

//    int k_label = 0;
    bool channelSet = false;
    doubleValidator->setNotation(QDoubleValidator::StandardNotation);

    for (int i_file = 0; i_file < plotData->eegFileNb; i_file++)
    {
        file newFile;

        QVBoxLayout* fileLayout = new QVBoxLayout;


        // File label
        QLabel* fileLabel = new QLabel("File " + QString::number(i_file));
        fileLabel->setFont(font);
        fileLabel->setAlignment(Qt::AlignCenter);
        setMinMaxSize(fileLabel, labelSize, labelSize);

        fileLayout->addWidget(fileLabel);


        // Line edit
//        k_label = 0;

        for (int j_label = 0; j_label < channelLabels.length(); j_label++)
        {
            channelSet = false;
            for (int k_chan = 0; k_chan < plotData->eegDataStorage[i_file]->channelNb; k_chan++)
            {
                if (channelLabels[j_label] == plotData->eegDataStorage[i_file]->channels[k_chan]->label)
                {
                    LineEditID* lineEdit = new LineEditID(i_file, k_chan, plotData->eegDataStorage[i_file]->channels[k_chan]->type);
                    lineEdit->setFont(font);
                    lineEdit->setValidator(doubleValidator);
                    lineEdit->setText((QString::number(plotData->eegDataStorage[i_file]->channels[k_chan]->amplitudeRange.upper, 'g', 10)));
                    setMinMaxSize(lineEdit, labelSize, labelSize);
                    newFile.channelLineEdit.append(lineEdit);
                    newFile.channelNames.append(plotData->eegDataStorage[i_file]->channels[k_chan]->label);
                    fileLayout->addWidget(lineEdit);

                    channelSet = true;
                }

            }

            if (!channelSet)
            {
                // Create empty widget
                QLabel* empty = new QLabel;
                empty->setText("-");
                empty->setFont(font);
                setMinMaxSize(empty, labelSize, labelSize);
                fileLayout->addWidget(empty);
            }

        }

        files.append(newFile);


        // Layouts
        fileLayout->addStretch();
        fileLayout->setSpacing(layoutSpacing);
        this->ui->amplitudeScaleLayout->addLayout(fileLayout);
    }

    this->ui->amplitudeScaleLayout->addStretch();
    this->ui->amplitudeScaleLayout->setSpacing(layoutSpacing);
}


/***************
 * Destructor
 ***************/
ScaleWindow::~ScaleWindow()
{
    delete ui;
}

//      %%%%%%%%%%%%%%%%%%%%%%%%%
//      %%%% METHODS private %%%%
//      %%%%%%%%%%%%%%%%%%%%%%%%%


void ScaleWindow::setMinMaxSize(QWidget* widget, QSize minSize, QSize maxSize)
{
    widget->setMinimumSize(minSize);
    widget->setMaximumSize(maxSize);
}


//      =====================
//      %%%% Load / Save %%%%
//      =====================

/***************
 * Read the specified .xml file containing the various channel scales.
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
void ScaleWindow::xmlRead(QString filename, int fileIndex)
{
    // For info, here's the final structure of the save file in .xml format:

    /*
     *  <ROOT> main node
     *
     *      <Channel Label = "string" Scale = "string"/> first list node
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


    // If only one file is concerned
    if (fileIndex > 0)
    {
        // For each channel to load
        for (int i_load = 0; i_load < listNode.count(); i_load++)
        {
            // For each actually existing channel
            for (int j_chan = 0; j_chan < files[fileIndex - 1].channelNames.length(); j_chan++)
            {
                // If they correspond
                if (listNode.at(i_load).attributes().namedItem("Label").nodeValue()
                        == files[fileIndex - 1].channelNames[j_chan])
                {
                    QString newScale = listNode.at(i_load).attributes().namedItem("Scale").nodeValue();
                    files[fileIndex - 1].channelLineEdit[j_chan]->setText(newScale);
                }
            }
        }
    }

    else
    {
        for (int i_file = 0; i_file < files.length(); i_file++)
        {
            // For each channel to load
            for (int j_load = 0; j_load < listNode.count(); j_load++)
            {
                // For each actually existing channel
                for (int k_chan = 0; k_chan < files[i_file].channelNames.length(); k_chan++)
                {
                    // If they correspond
                    if (listNode.at(j_load).attributes().namedItem("Label").nodeValue()
                            == files[i_file].channelNames[k_chan])
                    {
                        QString newScale = listNode.at(j_load).attributes().namedItem("Scale").nodeValue();
                        files[i_file].channelLineEdit[k_chan]->setText(newScale);
                    }
                }
            }
        }
    }
}


/***************
 * Save the list of channel and their scales in a .xml file.
 * Only the config of a single data file can be saved. The aim is to use this
 * config on other files. If we save multiple config at the same time, that mean
 * that we'll have to have the same files loaded to apply them (in that case,
 * better to save the entire project).
 *-------------------------
 * QString saveFilename: the path of the future save file.
 * int targetFileIndex: the index of the data file from which the information
 * are taken to save.
 ***************/
void ScaleWindow::xmlWrite(QString saveFilename, int targetFileIndex)
{
    // For info, here's the final structure of the save file in .xml format:

    /*
     *  <ROOT> main node
     *
     *      <Channel Label = "string" Scale = "string"/> first list node
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
    for (int i_chan = 0; i_chan < files[targetFileIndex].channelLineEdit.length(); i_chan++)
    {
        QDomElement channel = saveDocument.createElement("Channel");
        channel.setAttribute("Label", files[targetFileIndex].channelNames[i_chan]);
        channel.setAttribute("Scale", files[targetFileIndex].channelLineEdit[i_chan]->text());

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


//      %%%%%%%%%%%%%%%%%%%%%%%
//      %%%% SLOTS private %%%%
//      %%%%%%%%%%%%%%%%%%%%%%%


/***************
 * Set the specified scale to all concerned channels.
 *-------------------------
 * QVector<int> filesID: the file(s) concerned by the changes.
 * QString channelType: the type of channel concerned.
 * QString newAmplitudeScale: the new scale to set to the channels
 ***************/
void ScaleWindow::applyMultipleNewScales(QVector<int> filesID,
                                         QString channelType,
                                         QString newAmplitudeScale)
{
    for (int i_file = 0; i_file < filesID.length(); i_file++)
    {
        int fileID = filesID[i_file];

        for (int j_chan = 0; j_chan < files[fileID].channelLineEdit.length(); j_chan++)
        {
            if (files[fileID].channelLineEdit[j_chan]->type == channelType)
            {
                files[fileID].channelLineEdit[j_chan]->setText(newAmplitudeScale);
            }
        }
    }
}


/***************
 * Ask the file that the user wants to load.
 *-------------------------
 * int fileIndex: the file index that will receive the scales from the save
 * file that is laoded. < 0 (generally -1) for all file.
 ***************/
void ScaleWindow::requestLoad(int fileIndex)
{
    // Get the save file name
    //--------------------
    QFileDialog nameDialog;
    QString filename = QFileDialog::getOpenFileName(this, "Choose a file",
                                                    QDir::currentPath(),
                                                    "xml(*.xml)");

    xmlRead(filename, fileIndex);
}


/***************
 * Ask the user where to save the file, also the name of the save file.
 *-------------------------
 * int fileIndex: the file index that from which the scale values are taken to
 * save.
 ***************/
void ScaleWindow::requestSave(int fileIndex)
{
    // Get the save file name
    //--------------------
    QFileDialog nameDialog;
    QString filename = nameDialog.getSaveFileName(this,
                                                  tr("Choose save location"),
                                                  QDir::currentPath() + "/untitled.xml",
                                                  tr(".xml"));

    xmlWrite(filename, fileIndex);
}


//      %%%%%%%%%%%%%%%%%%
//      %%%% UI SLOTS %%%%
//      %%%%%%%%%%%%%%%%%%


//      ====================
//      %%%% Time scale %%%%
//      ====================


/***************
 * Set the time scale (in milliseconds) to the value specified by the user in
 * the window.
 ***************/
void ScaleWindow::on_applyTimeButton_clicked()
{
    pageData->setTimeScale(ui->timeScaleEdit->text().toInt());
}


/***************
 * Set the time scale (in milliseconds) to the default value specified in the
 * PageData data container.
 * Note: default value is set at 10000 milliseconds (= 10 seconds).
 ***************/
void ScaleWindow::on_defaultTimeButton_clicked()
{
    pageData->setDefaultTimeScale();
}


//      ==========================
//      %%%% Amplitude scales %%%%
//      ==========================


//      %%%% Load and Save configuration %%%%


/***************
 * Load the configuration stored in a .xml save file. The user have to choose
 * which file will receive the modifications (can be all of them).
 ***************/
void ScaleWindow::on_loadAmplitudeButton_clicked()
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
 * Save the configuration stored in a .xml save file. The user have to choose
 * which file have to be saved.
 ***************/
void ScaleWindow::on_saveAmplitudeButton_clicked()
{
    // Get the target file index
    //--------------------
    QString message = "Choose which file to save the channel configuration of:";

    chooseFile = new ChooseFileWindow(filenameList, message);

    connect(chooseFile, SIGNAL(sendFileIndex(int)),
            this, SLOT(requestSave(int)));

    chooseFile->show();
}


//      %%%% Change scales %%%%


/***************
 * Apply the changes made by the user.
 ***************/
void ScaleWindow::on_applyAmplitudeButton_clicked()
{
    QVector<QVector<int>> newAmplitudeSCales;

    for (int i_file = 0; i_file < files.length(); i_file++)
    {
        QVector<int> fileAmplitudeScales;

        for (int j_chan = 0; j_chan < files[i_file].channelLineEdit.length(); j_chan++)
        {
            fileAmplitudeScales.append(files[i_file].channelLineEdit[j_chan]->text().toInt());
        }

        newAmplitudeSCales.append(fileAmplitudeScales);
    }

    plotData->setChannelsAmplitudeScale(newAmplitudeSCales);
}


/***************
 * Set the default value to all the channels
 ***************/
void ScaleWindow::on_setDefaultsButton_clicked()
{
    for (int i_file = 0; i_file < files.length(); i_file++)
    {
        for (int j_chan = 0; j_chan < files[i_file].channelLineEdit.length(); j_chan++)
        {
            if (files[i_file].channelLineEdit[j_chan]->type == "EEG")
            {
                files[i_file].channelLineEdit[j_chan]->setText(QString::number(plotData->defaultUVAmplitudeScale));
            }

            else if (files[i_file].channelLineEdit[j_chan]->type == "MEG")
            {
                files[i_file].channelLineEdit[j_chan]->setText(QString::number(plotData->defaultFTAmplitudeScale));
            }

            else
            {
                files[i_file].channelLineEdit[j_chan]->setText(QString::number(plotData->defaultVAmplitudeScale));
            }
        }
    }
}


/***************
 * Open a window to allow the user to make mass changes to whole group of
 * channels.
 ***************/
void ScaleWindow::on_setScalesButton_clicked()
{
    changeMultipleScalesWindow = new ChangeMultipleAmplitudeScales(files.length());

    connect(this->changeMultipleScalesWindow, SIGNAL(sendNewScales(QVector<int>, QString, QString)),
            this, SLOT(applyMultipleNewScales(QVector<int>, QString, QString)));

    changeMultipleScalesWindow->show();
}


//      ================
//      %%%% Others %%%%
//      ================


/***************
 * Simply close the window.
 ***************/
void ScaleWindow::on_closeButton_clicked()
{
    this->~ScaleWindow();
}
