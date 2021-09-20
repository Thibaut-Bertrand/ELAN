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
 * Create and manage the main window of the application, as well as every other
 * sub-windows (seting window...). The central widget contain a wrapper that
 * manage multiple layers for the display of data.
 *
 * Author: Thibaut Bertrand
 * thibaut.bertrand@inserm.fr
 ***************/


#include "MainWindow.h"
#include "ui_MainWindow.h"


/***************
 * Constructor
 ***************/
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    // Event filtering to catch arrow key intput
    //--------------------
    installEventFilter(this);

    this->ui->dockWidget_2->installEventFilter(this);

    setFocus();


    //==============================
    // Creation of the data container
    //==============================


    projectData = new ProjectData();


    connect(this->projectData, SIGNAL(sendCursorTimes(int, int)),
            this, SLOT(dockShowDataCursor(int, int)));

    connect(this->projectData->pageDataContainer, SIGNAL(notifyChange(size_t)),
            this, SLOT(onPageDataChanged(size_t)));

    connect(this->projectData->plotDataContainer, SIGNAL(notifyChange(size_t)),
            this, SLOT(onPlotDataChanged(size_t)));

    connect(this->projectData, SIGNAL(notifyChange(size_t)),
            this, SLOT(onProjectDataChanged(size_t)));


    // WRAPPER CREATION
    setWrapper();


    //==============================
    // Creation of the layout displaying event codes on top of the plots
    //==============================

    setEventLabels();


    //==============================
    // Creation of the layout displaying channels labels on the left of the plots
    //==============================
    setChannelLabels();


    //==============================
    // Project manager
    //==============================
    projectManager = new ProjectManager(projectData);


    //==============================
    // UI
    //==============================
    ui->actualTimeScaleButton->setText(QString::number(projectData->pageDataContainer->timeScale));
    ui->actualPageButton->setText(QString::number(0));
}


/***************
 * Destructor
 ***************/
MainWindow::~MainWindow()
{
    delete ui;
}


//      %%%%%%%%%%%%%%%%%%%%%%%%%%
//      %%%% METHODS override %%%%
//      %%%%%%%%%%%%%%%%%%%%%%%%%%


/***************
 * Event filter used to catch arrow keys inputs (page changing...).
 * cf Qt documentation.
 ***************/
bool MainWindow::eventFilter(QObject* obj, QEvent* e)
{
    QKeyEvent *keyEvent = NULL; // Event data, if this is a keystroke event
    bool result = false; // Return true to consume the keystroke

    if (e->type() == QEvent::KeyPress)
    {
         keyEvent = dynamic_cast<QKeyEvent*>(e);

         this->keyPressEvent(keyEvent);

         result = true;
    }

    else if (e->type() == QEvent::KeyRelease)
    {
        keyEvent = dynamic_cast<QKeyEvent*>(e);

        this->keyReleaseEvent(keyEvent);

        result = true;
    }

    // Standard event processing
    else
    {
        result = QObject::eventFilter(obj, e);
    }

    return result;
}


/***************
 * Catch Keyboard inputs.
 * Allow Page changes with the Q and D keys.
 * cf Qt documentation.
 ***************/
void MainWindow::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Q
            || e->key() == Qt::Key_D
            || e->key() == Qt::Key_Left
            || e->key() == Qt::Key_Right)
    {
        emit pageChange(e); // -> Wrapper -> CentralMiddleLayerManager
        projectData->pageDataContainer->pageChange(e);
    }


    // Plot info
    if (e->key() == Qt::Key_Control)
    {
        projectData->presentationDataContainer->isCtrlPressed = true;
    }


    // Butterfly
    if (e->key() == Qt::Key_B)
    {
        if (this->ui->butterflyCheckbox->isChecked())
        {
            this->ui->butterflyCheckbox->setChecked(false);
        }

        else
        {
            this->ui->butterflyCheckbox->setChecked(true);
        }
    }


    // Signal centering
    if (e->key() == Qt::Key_C)
    {
        if (this->ui->actionSignalCentering->isEnabled())
        {
            if (this->ui->actionSignalCentering->isChecked())
            {
                this->ui->actionSignalCentering->setChecked(false);
            }

            else
            {
                this->ui->actionSignalCentering->setChecked(true);
            }

            on_actionSignalCentering_triggered();
        }
    }
}


