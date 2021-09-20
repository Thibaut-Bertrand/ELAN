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
 * Create and manage the Event Window, which is used to modify the visibility
 * and browsability state of events, as well as to modify or delete existing
 * event or create new ones.
 * A visible event is, obviously, visible on the sreen.
 * A browsable event is always visible, and is an event that the user can use to
 * focus the view in the screen, so that it can be used instead of the page
 * view.
 *
 * Author: Thibaut Bertrand
 * thibaut.bertrand@inserm.fr
 ***************/


#include "EventWindow.h"
#include "ui_EventWindow.h"


/***************
 * Constructor
 *-------------------------
 * PlotData* plot: reference to the PlotData data container of the project.
 ***************/
EventWindow::EventWindow(PlotData* plot, QWidget* parent) :
    QDialog(parent), ui(new Ui::EventWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("Event management");


    plotData = plot;


    // Creating the tab widget
    // There will be one tab for each loaded event file
    //--------------------
    QTabWidget* tabWidget = new QTabWidget;
    this->ui->wrapperLayout->addWidget(tabWidget);


    // For each event file
    //--------------------
    for (int i_file = 0; i_file < plotData->eegFileNb; i_file++)
    {
        // Event file info
        //--------------------
        eventFile newEventFile;
        newEventFile.fileIndex = plotData->eegDataStorage[i_file]->activeEventFileIndex;
        newEventFile.filename = plotData->eegDataStorage[i_file]->eventDataStorage[newEventFile.fileIndex]->eventFilename;


        // File reference layout
        // Its the top most layout. Instead of writing the whole filename in
        // each tab, I just write "File x" to take less space. The reference
        // layout is just used to indicate which file is which.
        //--------------------
        QLabel* fileReferenceLabel = new QLabel;
        fileReferenceLabel->setFont(font);
        fileReferenceLabel->setText("File " + QString::number(i_file) + ": " + newEventFile.filename);
        this->ui->fileReferenceLayout->addWidget(fileReferenceLabel);


        // Tab widget
        //--------------------
        QScrollArea* scrollArea = new QScrollArea;
        tabWidget->addTab(scrollArea, "File " + QString::number(i_file));

        QWidget* fileContainer = new QWidget;

        newEventFile.layout = new QVBoxLayout;

        fileContainer->setLayout(newEventFile.layout);

        scrollArea->setWidget(fileContainer);
        scrollArea->setWidgetResizable(true);


        // Event file buttons
        //--------------------

        // Add event
        newEventFile.addEventButton = new ButtonID(i_file);
        newEventFile.addEventButton->setText("Add Event");
        newEventFile.addEventButton->setFont(font);
        setMinMaxSize(newEventFile.addEventButton, buttonSize, buttonSize);

        connect(newEventFile.addEventButton, SIGNAL(sendID(int, int)),
                this, SLOT(requestAddNewEvent(int)));


        // Apply to all file button
        newEventFile.applyToAllFileButton = new ButtonID(i_file);
        newEventFile.applyToAllFileButton->setText("Apply to All File");
        newEventFile.applyToAllFileButton->setFont(font);
        setMinMaxSize(newEventFile.applyToAllFileButton, bigButtonSize, bigButtonSize);

        connect(newEventFile.applyToAllFileButton, SIGNAL(sendID(int, int)),
                this, SLOT(applyToAllFile(int)));


        // Put the buttons in a layout
        QHBoxLayout* buttonLayout = new QHBoxLayout;
        buttonLayout->addWidget(newEventFile.addEventButton);
        buttonLayout->addWidget(newEventFile.applyToAllFileButton);
        buttonLayout->addStretch();

        newEventFile.layout->addLayout(buttonLayout);


        // Checkboxes labels
        //--------------------

        // Code
        QLabel* codeLabel = new QLabel;
        codeLabel->setText("Code");
        codeLabel->setFont(font);
        setMinMaxSize(codeLabel, labelSize, labelSize);
        codeLabel->setAlignment(Qt::AlignCenter);

        // Visible
        QLabel* visibleLabel = new QLabel;
        visibleLabel->setText("Visible");
        visibleLabel->setFont(font);
        setMinMaxSize(visibleLabel, labelSize, labelSize);
        visibleLabel->setAlignment(Qt::AlignCenter);

        // Browsable
        QLabel* browsableLabel = new QLabel;
        browsableLabel->setText("Browsable");
        browsableLabel->setFont(font);
        setMinMaxSize(browsableLabel, labelSize, labelSize);
        browsableLabel->setAlignment(Qt::AlignCenter);

        // Recode
        QLabel* recodeLabel = new QLabel;
        recodeLabel->setText("Recode");
        recodeLabel->setFont(font);
        setMinMaxSize(recodeLabel, labelSize, labelSize);
        recodeLabel->setAlignment(Qt::AlignCenter);

        // Delete
        QLabel* deleteLabel = new QLabel;
        deleteLabel->setText("Delete");
        deleteLabel->setFont(font);
        setMinMaxSize(deleteLabel, labelSize, labelSize);
        deleteLabel->setAlignment(Qt::AlignCenter);

        // Put the labels in a layout
        QHBoxLayout* labelLayout = new QHBoxLayout;
        labelLayout->addWidget(codeLabel);
        labelLayout->addWidget(visibleLabel);
        labelLayout->addWidget(browsableLabel);
        labelLayout->addWidget(recodeLabel);
        labelLayout->addWidget(deleteLabel);
        labelLayout->addStretch();

        newEventFile.layout->addLayout(labelLayout);


        // For each event of the file
        // Each event is composed of 1 label, 4 checkboxes and 1 button
        //--------------------
        for (int j_event = 0; j_event < plotData->eegDataStorage[i_file]->eventDataStorage[newEventFile.fileIndex]->uniqueEventNb; j_event++)
        {
            event newEvent;


            // Event label
            //--------------------
            int code = plotData->eegDataStorage[i_file]->eventDataStorage[newEventFile.fileIndex]->eventStates[j_event][0];
            newEvent.label = new QLabel(QString::number(code));
            setMinMaxSize(newEvent.label, labelSize, labelSize);
            newEvent.label->setFont(font);
            newEvent.label->setAlignment(Qt::AlignCenter);


            // Event visible checkbox
            //--------------------
            newEvent.visibleCheckbox = new CheckboxID(i_file, j_event, 0); // Visible checkboxes type have a type of 0
            newEvent.visibleCheckbox->setText("");
            setMinMaxSize(newEvent.visibleCheckbox, labelSize, labelSize);

            if (plotData->eegDataStorage[i_file]->eventDataStorage[newEventFile.fileIndex]->eventStates[j_event][1] == 1)
            {
                newEvent.visibleCheckbox->setChecked(true);
            }

            else
            {
                newEvent.visibleCheckbox->setChecked(false);
            }

            connect(newEvent.visibleCheckbox, SIGNAL(sendInfoShort(int, int)),
                    this, SLOT(changeBrowsableCheckBoxState(int, int)));

            connect(newEvent.visibleCheckbox, SIGNAL(sendThreeID(int, int, int)),
                    this, SLOT(trackLastInteraction(int, int, int)));

            // Disabled them because the browsable boxes don't update automatically
//            connect(this, SIGNAL(controlPress()),
//                    newEvent.visibleCheckbox, SLOT(controlPress()));

//            connect(this, SIGNAL(controlRelease()),
//                    newEvent.visibleCheckbox, SLOT(controlRelease()));


            // Event browsable checkbox
            //--------------------
            newEvent.browsableCheckbox = new CheckboxID(i_file, j_event, 1); // Browsable checkboxes type have a type of 1
            newEvent.browsableCheckbox->setText("");
            setMinMaxSize(newEvent.browsableCheckbox, labelSize, labelSize);

            if (plotData->eegDataStorage[i_file]->eventDataStorage[newEventFile.fileIndex]->eventStates[j_event][2] == 1)
            {
                newEvent.browsableCheckbox->setChecked(true);
            }

            else
            {
                newEvent.browsableCheckbox->setChecked(false);
            }

            connect(newEvent.browsableCheckbox, SIGNAL(sendInfoShort(int, int)),
                    this, SLOT(changeVisibleCheckBoxState(int, int)));

            connect(newEvent.browsableCheckbox, SIGNAL(sendThreeID(int, int, int)),
                    this, SLOT(trackLastInteraction(int, int, int)));

            // Disabled them because the visible boxes don't update automatically
//            connect(this, SIGNAL(controlPress()),
//                    newEvent.browsableCheckbox, SLOT(controlPress()));

//            connect(this, SIGNAL(controlRelease()),
//                    newEvent.browsableCheckbox, SLOT(controlRelease()));


            // Event recode checkbox
            //--------------------
            newEvent.recodeCheckbox = new CheckboxID(i_file, j_event); // Recode chekboxes don't need a type here
            newEvent.recodeCheckbox->setText("");
            newEvent.recodeCheckbox->setFont(font);
            setMinMaxSize(newEvent.recodeCheckbox, labelSize, labelSize);

            connect(newEvent.recodeCheckbox, SIGNAL(sendInfoShort(int, int)),
                    this, SLOT(recodeCheckboxChanged(int, int)));


            // Event delete checkbox
            //--------------------
            newEvent.deleteCheckbox = new CheckboxID(i_file, j_event, 2); // Delete checkboxes type have a type of 2
            newEvent.deleteCheckbox->setText("");
            setMinMaxSize(newEvent.deleteCheckbox, labelSize, labelSize);

            connect(newEvent.deleteCheckbox, SIGNAL(sendThreeID(int, int, int)),
                    this, SLOT(trackLastInteraction(int, int, int)));

            connect(this, SIGNAL(controlPress()),
                    newEvent.deleteCheckbox, SLOT(controlPress()));

            connect(this, SIGNAL(controlRelease()),
                    newEvent.deleteCheckbox, SLOT(controlRelease()));


            // Event layout
            //--------------------
            QHBoxLayout* eventLayout = new QHBoxLayout;
            eventLayout ->addWidget(newEvent.label);
            eventLayout ->addWidget(newEvent.visibleCheckbox);
            eventLayout ->addWidget(newEvent.browsableCheckbox);
            eventLayout ->addWidget(newEvent.recodeCheckbox);
            eventLayout ->addWidget(newEvent.deleteCheckbox);
            eventLayout ->addStretch();


            // Add the new event to the list
            //--------------------
            newEventFile.events.append(newEvent);
            newEventFile.layout->addLayout(eventLayout);
        }

        newEventFile.layout->addStretch();

        eventFiles.append(newEventFile);
    }

    this->ui->fileReferenceLayout->addStretch();
}