/***************
 * cf Qt documentation.
 ***************/
void MainWindow::keyReleaseEvent(QKeyEvent *e)
{

    // Plot info
    if (e->key() == Qt::Key_Control)
    {
        projectData->presentationDataContainer->isCtrlPressed = false;
    }
}


//      %%%%%%%%%%%%%%%%%%%%%%%%%
//      %%%% METHODS private %%%%
//      %%%%%%%%%%%%%%%%%%%%%%%%%


void MainWindow::clearLayout(QLayout* layout)
{
    if (layout == NULL)
    {
        return;
    }


    QLayoutItem* item;

    while((item = layout->takeAt(0)))
    {
        if (item->layout())
        {
            clearLayout(item->layout());
            item->layout()->deleteLater();
            /* DO NOT USE "delete item->layout();". This will try to delete
             * NULL and that's a nope. That's because there's some weird
             * behavior when deleting QLayouts. Migh be the case for any
             * QWidgets too (but in that case it's okay to just use
             * "delete item->widget();" */
        }

        if (item->widget())
        {
            delete item->widget();
        }

        delete item;
    }
}


/***************
 * Initialize the Wrapper, which will contain the main display components:
 * - CentralBottomLayer: backgound colors and various graduation,
 * - CentralMiddleLayerManager, which contain the CentralMiddleLayerDisplay
 * (display the plot data), and the CentralMiddleLayerEvents (display the
 * events).
 * - CentralTopLayer, which contain the EventCatcherTopLayer (display the
 * cursors and catch mouse events from the user on the central widget).
 ***************/
void MainWindow::setWrapper()
{
    // The wrapper contain the layers displaying the data and allow their communication
    wrapper = new Wrapper(ui->plotFrame->size(), projectData);


    //==============================
    // Connection hub
    //==============================

    // SIGNALs coming from ui
    //--------------------

    // Cursors
    //----------
    connect(this, SIGNAL(cursorStateChange()),
            this->wrapper, SIGNAL(forwardCursorStateChange()));

    connect(this, SIGNAL(cursorIntervalInput(int, int)),
            this->wrapper, SIGNAL(forwardCursorIntervalInput(int, int)));


    // Events
    //----------
    connect(this, SIGNAL(sendEventFileName(QString)),
            this->wrapper, SIGNAL(forwardEventFileName(QString)));


    // Files
    //----------
    connect(this, SIGNAL(sendFileName(QString)),
            this->wrapper, SIGNAL(forwardFileName(QString)));


    // Pages
    //----------
    connect(this, SIGNAL(pageToDisplay(int)),
            this->wrapper, SIGNAL(forwardPageToDisplay(int)));

    connect(this, SIGNAL(pageChange(QString)),
            this->wrapper, SIGNAL(forwardPageChange(QString)));

    connect(this, SIGNAL(pageChange(QKeyEvent*)),
            this->wrapper, SIGNAL(forwardPageChange(QKeyEvent*)));


    // SIGNALs coming from the Wrapper
    //--------------------
    connect(this->wrapper, SIGNAL(forwardKeyboardReleased()),
            this, SLOT(getKeyboard()));


    ui->plotLayout->addWidget(wrapper);
}


/***************
 * Initialize the EventLabels, which contain the EventLabelsDisplayer (display
 * the event labels on the top side of the central widget).
 ***************/
void MainWindow::setEventLabels()
{
    eventLabels = new EventLabels(ui->plotFrame->size(), projectData);

    ui->eventLabelLayout->addWidget(eventLabels);
}


/***************
 * Initialize the ChannelLabels, which contain the ChannelLabelsDisplayer
 * (display the channel labels on the left side of the central widget).
 ***************/
void MainWindow::setChannelLabels()
{
    channelLabels = new ChannelLabels(ui->plotFrame->size(), projectData);

    ui->channelLabelLayout->addWidget(channelLabels);
}


/***************
 * When data files (.eeg...) are (un)loaded, this method will update the
 * scrollArea (bottom of the left Dock Widget), so that each loaded file name is
 * associated with it's specific color. And so the user will have an easier time
 * to tell which plot is from which file.
 ***************/
void MainWindow::setFileNamesLegend()
{
    // Clear loadedFilesVLayout
    // ----------
    clearLayout(this->ui->loadedFilesVLayout);

    // Populate loadedFilesVLayout
    // ----------
    for (int i = 0; i < this->projectData->plotDataContainer->eegFileNb; i++)
    {
        qDebug()<<this->projectData->plotDataContainer->eegDataStorage[i]->eegFilename;

        QHBoxLayout* fileLayout = new QHBoxLayout;


        // --- Small square with the color associated with the file
        QWidget* colorWidget = new QWidget;
        colorWidget->setMaximumSize(10, 10);
        colorWidget->setMinimumSize(10, 10);

        QString styleSheet = "background-color:";

        if (this->projectData->plotDataContainer->eegFileNb == 1)
        {
            styleSheet += "black";
        }

        else
        {
            styleSheet += this->projectData->presentationDataContainer->plotColors[i].name(QColor::HexArgb);
        }

        styleSheet += "; border:1px solid black";

        colorWidget->setStyleSheet(styleSheet);


        // --- File name
        QString fileName = projectData->plotDataContainer->eegDataStorage[i]->eegFilename.mid(projectData->plotDataContainer->eegDataStorage[i]->eegFilename.lastIndexOf("/") + 1);

        QLabel* nameLabel = new QLabel(fileName);

        fileLayout->addWidget(colorWidget);
        fileLayout->addWidget(nameLabel);

        fileLayout->addStretch();

        this->ui->loadedFilesVLayout->addLayout(fileLayout);
    }

    this->ui->loadedFilesVLayout->addStretch();
}


//      %%%%%%%%%%%%%%%%%%%%%%%%
//      %%%% METHODS public %%%%
//      %%%%%%%%%%%%%%%%%%%%%%%%


/***************
 * Check if the header file (.ent) of the specified .eeg file exist (true) or
 * not (false).
 *-------------------------
 * QString filename: .eeg filename, selected by the user when opening it.
 ***************/
bool MainWindow::checkHeaderFile(QString filename)
{
    if (filename.endsWith("eeg"))
    {
        QString headerFilename = filename + ".ent";
        QFile headerFile(headerFilename);

        return headerFile.exists();
    }

    else if (filename.endsWith("h5") || filename.endsWith("p"))
    {
        return true;
    }

    else
    {
        return false;
    }
}


/***************
 * If the header file is not in the same directory, display an error message.
 ***************/
void MainWindow::headerFileNotFoundWarning()
{
    QMessageBox msgBox;

    msgBox.setText("Header file not found.");
    msgBox.setInformativeText("Please, make sure the header file (.ent) of the selected .eeg file is present in the same folder.");
    msgBox.setIcon(QMessageBox::Warning);

    msgBox.exec();
}


//      ================================
//      %%%% Locking / Unlocking UI %%%%
//      ================================


/***************
 * When the user load a the first data file, some buttons and other options have
 * to be unlocked. Inversely when s-he load the first date file.
 ***************/
void MainWindow::eegEnabler(bool areEnabled)
{
    eegFileOpen = areEnabled;


    // Actions
    this->ui->actionButterfly->setEnabled(areEnabled); // Create butterfly plots
    this->ui->actionLoadEventFile->setEnabled(areEnabled); // Allow to load event file
    this->ui->actionSaveProject->setEnabled(areEnabled); // Save the project
    this->ui->actionSelect->setEnabled(areEnabled); // Select the channel states
    this->ui->actionSignalCentering->setEnabled(areEnabled); // Center the signals
    this->ui->actionUnloadFiles->setEnabled(areEnabled); // Unlod data file(s)


}


/***************
 * When the user load a the first event file, some buttons and other options have
 * to be unlocked. Inversely when s-he load the first event file.
 ***************/
void MainWindow::eventEnabler(bool areEnabled)
{
    // Actions
    this->ui->actionAlignFiles->setEnabled(areEnabled); // Align data files with events
    this->ui->actionManageDisplayedFiles->setEnabled(areEnabled); // Select which event files are displayed or not
    this->ui->actionManageEvent->setEnabled(areEnabled); // Manage events states, codes...
    this->ui->actionSaveEventFile->setEnabled(areEnabled); // Save a .pos file (either replace an existing one, or create a new one)
    this->ui->actionUnloadEventFiles->setEnabled(areEnabled); // Unload event file(s)


    // Buttons
    this->ui->addEventButton->setEnabled(areEnabled); // Add event on one of the cursor
    this->ui->eventModeButton->setEnabled(areEnabled); // Toggle the event focused view mode
}