/***************
 * Destructor
 ***************/
EventWindow::~EventWindow()
{
    delete ui;
}


//      %%%%%%%%%%%%%%%%%%%%%%%%%%
//      %%%% METHODS override %%%%
//      %%%%%%%%%%%%%%%%%%%%%%%%%%


/***************
 * Ask confirmation before closing.
 ***************/
void EventWindow::closeEvent(QCloseEvent *e)
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Warning: changes not applied");
    msgBox.setText("If you had made any changes, closing the window will "
                   "discard them. If you want to apply them, click on "
                   "the 'Apply' button at the bottom of the window.");
    msgBox.setInformativeText("Are you sure you want to quit?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    int ret = msgBox.exec();

    if (ret == QMessageBox::Yes)
    {
        // Discard Changes
        this->~EventWindow();
    }
    else if (ret == QMessageBox::No)
    {
        // Return to Event Window
        msgBox.close();
        e->ignore();
    }
}


/***************
 * cf Qt documentation.
 ***************/
void EventWindow::keyPressEvent(QKeyEvent *e)
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
void EventWindow::keyReleaseEvent(QKeyEvent *e)
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


/***************
 * When the user wants to apply the changes s-he made, this method is called.
 ***************/
void EventWindow::applyChanges()
{
    plotData->listOfEventsToChange.clear();
    plotData->listOfEventsToChange.resize(eventFiles.length());

    for (int i_file = 0; i_file < eventFiles.length(); i_file++)
    {
        plotData->listOfEventsToChange[i_file].eegFileID = i_file;

        for (int j_event = 0; j_event < eventFiles[i_file].events.length(); j_event++)
        {
            // Deleted events
            if (eventFiles[i_file].events[j_event].deleteCheckbox->isChecked())
            {
                plotData->listOfEventsToChange[i_file].eventCodesToDelete.append(eventFiles[i_file].events[j_event].label->text().toInt());
            }

            else if (!eventFiles[i_file].events[j_event].deleteCheckbox->isChecked())
            {
                // Added events
                if (eventFiles[i_file].events[j_event].isNewEvent)
                {
                    int eventToAdd = eventFiles[i_file].events[j_event].label->text().toInt();
                    plotData->listOfEventsToChange[i_file].eventCodesToAdd.append(eventToAdd);
                }


                // Recoded events
                if (eventFiles[i_file].events[j_event].recodeCheckbox->isChecked())
                {
                    int originalCode = eventFiles[i_file].events[j_event].label->text().toInt();
                    int newCode = eventFiles[i_file].events[j_event].recodeCheckbox->text().toInt();
                    plotData->listOfEventsToChange[i_file].eventCodesToRecode.append(QVector<int>{originalCode, newCode});
                }


                // Event states
                bool visibility = eventFiles[i_file].events[j_event].visibleCheckbox->isChecked();
                bool browsability = eventFiles[i_file].events[j_event].browsableCheckbox->isChecked();
                plotData->listOfEventsToChange[i_file].remainingEventStates.append(QVector<bool>{visibility, browsability});
            }
        }
    }

    plotData->applyChangesToEventFiles();
}