//      %%%%%%%%%%%%%%%
//      %%%% SLOTS %%%%
//      %%%%%%%%%%%%%%%


//      =================
//      %%%% Cursors %%%%
//      =================


/***************
 * Show the data, time and sample, corresponding to the cursors.
 *-------------------------
 * int cursorOne: the time of the first cursor. If -1, then it's not displayed.
 * int cursorTwo: the time of the second cursor. If -1, then it's not displayed.
 ***************/
void MainWindow::dockShowDataCursor(int cursorOne, int cursorTwo)
{
    cursorOne == -1 ? this->ui->cursorOneTimeLabel->setText("-") :
                      this->ui->cursorOneTimeLabel->setText(QString::number(cursorOne));

    cursorTwo == -1 ? this->ui->cursorTwoTimeLabel->setText("-") :
                      this->ui->cursorTwoTimeLabel->setText(QString::number(cursorTwo));

    if (cursorOne >= 0 && cursorTwo >= 0)
    {
        dockShowIntervalData(cursorTwo - cursorOne);
    }

    else
    {
        dockShowIntervalData(-1);
    }
}


/***************
 * Show the time interval of the cursors on the side of the MainWindow.
 *-------------------------
 * int time = the time interval btween the two cursors.
 * int sample = the sample interval between the two cursors.
 ***************/
void MainWindow::dockShowIntervalData(int time)
{
    if (time == -1)
    {
        ui->intervalTimeLabel->setText("-");
    }

    else
    {
        ui->intervalTimeLabel->setText(QString::number(time));
    }
}


//      ==================
//      %%%% Keyboard %%%%
//      ==================


/***************
 * Capture the keyboard so that it's not used by another widget (by default, Qt
 * give the keyboard to the DockWidget).
 ***************/
void MainWindow::getKeyboard()
{
    this->grabKeyboard();
}


//      =========================
//      %%%% Loading Project %%%%
//      =========================


/***************
 * When loading a .eeg file through a project, this update the objects that
 * needs to be updated.
 ***************/
void MainWindow::eegLoaded()
{
    eegEnabler(true);
}


/***************
 * When loading a .pos file through a project, this update the objects that
 * needs to be updated.
 ***************/
void MainWindow::posLoaded()
{
    eventEnabler(true);
}


//      ===============
//      %%%% Modes %%%%
//      ===============


//      ===============
//      %%%% Pages %%%%
//      ===============


/***************
 * Display the sample number of the selected reference (either the first
 * sample of the page or the middle one).
 ************************************************************************
 * ########################### Obsolote because multiple file != sample freq
 ***************/
void MainWindow::dockShowReferenceTime()
{
//    this->ui->referenceTimeLabel->setText(QString::number(pageManager->referenceSample));
}


/***************
 ***************/
void MainWindow::updatePageNb()
{
    this->ui->pageTotalLabel->setText(QString::number(projectData->pageDataContainer->totalPageNb));
    this->ui->actualPageButton->setText(QString::number(projectData->pageDataContainer->actualPage));
}


//      %%%%%%%%%%%%%%%%%%
//      %%%% UI SLOTS %%%%
//      %%%%%%%%%%%%%%%%%%


//      ==================
//      %%%% Channels %%%%
//      ==================


/***************
 * Just send a signal to open the channel display state selection window.
 ***************/
void MainWindow::on_actionSelect_triggered()
{
    projectData->openSelectChannelWindow();
}


//      =================
//      %%%% Cursors %%%%
//      =================


/***************
 * Switch to cursor double click mode.
 ***************/
void MainWindow::on_clickModeButton_clicked()
{
    this->ui->clickModeButton->setStyleSheet("font: bold;");
    this->ui->dragModeButton->setStyleSheet("font: normal;");
    this->ui->cursorValueButton->setEnabled(true);

    emit cursorStateChange(); // -> Wrapper -> CentralTopLayer
}


/***************
 * Set the cursor to the positiion specified by the user.
 ***************/
void MainWindow::on_cursorValueButton_clicked()
{
    projectData->openSetCursorWindow();
}


/***************
 * Switch to cursor drag mode.
 ***************/
void MainWindow::on_dragModeButton_clicked()
{
    this->ui->dragModeButton->setStyleSheet("font: bold;");
    this->ui->clickModeButton->setStyleSheet("font: normal;");
    this->ui->cursorValueButton->setEnabled(false);

    emit cursorStateChange(); // -> Wrapper -> CentralTopLayer -> EventCatcherTopLayer
}


/***************
 * Allow the user to select which data file to focus for the amplitude. The
 * amplitude values are shown in the dock widget (mean of all channel at cursor
 * position in dock widget, and value at first cursor position in the
 * ChannelLablDisplayer).
 ***************/
void MainWindow::on_fileAmplitudeButton_clicked()
{
    projectData->selectFileForAmplitude();
}


/***************
 * Reset the cursors position (set their position at 0 and hide them).
 ***************/
void MainWindow::on_resetCursorButton_clicked()
{
    projectData->presentationDataContainer->hideCursors();
}


//      ==============
//      %%%% Data %%%%
//      ==============


/***************
 * Take the actually displayed data on the CentralLayer, and ask the user which
 * channel to take (all or some of them). Then, put the selected channels into a
 * butterfly.
 * TODO: automatic selection for some pattern (all left/right channels, all
 * EEG/MEG...
 ***************/
void MainWindow::on_actionButterfly_triggered()
{
    projectData->openButterflyCreationWindow();
}


/***************
 * Center the signal by substracting the average value of the data to each
 * point, for each channel independently.
 ***************/
void MainWindow::on_actionSignalCentering_triggered()
{
    projectData->changeSignalCentering(this->ui->actionSignalCentering->isChecked());
}


//      ==============
//      %%%% Dock %%%%
//      ==============


/***************
* Enable/disable plot origin display by the CentralBottomLayer.
 ***************/
void MainWindow::on_plotOriginCheckbox_clicked()
{
    projectData->presentationDataContainer->setAxesState(this->ui->plotOriginCheckbox->isChecked());
}


/***************
 * Enable/disable the butterfly plot displaying.
 *-------------------------
 * int arg1 = check state (0 = unchecked and 2 = checked).
 ***************/
void MainWindow::on_butterflyCheckbox_stateChanged(int arg1)
{
    this->ui->butterflyCheckbox->isChecked() ?
                projectData->plotDataContainer->changeButterflyDisplay(true) :
                projectData->plotDataContainer->changeButterflyDisplay(false);
}


/***************
 * Enable/disable the butterfly plot displaying.
 ***************/
void MainWindow::on_cursorIntervalCheckbox_clicked()
{
    projectData->presentationDataContainer->changeCursorIntervalState();
}


/***************
 * Enable/disable the ticks displaying (in the event displayer, on the top side
 * of the central widget).
 ***************/
void MainWindow::on_ticksCheckbox_clicked()
{
    projectData->presentationDataContainer->changeTicksState();
}


//      ================
//      %%%% Events %%%%
//      ================


/***************
 * Open a window to select the event file.
 ***************/
void MainWindow::on_actionLoadEventFile_triggered()
{
    QString fileName;

    fileName = QFileDialog::getOpenFileName(this, "Choose a file", QDir::currentPath(), "pos(*.pos)");

    if (!fileName.endsWith("pos") && !fileName.isEmpty())
    {
        QMessageBox wrongFileMsgBox;
        wrongFileMsgBox.setText("Please choose a correct file with a .pos extension.");
        wrongFileMsgBox.exec();
    }

    else
    {
        if (fileName != "")
        {
            eventEnabler(true);

            projectData->selectEventFileAssociation(fileName);
        }
    }
}


/***************
 * Send the signal to open the event manager.
 ***************/
void MainWindow::on_actionManageEvent_triggered()
{
    this->releaseKeyboard();

    projectData->openEventWindow();
}


/***************
 * Simply request the saving of an event (.pos) file.
 ***************/
void MainWindow::on_actionSaveEventFile_triggered()
{
    projectData->requestPosFileSaving();
}


/***************
 * Request the window that manage which event file are displayed for each .eeg
 * file. Only one .pos file can be active for each .eeg file.
 ***************/
void MainWindow::on_actionManageDisplayedFiles_triggered()
{
    projectData->openActiveEventFileWindow();
}


/***************
 * Request the window that manage which event file to unload
 ***************/