/***************
 * This methods is used to detect if the user want to (un)check multiple
 * checkboxes at the same time (when shift-clicking). This method make use of
 * the two QVector lastInteraction and secondToLastInteraction.
 ***************/
void EventWindow::checkboxMultipleStateChange()
{
    if (isShiftPressed)
    {
        // Get the checkstate that need to be applied to other checkboxes.
        bool checkStateToApply;

        switch (lastInteraction[2])
        {
        case 0: // Visible checkboxes

            checkStateToApply = eventFiles[lastInteraction[0]].events[lastInteraction[1]].visibleCheckbox->checkState();

            // If the last interacted event is after the second to last
            if (lastInteraction[1] > secondToLastInteraction[1])
            {
                for (int i_event = secondToLastInteraction[1]; i_event < lastInteraction[1]; i_event++)
                {
                    eventFiles[lastInteraction[0]].events[i_event].visibleCheckbox->setNewState(checkStateToApply);
                }
            }

            else
            {
                for (int i_event = lastInteraction[1]; i_event < secondToLastInteraction[1]; i_event++)
                {
                    eventFiles[lastInteraction[0]].events[i_event].visibleCheckbox->setNewState(checkStateToApply);
                }
            }

            break;


        case 1: // Browsable checkboxes

            checkStateToApply = eventFiles[lastInteraction[0]].events[lastInteraction[1]].browsableCheckbox->checkState();

            // If the last interacted event is after the second to last
            if (lastInteraction[1] > secondToLastInteraction[1])
            {
                for (int i_event = secondToLastInteraction[1]; i_event < lastInteraction[1]; i_event++)
                {
                    eventFiles[lastInteraction[0]].events[i_event].browsableCheckbox->setNewState(checkStateToApply);
                }
            }

            else
            {
                for (int i_event = lastInteraction[1]; i_event < secondToLastInteraction[1]; i_event++)
                {
                    eventFiles[lastInteraction[0]].events[i_event].browsableCheckbox->setNewState(checkStateToApply);
                }
            }

            break;


        case 2: // Delete checkboxes

            checkStateToApply = eventFiles[lastInteraction[0]].events[lastInteraction[1]].deleteCheckbox->checkState();

            // If the last interacted event is after the second to last
            if (lastInteraction[1] > secondToLastInteraction[1])
            {
                for (int i_event = secondToLastInteraction[1]; i_event < lastInteraction[1]; i_event++)
                {
                    eventFiles[lastInteraction[0]].events[i_event].deleteCheckbox->setChecked(checkStateToApply);
                }
            }

            else
            {
                for (int i_event = lastInteraction[1]; i_event < secondToLastInteraction[1]; i_event++)
                {
                    eventFiles[lastInteraction[0]].events[i_event].deleteCheckbox->setChecked(checkStateToApply);
                }
            }

            break;


         default:

            qDebug() << "ERROR: EventWindow, checkboxMultipleStateChange(), checkbox type does not exist.";
        }
    }
}