void MainWindow::on_actionUnloadEventFiles_triggered()
{
    projectData->openUnloadEventFileWindow();
}


/***************
 * Change the state of the event to be accepted.
 ***************/
void MainWindow::on_acceptEventButton_clicked()
{
    projectData->changeCurrentEventState(true);
}


/***************
 * Add an event on the position of the desired cursor (the first or the  second)
 ***************/
void MainWindow::on_addEventButton_clicked()
{
    projectData->addEventOnCursor();
}


/***************
 * Recode the currently focused event.
 ***************/
void MainWindow::on_recodeEventButton_clicked()
{
    projectData->changeCurrentEventCode();
}


/***************
 * Change the state of teh event to be rejected
 ***************/
void MainWindow::on_rejectEventButton_clicked()
{
    projectData->changeCurrentEventState(false);
}



//      ===============
//      %%%% Files %%%%
//      ===============


/***************
 * Open the window to align multiple files on specified reference (an event).
 ***************/
void MainWindow::on_actionAlignFiles_triggered()
{
    projectData->openSelectAlignementWindow();
}


/***************
 * Select a file, gets its name and send it to the CentralMidlleLayerManager.
 ***************/
void MainWindow::on_actionLoadFile_triggered()
{
    if (!eegFileOpen)
    {
        QString fileName = QFileDialog::getOpenFileName(this, "Choose a file",
                                                        QDir::currentPath(),
                                                        "EEG (*.eeg);;HDF5 (*.h5)");

        if (fileName != "")
        {
            if (checkHeaderFile(fileName))
            {
                eegFileOpen = true;

                eegEnabler(true);

                projectData->eegFileLoading(fileName);
            }

            else
            {
                // Warning
                headerFileNotFoundWarning();
            }
        }
    }

    else
    {
        // TODO:
        // Ask if the user wants to close the actual file or load 2 or more files at the same time

        // Load the file
        QString fileName = QFileDialog::getOpenFileName(this, "Choose a file", QDir::currentPath(), "eeg(*.eeg)");

        if (fileName != "")
        {
            if (checkHeaderFile(fileName))
            {
                projectData->eegFileLoading(fileName);
            }

            else
            {
                // Warning
                headerFileNotFoundWarning();
            }
        }
    }
}


/***************
 * Called when the user want to close one of the loaded .eeg file.
 ***************/
void MainWindow::on_actionUnloadFiles_triggered()
{
    projectData->openUnloadEegFileWindow();
}


/***************
 * Allow the user to set the working directory for its project. By default, the
 * current directory is the directory from where the application was launched.
 ***************/
void MainWindow::on_actionSet_Current_Dir_triggered()
{
    QString path = QFileDialog::getExistingDirectory(this,
                                                     "Choose your working folder",
                                                     QDir::currentPath(),
                                                     QFileDialog::ShowDirsOnly);

    if (path != "")
    {
        QDir::setCurrent(path);
    }

    projectData->projectCurrentDirectory = path;
}


//      ===============
//      %%%% Modes %%%%
//      ===============


/***************
 * Update the objects that need it when the user switch to the event view mode.
 ***************/
void MainWindow::on_eventModeButton_clicked()
{
    this->ui->pageModeButton->setStyleSheet("font: normal;");
    this->ui->eventModeButton->setStyleSheet("font: bold;");

    this->ui->acceptEventButton->setEnabled(true);
    this->ui->rejectEventButton->setEnabled(true);
    this->ui->recodeEventButton->setEnabled(true);

    this->ui->actionPageMode->setChecked(false);
    this->ui->actionNextPage->setEnabled(false);
    this->ui->actionPreviousPage->setEnabled(false);
    this->ui->actionEventMode->setChecked(true);
    this->ui->actionNextEvent->setEnabled(true);
    this->ui->actionPreviousEvent->setEnabled(true);

    this->ui->pageLabel->setText("Event");

    projectData->setEventFocusedViewMode();
}


/***************
 * Update the objects that need it when the user switch to the page view mode.
 ***************/
void MainWindow::on_pageModeButton_clicked()
{
    this->ui->pageModeButton->setStyleSheet("font: bold;");
    this->ui->eventModeButton->setStyleSheet("font: normal;");

    this->ui->acceptEventButton->setEnabled(false);
    this->ui->rejectEventButton->setEnabled(false);
    this->ui->recodeEventButton->setEnabled(false);

    this->ui->actionPageMode->setChecked(true);
    this->ui->actionNextPage->setEnabled(true);
    this->ui->actionPreviousPage->setEnabled(true);
    this->ui->actionEventMode->setChecked(false);
    this->ui->actionNextEvent->setEnabled(false);
    this->ui->actionPreviousEvent->setEnabled(false);

    this->ui->pageLabel->setText("Page");

    projectData->setClassicViewMode();
}


/***************
 * Set the view to the event mode.
 ***************/
void MainWindow::on_actionEventMode_triggered()
{
    if (this->ui->actionPageMode->isChecked())
    {
        this->on_eventModeButton_clicked();
    }

    else
    {
        this->on_pageModeButton_clicked();
    }
}


/***************
 * Set the view to the page mode.
 ***************/
void MainWindow::on_actionPageMode_triggered()
{
    if (this->ui->actionPageMode->isChecked())
    {
        this->on_pageModeButton_clicked();
    }

    else
    {
        this->on_eventModeButton_clicked();
    }
}

//      ===============
//      %%%% Pages %%%%
//      ===============


/***************
 * Manage the display and change of the actual page.
 ***************/
void MainWindow::on_actualPageButton_clicked()
{
    projectData->goToPage();
}


/***************
 * Next page.
 ***************/
void MainWindow::on_nextPageButton_clicked()
{

    QKeyEvent* event = new QKeyEvent(QEvent::KeyPress, Qt::Key_D,
                                     Qt::NoModifier);

    this->keyPressEvent(event);
}


/***************
 * Previous page.
 ***************/
void MainWindow::on_previousPageButton_clicked()
{
    QKeyEvent* event = new QKeyEvent(QEvent::KeyPress, Qt::Key_Q,
                                     Qt::NoModifier);

    this->keyPressEvent(event);
}


//      ================
//      %%%% Scales %%%%
//      ================


/***************
 * Request the opening of the scale window.
 ***************/
void MainWindow::on_actionScales_triggered()
{
    projectData->openScaleWindow();
}


/***************
 * Request the opening of the scale window.
 ***************/
void MainWindow::on_actualTimeScaleButton_clicked()
{
    projectData->openScaleWindow();
}


/***************
 * Set the time scale to its default value.
 ***************/
void MainWindow::on_defaultTimeScaleButton_clicked()
{
    projectData->pageDataContainer->setDefaultTimeScale();
}


/***************
 * Decrease time scale.
 ***************/
void MainWindow::on_lowerTimeButton_clicked()
{
    projectData->pageDataContainer->setTimeScale(projectData->pageDataContainer->timeScale - 100);
}


/***************
 * Increase time scale.
 ***************/
void MainWindow::on_upperTimeButton_clicked()
{
    projectData->pageDataContainer->setTimeScale(projectData->pageDataContainer->timeScale + 100);
}


//      ================
//      %%%% Others %%%%
//      ================


/***************
 * Display the documentation of the application, or a link to it.
 ***************/
void MainWindow::on_actionDocumentation_triggered()
{

}


/***************
 * Generate and open the settings window.
 ***************/
void MainWindow::on_actionPreferences_triggered()
{
    settingWindow = new SettingWindow(projectData->presentationDataContainer,
                                      this);

    settingWindow->show();
}


/***************
 * Quit application
 ***************/
void MainWindow::on_actionQuit_triggered()
{
    qApp->quit();
}


//      =========================
//      %%%% Project Manager %%%%
//      =========================


/***************
 * Generate and open the loading project window, after getting the project file
 * from the user selection.
 ***************/
void MainWindow::on_actionLoadProject_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this,"Choose a file",
                                                    QDir::currentPath(),
                                                    "xml(*.xml)");

    if (filename != "")
    {
        projectManager->LoadProject(filename);
    }
}


/***************
 * Request project saving.
 ***************/
void MainWindow::on_actionSaveProject_triggered()
{
    projectManager->SaveProject();
}


//      =============
//      %%%% HUB %%%%
//      =============


/***************
 * Receive info from the data container and filter the hints to process only the
 * one of interest
 *-------------------------
 * size_t hint: flag that indicate specific changes in the data container. Allow
 * the method to know if it should take the signal into account or not.
 ***************/