/***************
 * A simple method to set the minimum and maximum of the specified widget. As
 * those two methods are often called together, I prefer to put them here.
 *-------------------------
 * QWidget* widget: the pointer to the widget which sizes have to be set.
 * QSize min: the minimum size.
 * QSize max: the maximum size.
 ***************/
void EventWindow::setMinMaxSize(QWidget *widget, QSize min, QSize max)
{
    widget->setMinimumSize(min);
    widget->setMaximumSize(max);
}


//      =====================
//      %%%% Load / Save %%%%
//      =====================


/***************
 * Read the specified .xml file containing events and their states (visible and
 * browsable).
 * The file does not need to have the exact same events as the actually loaded
 * .pos file. It will check which events are in common between the one actually
 * present and the one this method will try to load, and only keep the event
 * that are similar.
 *-------------------------
 * QString filename: the file save to laod (.xml format).
 * int fileIndex: the index of the loaded file that will receive the changes.
 * Note that 0 is for all file, so the real index is actually incremented by 1
 * here.
 ***************/
void EventWindow::xmlRead(QString filename, int fileIndex)
{
    // For info, here's the final structure of the save file in .xml format:

    /*
     *  <ROOT> main node
     *
     *      <Event Code = "string" IsVisible = "bool" IsBrowsable = "bool"/> first list node
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
    QDomNodeList listNode = root.childNodes(); // -> Event


    // If only one file is concerned
    if (fileIndex > 0)
    {
        // For each event to load
        for (int i_load = 0; i_load < listNode.count(); i_load++)
        {
            // For each actually existing event
            for (int j_event = 0; j_event < eventFiles[fileIndex - 1].events.length(); j_event++)
            {
                // If they correspond
                if (listNode.at(i_load).attributes().namedItem("Code").nodeValue()
                        == eventFiles[fileIndex - 1].events[j_event].label->text())
                {
                    bool isVisible;
                    listNode.at(i_load).attributes().namedItem("IsVisible").nodeValue().toInt() == 1 ?
                                isVisible = true :
                                isVisible = false;

                    bool isBrowsable = false;
                    listNode.at(i_load).attributes().namedItem("IsBrowsable").nodeValue().toInt() == 1 ?
                                isBrowsable = true :
                                isBrowsable = false;

                    eventFiles[fileIndex - 1].events[j_event].visibleCheckbox->setChecked(isVisible);
                    eventFiles[fileIndex - 1].events[j_event].browsableCheckbox->setChecked(isBrowsable);
                }
            }
        }
    }

    else
    {
        for (int i_file = 0; i_file < eventFiles.length(); i_file++)
        {
            // For each channel to load
            for (int j_load = 0; j_load < listNode.count(); j_load++)
            {
                // For each actually existing channel
                for (int k_event = 0; k_event < eventFiles[i_file].events.length(); k_event++)
                {
                    // If they correspond
                    if (listNode.at(j_load).attributes().namedItem("Code").nodeValue()
                            == eventFiles[i_file].events[k_event].label->text())
                    {
                        bool isVisible;
                        listNode.at(j_load).attributes().namedItem("IsVisible").nodeValue().toInt() == 1 ?
                                    isVisible = true :
                                    isVisible = false;

                        bool isBrowsable = false;
                        listNode.at(j_load).attributes().namedItem("IsBrowsable").nodeValue().toInt() == 1 ?
                                    isBrowsable = true :
                                    isBrowsable = false;

                        eventFiles[i_file].events[k_event].visibleCheckbox->setChecked(isVisible);
                        eventFiles[i_file].events[k_event].browsableCheckbox->setChecked(isBrowsable);
                    }
                }
            }
        }
    }
}


/***************
 * Save the list of event and their states (visible/browsable) in a .xml file.
 * Only the config of a single event file can be saved. The aim is to use this
 * config on other files. If we save multiple config at the same time, that mean
 * that we'll have to have the same files loaded to apply them (in that case,
 * better to save the entire project).
 *-------------------------
 * QString saveFilename: the path of the future save file.
 * int targetFileIndex: the index of the event file from which the information
 * are taken to save.
 ***************/
void EventWindow::xmlWrite(QString saveFilename, int fileIndex)
{
    // For info, here's the final structure of the save file in .xml format:

    /*
     *  <ROOT> main node
     *
     *      <Event Code = "string" IsBrowsable = "bool" IsVisible = "bool"/> first list node
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


    // Get the event
    //--------------------
    for (int i_event = 0; i_event < eventFiles[fileIndex].events.length(); i_event++)
    {
        QDomElement event = saveDocument.createElement("Event");
        event.setAttribute("Code", eventFiles[fileIndex].events[i_event].label->text());
        event.setAttribute("IsVisible", eventFiles[fileIndex].events[i_event].visibleCheckbox->isChecked());
        event.setAttribute("IsBrowsable", eventFiles[fileIndex].events[i_event].browsableCheckbox->isChecked());

        saveFile.appendChild(event);
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
 * When an event is set to be browsable, it has to be visible. This method will
 * set the visible checkbox as checked when the browsable checkbox is checked by
 * the user.
 *-------------------------
 * int file: the file index to which the checkbox is associated.
 * int event: the event index to which the checkbox is associated. It does not
 * matter whether it is the visible or browsable checkboxes as they works as
 * a pair anyway.
 ***************/
void EventWindow::changeVisibleCheckBoxState(int file, int event)
{
    if (eventFiles[file].events[event].browsableCheckbox->isChecked())
    {
        eventFiles[file].events[event].visibleCheckbox->setChecked(true);
    }
}


/***************
 * When an event is not set to be visible, it cannot be browsable. This method
 * will uncheck the browsable checkbox when the user uncheck the visible
 * checkbox.
 *-------------------------
 * int file: the file index to which the checkbox is associated.
 * int event: the event index to which the checkbox is associated. It does not
 * matter whether it is the visible or browsable checkboxes as they works as
 * a pair anyway.
 ***************/
void EventWindow::changeBrowsableCheckBoxState(int file, int event)
{
    if (!eventFiles[file].events[event].visibleCheckbox->isChecked())
    {
        eventFiles[file].events[event].browsableCheckbox->setChecked(false);
    }
}


/***************
 * Keep track of the lasts checkboxes interacted by the user. Used when the user
 * want to (de)select multiple checboxes at the same time using shift + click.
 *-------------------------
 * int fileID: the file to which is associated the targeted channel. Between 0
 * and (number of file - 1).
 * int eventID: the event to which is associated the checkbox. Between 0 and
 * (number of event - 1);
 * QString boxType: the type of the checkbox that have been interacted with. Can
 * be 0 (= Visible), 1 (= "Browsable") or 2 (= "Delete"). This won't work with
 * "Recode" because those box need some other user' actions.
 ***************/
void EventWindow::trackLastInteraction(int fileID, int eventID, int boxType)
{
    if (lastInteraction[0] == -1
            && lastInteraction[1] == -1
            && lastInteraction[2] == -1)
    {
        lastInteraction = {fileID, eventID, boxType};
    }

    else
    {
        secondToLastInteraction = lastInteraction;
        lastInteraction = {fileID, eventID, boxType};
    }


    // If they are in the same file [0],
    // and if they are different event [1],
    // and if they are of the same type [2],
    // then apply multiple changes.
    if (lastInteraction[0] == secondToLastInteraction[0]
            && lastInteraction[1] != secondToLastInteraction[1]
            && lastInteraction[2] == secondToLastInteraction[2])
    {
        checkboxMultipleStateChange();
    }
}


/***************
 * Called when the user wants to create a new event code. Create and display the
 * window to get its input. The user can only create a code that does not
 * already exist in the targeted file.
 *-------------------------
 * int fileID: the file to which the user wants to add a new event code.
 ***************/
void EventWindow::requestAddNewEvent(int fileID)
{
    // Get the existing code list fomr the corresponding file
    QVector<int> existingEventCodes;

    for (int i_event = 0; i_event < eventFiles[fileID].events.length(); i_event++)
    {
        existingEventCodes.append(eventFiles[fileID].events[i_event].label->text().toInt());
    }

    int newEventID =eventFiles[fileID].events.length();


    // Create and open the AddEventDialog window
    addEventDialog = new AddEventDialog(fileID, newEventID, existingEventCodes);

    connect(addEventDialog, SIGNAL(newEvent(int, int, int, bool, bool)),
            this, SLOT(addNewEvent(int, int, int, bool, bool)));

    addEventDialog->show();
}


/***************
 * Called after ther user gives its input on the new event code s-he wants to
 * create.
 *
 * TODO: adapt this method to create generic events, so that the duplicated code
 * in the constructor can be removed.
 *-------------------------
 * int fileID: the file to which the new event is associated.
 * int eventID: the ID of the new event (that is, it's position in the window).
 * int newEventCode: the code of the new event.
 * bool visibleState: either visible (true), or not (false).
 * bool browsableState: either browsable (true), or not (false).
 *
 * Note 1: An invisible event is not browsable.
 * Note 2: A browsable event is visible.
 ***************/
void EventWindow::addNewEvent(int fileID, int eventID, int newEventCode,
                              bool visibleState, bool browsableState,
                              bool isNewEvent)
{
    event newEvent;

    newEvent.isNewEvent = isNewEvent;


    // Event label
    //--------------------
    int code = newEventCode;
    newEvent.label = new QLabel(QString::number(code));
    setMinMaxSize(newEvent.label, labelSize, labelSize);
    newEvent.label->setFont(font);
    newEvent.label->setAlignment(Qt::AlignCenter);


    // Event visible checkbox
    //--------------------
    newEvent.visibleCheckbox = new CheckboxID(fileID, eventID, 0); // Visible checkboxes type have a type of 0
    newEvent.visibleCheckbox->setText("");
    setMinMaxSize(newEvent.visibleCheckbox, labelSize, labelSize);
    newEvent.visibleCheckbox->setChecked(visibleState);

    connect(newEvent.visibleCheckbox, SIGNAL(sendInfoShort(int, int)),
            this, SLOT(changeBrowsableCheckBoxState(int, int)));

    connect(newEvent.visibleCheckbox, SIGNAL(sendThreeID(int, int, int)),
            this, SLOT(trackLastInteraction(int, int, int)));


    // Event browsable checkbox
    //--------------------
    newEvent.browsableCheckbox = new CheckboxID(fileID, eventID, 1); // Browsable checkboxes type have a type of 1
    newEvent.browsableCheckbox->setText("");
    setMinMaxSize(newEvent.browsableCheckbox, labelSize, labelSize);
    newEvent.browsableCheckbox->setChecked(browsableState);

    connect(newEvent.browsableCheckbox, SIGNAL(sendInfoShort(int, int)),
            this, SLOT(changeVisibleCheckBoxState(int, int)));

    connect(newEvent.browsableCheckbox, SIGNAL(sendThreeID(int, int, int)),
            this, SLOT(trackLastInteraction(int, int, int)));


    // Event recode checkbox
    //--------------------
    newEvent.recodeCheckbox = new CheckboxID(fileID, eventID); // Recode chekboxes don't need a type here
    newEvent.recodeCheckbox->setText("");
    newEvent.recodeCheckbox->setFont(font);
    setMinMaxSize(newEvent.recodeCheckbox, labelSize, labelSize);

    connect(newEvent.recodeCheckbox, SIGNAL(sendInfoShort(int, int)),
            this, SLOT(recodeCheckboxChanged(int, int)));


    // Event delete checkbox
    //--------------------
    newEvent.deleteCheckbox = new CheckboxID(fileID, eventID, 2); // Delete checkboxes type have a type of 2
    newEvent.deleteCheckbox->setText("");
    setMinMaxSize(newEvent.deleteCheckbox, labelSize, labelSize);

    connect(newEvent.deleteCheckbox, SIGNAL(sendThreeID(int, int, int)),
            this, SLOT(trackLastInteraction(int, int, int)));


    // Event layout
    //--------------------
    QHBoxLayout* eventLayout = new QHBoxLayout;
    eventLayout ->addWidget(newEvent.label);
    eventLayout ->addWidget(newEvent.visibleCheckbox);
    eventLayout ->addWidget(newEvent.browsableCheckbox);
    eventLayout ->addWidget(newEvent.recodeCheckbox);
    eventLayout ->addWidget(newEvent.deleteCheckbox);
    eventLayout ->addStretch();


    // Add the new event to the list
    //--------------------
    eventFiles[fileID].events.append(newEvent);
    eventFiles[fileID].layout->insertLayout(eventFiles[fileID].layout->count() - 1, eventLayout);
}


/***************
 * This method will open a window when one of the "Recode" checkboxes is
 * CHECKED. The window will ask to the user which is the new code of the event
 * or if he wants to create a new one.
 *-------------------------
 * int fileID: the file ID to which the event is associated.
 * int eventID: the event to which the checkbox is associated.
 ***************/
void EventWindow::recodeCheckboxChanged(int fileID, int eventID)
{
    if (eventFiles[fileID].events[eventID].recodeCheckbox->isChecked())
    {
        // If the box is checked, open the window to get the user input.
        openRecodeEventWindow(fileID, eventID);
    }

    else
    {
        // Otherwise, just delete the text that was there
        eventFiles[fileID].events[eventID].recodeCheckbox->setText("");
    }
}


/***************
 * Create and display the window asking for the new event code.
 *-------------------------
 * int fileID: the file to which the event is associated.
 * int eventID: the target event.
***************/
void EventWindow::openRecodeEventWindow(int fileID, int eventID)
{
    // Get the original event code
    int originalEventCode = eventFiles[fileID].events[eventID].label->text().toInt();


    // Get the existing code list fomr the corresponding file
    QVector<int> existingEventCodes;

    for (int i_event = 0; i_event < eventFiles[fileID].events.length(); i_event++)
    {
        existingEventCodes.append(eventFiles[fileID].events[i_event].label->text().toInt());
    }


    // Create and open the RecodeEventDialog window
    recodeEventDialog = new RecodeEventDialog(originalEventCode, eventID, existingEventCodes, fileID);

    connect(recodeEventDialog, SIGNAL(sendNewCode(int, int, int)),
            this, SLOT(displayNewEventCode(int, int, int)));

    recodeEventDialog->show();
}


/***************
 * Simply display the new event code beside the corresponding "Recode" checkbox
 * after the user selected the new code.
 *-------------------------
 * int fileID: the file ID to which the event is associated.
 * int eventID: the event to which the checkbox is associated.
 * int newEventCode: the new event code to display. If the code is inferior or
 * equal to 0 (typically -1), this mean that the user didn't choose a new code,
 * cancelled the selection, or if there's a probelm somwhere.
 ***************/
void EventWindow::displayNewEventCode(int fileID, int eventID, int newEventCode)
{
    if (newEventCode > 0)
    {
        eventFiles[fileID].events[eventID].recodeCheckbox->setText(QString::number(newEventCode));
    }

    else
    {
        eventFiles[fileID].events[eventID].recodeCheckbox->setChecked(false);
    }
}


/***************
 * Apply the state of all checkboxes of the specified event, from the specified
 * file, to all other events sharing the same code accross all other files.
 *-------------------------
 * int fileID: the file ID to which the event is associated.
 ***************/
void EventWindow::applyToAllFile(int fileID)
{
    // For each event files
    for (int i_file = 0; i_file < eventFiles.length(); i_file++)
    {
        // That are not the one taken as a reference (from which informations are taken)
        if (i_file != fileID)
        {
            // For each of the reference event
            for (int j_ref = 0; j_ref < eventFiles[fileID].events.length(); j_ref++)
            {
                // For each of the event in the file being altered
                for (int k_event = 0; k_event < eventFiles[i_file].events.length(); k_event++)
                {
                    // If the events correspond
                    if (eventFiles[fileID].events[j_ref].label->text() == eventFiles[i_file].events[k_event].label->text())
                    {qDebug()<<"it's a match!";
                        // Visible checkbox
                        eventFiles[i_file].events[k_event].visibleCheckbox->setChecked(eventFiles[fileID].events[j_ref].visibleCheckbox->isChecked());

                        // Browsable checkbox
                        eventFiles[i_file].events[k_event].browsableCheckbox->setChecked(eventFiles[fileID].events[j_ref].browsableCheckbox->isChecked());

                        // Recode checkbox
                        if (eventFiles[fileID].events[j_ref].recodeCheckbox->isChecked())
                        {
                            eventFiles[i_file].events[k_event].recodeCheckbox->setChecked(true);
                            eventFiles[i_file].events[k_event].recodeCheckbox->setText(eventFiles[fileID].events[j_ref].recodeCheckbox->text());
                        }

                        // Delete checkbox
                        eventFiles[i_file].events[k_event].deleteCheckbox->setChecked(eventFiles[fileID].events[j_ref].deleteCheckbox->isChecked());
                    }
                }
            }
        }
    }
}


//      =====================
//      %%%% Load / Save %%%%
//      =====================


/***************
 * The user can load a particular event configuration, that is, the list of code
 * with their associated visible and browsable states.
 ***************/
void EventWindow::on_loadPresetButton_clicked()
{
    // Get the target file index
    //--------------------
    QString message = "Choose the file that will receive the modifications (can be all of them):";

    QVector<QString> filenameList;

    for (int i_file = 0; i_file < eventFiles.length(); i_file++)
    {
        filenameList.append(eventFiles[i_file].filename);
    }

    QVector<QString> fileList = filenameList;
    fileList.insert(0, "All File");

    chooseFile = new ChooseFileWindow(fileList, message);

    connect(chooseFile, SIGNAL(sendFileIndex(int)),
            this, SLOT(requestLoad(int)));

    chooseFile->show();
}


/***************
 * The user can save a particular event configuration, that is, the list of code
 * with their associated visible and browsable states.
 ***************/
void EventWindow::on_savePresetButton_clicked()
{
    // Get the target file index
    //--------------------
    QString message = "Choose which file to save the channel configuration of:";

    QVector<QString> filenameList;

    for (int i_file = 0; i_file < eventFiles.length(); i_file++)
    {
        filenameList.append(eventFiles[i_file].filename);
    }

    chooseFile = new ChooseFileWindow(filenameList, message);

    connect(chooseFile, SIGNAL(sendFileIndex(int)),
            this, SLOT(requestSave(int)));

    chooseFile->show();
}


/***************
 * Ask the user which file to laod, then call the loading method.
 *-------------------------
 * int fileIndex: the data file index target of the channel configuration save..
 ***************/
void EventWindow::requestLoad(int fileIndex)
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
void EventWindow::requestSave(int fileIndex)
{
    // Get the save file name
    //--------------------
    QFileDialog nameDialog;
    QString filename = nameDialog.getSaveFileName(this, tr("Choose save location"),
                                                  QDir::currentPath() + "/untitled.xml",
                                                  tr(".xml"));

    xmlWrite(filename, fileIndex);
}


/***************
 * All the changes of the user are applied to LOADED DATA (not the files!).
 * Before the changes are applied, a dialog window will ask the user if he
 * really wants to do it. There's currently no way to undo the changes once
 * applied, unless the user either undo them her-himself (by coming back here,
 * and even there some changes are irreversible, like the delete option), or
 * unload/reload the .pos file without overwriting it.
 *
 * In order to save the files with those modification, the user must click on
 * the "Save Event File" menu, in the MainWindow. In this case, the user will
 * have the choice to either overwrite the currently loaded .pos file, or create
 * a new one.
 *
 * Alternatively, he can choose to save the event configuration by clicking on
 * the "Save Configurration" button in this very window. This will save the code
 * and state of the event, which can be applied on future files (or rather data)
 * with the "Load Configuration" button.
 ***************/
void EventWindow::on_applyButton_clicked()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Warning: changes are irreversible");
    msgBox.setText("If you click on 'Apply' again, all the changes you have "
                   "made will be applied. Note that once they are done, "
                   "those changes are irreversible (unless you reload the "
                   "original .pos file).\n");
    msgBox.setInformativeText("Are you sure you want to apply the changes?");
    msgBox.setStandardButtons(QMessageBox::Apply | QMessageBox::Cancel);
    int ret = msgBox.exec();

    if (ret == QMessageBox::Apply)
    {
        applyChanges();

        this->~EventWindow();
    }

    else if (ret == QMessageBox::Cancel)
    {
        // Return to Event Window
        msgBox.close();
    }
}


/***************
 * Close the window, discarding any changes if they weren't applied.
 ***************/
void EventWindow::on_closeButton_clicked()
{
    this->~EventWindow();
}