void MainWindow::onPageDataChanged(size_t hint)
{
    if (hint & PageData::eHintPageNumberChanged
            || hint & PageData::eHintPageChanged)
    {
        updatePageNb();

        this->ui->referenceTimeLabel->setText(QString::number(projectData->pageDataContainer->timeAtFirstSample));

        projectData->getTimeAtCursor();
    }


    else if (hint & PageData::eHintTimeScaleChanged)
    {
        this->ui->actualTimeScaleButton->setText(QString::number(projectData->pageDataContainer->timeScale));

        this->ui->referenceTimeLabel->setText(QString::number(projectData->pageDataContainer->timeAtFirstSample));

        projectData->getTimeAtCursor();
    }
}


/***************
 * Receive info from the data container and filter the hints to process only the
 * one of interest
 *-------------------------
 * size_t hint: flag that indicate specific changes in the data container. Allow
 * the method to know if it should take the signal into account or not.
 ***************/
void MainWindow::onPlotDataChanged(size_t hint)
{
    if (hint & PlotData::eHintButterflyDisplayChanged)
    {
        projectData->plotDataContainer->areBtfDipslayed ?
                    this->ui->butterflyCheckbox->setChecked(true) :
                    this->ui->butterflyCheckbox->setChecked(false);
    }

    if (hint & PlotData::eHintLoadedEventFileChanged)
    {
        if (projectData->plotDataContainer->areEventsLoaded())
        {
            eventEnabler(true);
        }
    }

    if (hint & PlotData::eHintAmpliAtChannelChanged)
    {
        double meanOne = 0.;
        double meanTwo = 0.;

        if (!projectData->plotDataContainer->amplitudesAtFirstCursor.isEmpty())
        {
            double sum = 0.;

            for (int i_ampli = 0; i_ampli < projectData->plotDataContainer->amplitudesAtFirstCursor.length(); i_ampli++)
            {
                sum += projectData->plotDataContainer->amplitudesAtFirstCursor[i_ampli];
            }

            meanOne = sum / projectData->plotDataContainer->amplitudesAtFirstCursor.length();

            this->ui->cursorOneAmplitudeLabel->setText(QString::number(meanOne));
        }

        else
        {
            this->ui->cursorOneAmplitudeLabel->setText("-");
        }


        if (!projectData->plotDataContainer->amplitudesAtSecondCursor.isEmpty())
        {
            double sum = 0.;

            for (int i_ampli = 0; i_ampli < projectData->plotDataContainer->amplitudesAtSecondCursor.length(); i_ampli++)
            {
                sum += projectData->plotDataContainer->amplitudesAtSecondCursor[i_ampli];
            }

            meanTwo = sum / projectData->plotDataContainer->amplitudesAtSecondCursor.length();

            this->ui->cursorTwoAmplitudeLabel->setText(QString::number(meanTwo));
        }

        else
        {
            this->ui->cursorTwoAmplitudeLabel->setText("-");
        }


        if (!projectData->plotDataContainer->amplitudesAtFirstCursor.isEmpty()
                && !projectData->plotDataContainer->amplitudesAtSecondCursor.isEmpty())
        {
            double meanInter = (meanOne + meanTwo) / 2;

            this->ui->intervalAmplitudeLabel->setText(QString::number(meanInter));
        }

        else
        {
            this->ui->intervalAmplitudeLabel->setText("-");
        }
    }
}


/***************
 * Receive info from the data container and filter the hints to process only the
 * one of interest
 *-------------------------
 * size_t hint: flag that indicate specific changes in the data container. Allow
 * the method to know if it should take the signal into account or not.
 ***************/
void MainWindow::onProjectDataChanged(size_t hint)
{
    if (hint & ProjectData::eHintEegFileUnloaded)
    {
        if (projectData->plotDataContainer->eegFileNb == 0)
        {
            // Disable everything
            eventEnabler(false);
            eegEnabler(false);


            // Remove the display components
            delete wrapper;
            delete eventLabels;
            delete channelLabels;


            // Recreate them
            setWrapper();
            setEventLabels();
            setChannelLabels();
        }

        setFileNamesLegend();
    }

    if (hint & ProjectData::eHintEegFileChanged)
    {
        if (projectData->plotDataContainer->eegFileNb > 0)
        {
            eegEnabler(true);
        }

        setFileNamesLegend();
    }
}

