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
 * This class is used as a wrapper that contain other, more specific data
 * containers, such as PlotData for variables concerning plots in general,
 * ScaleData for variables concerning the amplitude scales and ranges, PageData
 * for the time scale and the page information and PresentationData for colors,
 * themes and other aesthetic variables.
 * This class also contain some variable that don't match the other classes.
 *
 * Author: Thibaut Bertrand
 * thibaut.bertrand@inserm.fr
 ***************/


#include "ProjectData.h"


/***************
 * Constructor
 ***************/
ProjectData::ProjectData()
{
    pageDataContainer = new PageData();

    connect(this->pageDataContainer, SIGNAL(notifyChange(size_t)),
            this, SLOT(onPageDataChanged(size_t)));


    plotDataContainer = new PlotData();

    connect(this->plotDataContainer, SIGNAL(notifyChange(size_t)),
            this, SLOT(onPlotDataChanged(size_t)));


    presentationDataContainer = new PresentationData();

    connect(this->presentationDataContainer, SIGNAL(notifyChange(size_t)),
            this, SLOT(onPresentationDataChanged(size_t)));
}


//      %%%%%%%%%%%%%%%%%%%%%%%%
//      %%%% METHODS public %%%%
//      %%%%%%%%%%%%%%%%%%%%%%%%


//      ==============
//      %%%% Data %%%%
//      ==============


/***************
 * When changing page, this method will get the new data (eeg and event).
 ***************/
void ProjectData:: getDataForNewPage()
{
    // Get EEG data
    plotDataContainer->getDataForNewPage(pageDataContainer->timeAtFirstSample);


    // Get event (if needed)
    if (presentationDataContainer->areEventDisplayed)
    {
        plotDataContainer->requestEventToDisplay();
    }

    emit notifyChange(eHintPageChanged); // -> CentralMiddleLayerManager
}


/***************
 * Simply get the absolute time at the cursor's positions.
 ***************/
void ProjectData::getTimeAtCursor()
{
    // Get time at cursor, in milliseconds
    int cursorOneTime = -1;
    int cursorTwoTime = -1;

    // If in classic view (simple)
    if (!pageDataContainer->isViewFocusedOnEvent)
    {
        // Get time at first cursor
        if (presentationDataContainer->isCursorOneDisplayed)
        {
            cursorOneTime = ( (presentationDataContainer->cursorOneRelativePosition * pageDataContainer->timeScale) / 100 ) + pageDataContainer->timeAtFirstSample;
        }

        // Get time at second cursor
        if (presentationDataContainer->isCursorTwoDisplayed)
        {
            cursorTwoTime = ( (presentationDataContainer->cursorTwoRelativePosition * pageDataContainer->timeScale) / 100 ) + pageDataContainer->timeAtFirstSample;
        }
    }


    // In event focused view (way harder)
    else
    {
        // Get the time at the focused event (that's the time at the middle of the page)
        int eventSample = pageDataContainer->browsableEvents[pageDataContainer->currentEventFocusIndex][0];
        double fileFrequency = pageDataContainer->eegFileSamplingFrequency[pageDataContainer->browsableEvents[pageDataContainer->currentEventFocusIndex][2]];
        int timeAtEvent = (eventSample / fileFrequency) * 1000;

        // Get time at first cursor
        if (presentationDataContainer->isCursorOneDisplayed)
        {
            cursorOneTime = ( (presentationDataContainer->cursorOneRelativePosition * timeAtEvent) / 50 );
        }

        // Get time at second cursor
        if (presentationDataContainer->isCursorTwoDisplayed)
        {
            cursorTwoTime = ( (presentationDataContainer->cursorTwoRelativePosition * timeAtEvent) / 50 );
        }
    }

    emit sendCursorTimes(cursorOneTime, cursorTwoTime); // -> MainWindow
}


/***************
 * Ask to the user to which page s-he wants to go, then jump yo it.
 ***************/
void ProjectData::goToPage()
{
    bool ok;

    QString page = QInputDialog::getText(this, tr("Jump to Page..."),
                                         tr("Enter the page number:"),
                                         QLineEdit::Normal,
                                         "1", &ok);

    pageDataContainer->pageChange(page.toInt());
}


/***************
 * Called when the horizotal axis are changed (like when a new .eeg file is
 * loaded, with its own new channels). Update the channels labels position, on
 * the left side of the MainWindow.
 *-------------------------
 * QVector<double> positions: the positions of all the channels that have to be
 * updated.
 ***************/
void ProjectData::horizontalAxesChanged(QVector<double> positions)
{
    // Butterfly plots
    //--------------------
    if (plotDataContainer->areBtfDipslayed)
    {
        QVector<QString> butterflyLabels;

        for (int i_btf = 0; i_btf < plotDataContainer->butterflyPlotList.length(); i_btf++)
        {
            butterflyLabels.append(plotDataContainer->butterflyPlotList[i_btf]->name);
        }

        presentationDataContainer->setButterflyAxesPosition(positions,
                                                            butterflyLabels);
    }


    // Free channels
    //--------------------
    else
    {
        presentationDataContainer->setFreeChannelsAxesPosition(positions,
                    plotDataContainer->uniqueChannelLabelList,
                    plotDataContainer->uniqueChannelDisplayedList);
    }
}


//      =============
//      %%%% EEG %%%%
//      =============


/***************
 * Open the SelectChannelWindow window that let the user choose which channel
 * from which .eeg file to display or not.
 ***************/
void ProjectData::changeSignalCentering(bool isSignalCentered)
{
    plotDataContainer->isSignalCentered = isSignalCentered;

    getDataForNewPage();
}


/***************
 * Request the loading of an .eeg file.
 *-------------------------
 * QString filename: simply the name of the requested file.
 ***************/
void ProjectData::eegFileLoading(QString eegFilename)
{
    // Load .eeg file
    plotDataContainer->loadEegFile(eegFilename);


    // Initialize the eegDataStorage
    plotDataContainer->eegDataStorage.last()->updateSamplePerPage(pageDataContainer->timeScale);

    plotDataContainer->eegDataStorage.last()->initializeChannelsAmplitudeRange(plotDataContainer->defaultUVAmplitudeScale,
                                                                               plotDataContainer->defaultVAmplitudeScale,
                                                                               plotDataContainer->defaultFTAmplitudeScale);


    // Set page number
    QVector<int> freqList, sampList;

    for (int i_file = 0; i_file < plotDataContainer->eegFileNb; i_file++)
    {
        freqList.append(plotDataContainer->eegDataStorage[i_file]->sampFreq);
        sampList.append(plotDataContainer->eegDataStorage[i_file]->sampleNb);
    }

    pageDataContainer->setTotalPageNb(freqList, sampList);


    // Get data to display
    plotDataContainer->eegDataStorage.last()->getDataInRange(pageDataContainer->actualPage);


    // Get channels info accross all files
    plotDataContainer->getTotalUniqueChannels();

    emit notifyChange(eHintEegFileChanged); // -> CentralMiddleLayer
}


/***************
 * Simply ask the user which file to target to get the amplitude values at the
 * cursors.
 ***************/
void ProjectData::selectFileForAmplitude()
{
    if (plotDataContainer->eegFileNb > 0)
    {
        QVector<QString> fileList;

        for (int i_file = 0; i_file < plotDataContainer->eegFileNb; i_file++)
        {
            fileList.append(plotDataContainer->eegDataStorage[i_file]->eegFilename);
        }

        QString message = "Choose which data to target to get the amplitude values "
                          "at the cursor's positions.";

        chooseFile = new ChooseFileWindow(fileList, message);

        connect(chooseFile, SIGNAL(sendFileIndex(int)),
                this, SLOT(setTargetFileAmpli(int)));

        chooseFile->show();
    }
}


//      ================
//      %%%% Events %%%%
//      ================


/***************
 * Ask the user where the new event should be (first or second cursor), then ask
 * what will this event be (either an exiting one or a new one).
 ***************/
void ProjectData::addEventOnCursor()
{
    if (presentationDataContainer->cursorOneRelativePosition != 0
            || presentationDataContainer->cursorTwoRelativePosition != 0)
    {
        chooseCursorWindow = new ChooseCursorWindow(plotDataContainer,
                                                    presentationDataContainer);

        connect(chooseCursorWindow, SIGNAL(sendNewEventInfo(double, int, int)),
                plotDataContainer, SLOT(addEventOnCursor(double, int, int)));

        chooseCursorWindow->show();
    }
}


/***************
 * Change the currently focused event code, after requesting which will be the
 * new code to the user.
 ***************/
void ProjectData::changeCurrentEventCode()
{
    recodeCurrentEventWindow = new RecodeCurrentEventDialog(plotDataContainer,
                                                            pageDataContainer->browsableEvents[pageDataContainer->currentEventFocusIndex][2],
                                                            pageDataContainer->browsableEvents[pageDataContainer->currentEventFocusIndex][1]);

    recodeCurrentEventWindow->show();
}


/***************
 * Change the currently focused event accept/reject state. Only to use in event
 * focused view mode.
 *-------------------------
 * bool isAccepted: either accepted (true), or rejected (false).
 ***************/
void ProjectData::changeCurrentEventState(bool isAccepted)
{
    plotDataContainer->changeEventAcceptState(
                pageDataContainer->browsableEvents[pageDataContainer->currentEventFocusIndex][2],
                pageDataContainer->browsableEvents[pageDataContainer->currentEventFocusIndex][1],
                isAccepted);
}


/***************
 * The user is able to save a .pos file by clicking on "Event -> Save Event
 * File".
 * Just in case there's the need to add intermediate actions, I keep this
 * method.
 ***************/
void ProjectData::requestPosFileSaving()
{
    plotDataContainer->requestPosFileSaving();
}


/***************
 * Open a window that allow the user to select to which .eeg file associate the
 * specified event file.
 *-------------------------
 * QString eventFilename: simply the file name of the event file to load.
 ***************/
void ProjectData::selectEventFileAssociation(QString eventFilename)
{
    QVector<QString> eegFilenameList;

    for (int i_file = 0; i_file < plotDataContainer->eegFileNb; i_file++)
    {
        eegFilenameList.append(plotDataContainer->eegDataStorage[i_file]->eegFilename);
    }

    associationWindow = new AssociateEegPosFileWindow(eegFilenameList, eventFilename);

    connect(this->associationWindow, SIGNAL(sendFileIndex(QString, int)),
            this, SLOT(eventFileLoading(QString, int)));

    associationWindow->show();
    associationWindow->setWindowState(Qt::WindowState::WindowActive);
}


//      ====================
//      %%%% View modes %%%%
//      ====================


/***************
 * Set the view to the classic page mode. When changing view, the first page is
 * reloaded.
 ***************/
void ProjectData::setClassicViewMode()
{
    pageDataContainer->isViewFocusedOnEvent = false;
    pageDataContainer->pageChange(1);
}


/***************
 * Set the view to event focused mode. When changing view, the first page is
 * reloaded.
 ***************/
void ProjectData::setEventFocusedViewMode()
{
    pageDataContainer->browsableEvents = plotDataContainer->getAllBrowsableEvents();

    pageDataContainer->isViewFocusedOnEvent = true;
    pageDataContainer->currentEventFocusIndex = 0;
    pageDataContainer->eegFileSamplingFrequency = plotDataContainer->eegFileSamplingFrequency;
    pageDataContainer->pageChange(1);
}


//      =================
//      %%%% Windows %%%%
//      =================


/***************
 * Let the user choose which event file to activate for each data file that have
 * at least one of them loaded.
 ***************/
void ProjectData::openActiveEventFileWindow()
{
    activeEventFileWindow = new ChooseActiveEventFileWindow(plotDataContainer);

    connect(activeEventFileWindow, SIGNAL(activeEventFiles(QVector<int>)),
            this->plotDataContainer, SLOT(changeActiveEventFiles(QVector<int>)));

    activeEventFileWindow->show();
}


/***************
 * Open the window to allow the user to create various butterfly plots.
 ***************/
void ProjectData::openButterflyCreationWindow()
{
    butterflyCreationWindow = new ButterflyOrganization(plotDataContainer);

    butterflyCreationWindow->show();
}


/***************
 * Open the EventWindow window that let the user choose which event are visible/
 * browsable, delete/recode existing events and create new events.
 ***************/
void ProjectData::openEventWindow()
{
    eventWindow = new EventWindow(plotDataContainer);

    eventWindow->show();
}


/***************
 * Open the window to let the user manage the scales, time and amplitudes, as
 * s-he see fit.
 ***************/
void ProjectData::openScaleWindow()
{
    scaleWindow = new ScaleWindow(pageDataContainer, plotDataContainer);

    scaleWindow->show();
}


void ProjectData::openSelectAlignementWindow()
{
    selectAlignmentWindow = new SelectAlignmentReferenceWindow(plotDataContainer);

    connect(selectAlignmentWindow, SIGNAL(sendEventReference(QVector<int>)),
            this->plotDataContainer, SLOT(setEegOffset(QVector<int>)));

    selectAlignmentWindow->show();
}

/***************
 * Open the SelectChannelWindow window that let the user choose which channel
 * from which .eeg file to display or not.
 ***************/
void ProjectData::openSelectChannelWindow()
{
    selectChannelWindow = new SelectChannelWindow(plotDataContainer);

    selectChannelWindow->show();
}


/***************
 * Open the SelectChannelWindow window that let the user choose which channel
 * from which .eeg file to display or not.
 ***************/
void ProjectData::openSetCursorWindow()
{
    setCursorWindow = new SetCursorPositionWindow(presentationDataContainer,
                                                  pageDataContainer->timeScale);

    setCursorWindow->show();
}


/***************
 * Open the UnloadEegFileWindow window that let the user choose which .eeg file
 * to unload.
 ***************/
void ProjectData::openUnloadEegFileWindow()
{
    unloadEegWindow = new UnloadEegFileWindow(plotDataContainer);

    connect(unloadEegWindow, SIGNAL(sendFileToUnload(int)),
            this, SLOT(eegFileUnloading(int)));

    unloadEegWindow->show();
}


/***************
 * Open the UnloadEventFileWindow window that let the user choose which event
 * file from which .eeg file to unload.
 ***************/
void ProjectData::openUnloadEventFileWindow()
{
    unloadEventWindow = new UnloadEventFileWindow(plotDataContainer);

    connect(unloadEventWindow, SIGNAL(sendFileToUnload(int, int)),
            this->plotDataContainer, SLOT(unloadEventFile(int, int)));

    unloadEventWindow->show();
}


//      %%%%%%%%%%%%%%%%%%%%%%
//      %%%% SLOTS public %%%%
//      %%%%%%%%%%%%%%%%%%%%%%


//      ===================
//      %%%% Eeg files %%%%
//      ===================


/***************
 * Request the unloading of an .eeg file.
 *-------------------------
 * int fileIndex: the index of the file to unload.
 ***************/
void ProjectData::eegFileUnloading(int fileIndex)
{
    plotDataContainer->unloadEegFile(fileIndex);

    // If there's still some data to display
    if (plotDataContainer->eegFileNb > 0)
    {
        // Get channels info accross all files
        plotDataContainer->getTotalUniqueChannels();


        // Set page number
        QVector<int> freqList, sampList;

        for (int i_file = 0; i_file < plotDataContainer->eegFileNb; i_file++)
        {
            freqList.append(plotDataContainer->eegDataStorage[i_file]->sampFreq);
            sampList.append(plotDataContainer->eegDataStorage[i_file]->sampleNb);
        }

        pageDataContainer->setTotalPageNb(freqList, sampList);
    }

    emit notifyChange(eHintEegFileUnloaded); // -> MainWindow && -> CentralMiddleLayerManager
}


/***************
 * Set the index of the file that is used to get amplitude values on the cursor
 * position.
 *-------------------------
 * int fileIndex: the index of the target file.
 ***************/
void ProjectData::setTargetFileAmpli(int fileIndex)
{
    plotDataContainer->fileIndexForAmplitude = fileIndex;

    getTimeAtCursor();
}


//      ================
//      %%%% Events %%%%
//      ================


/***************
 * Request the loading of an event file.
 *-------------------------
 * QString eventFilename: the path of the event file to laod.
 * int eegFileIndex: the index of the data file to which associate the event
 * file.
 ***************/
void ProjectData::eventFileLoading(QString eventFilename, int eegFileIndex)
{
    plotDataContainer->loadEventFile(eventFilename, eegFileIndex);

    presentationDataContainer->areEventDisplayed = true;

    pageDataContainer->pageChange(1);
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
void ProjectData::onPageDataChanged(size_t hint)
{
    if (hint & PageData::eHintPageChanged)
    {
        getDataForNewPage();
    }

    else if (hint & PageData::eHintTimeScaleChanged)
    {
        if (plotDataContainer->eegFileNb > 0)
        {
            // Adapt the eegDataStorages to the new scale
            plotDataContainer->timeScaleChanged(pageDataContainer->timeScale);

            emit notifyChange(eHintTimeScaleChanged); // -> CentralMiddleLayer

            pageDataContainer->pageChange(1);

            QVector<int> freqList, sampList;

            for (int i_file = 0; i_file < plotDataContainer->eegFileNb; i_file++)
            {
                freqList.append(plotDataContainer->eegDataStorage[i_file]->sampFreq);
                sampList.append(plotDataContainer->eegDataStorage[i_file]->sampleNb);
            }

            pageDataContainer->setTotalPageNb(freqList, sampList);

            // Get data
            getDataForNewPage();
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
void ProjectData::onPlotDataChanged(size_t hint)
{
    if (hint & PlotData::eHintLoadedEventFileChanged)
    {
        getDataForNewPage();
    }

    if (hint & PlotData::eHintAmplitudeScalesChanged)
    {
        emit notifyChange(eHintAmplitudeScalesChanged); // -> CentralMiddleLayerManager

        getDataForNewPage();
    }

    if (hint & PlotData::eHintOffsetChanged)
    {
        pageDataContainer->pageChange(1);
    }
}


/***************
 * Receive info from the data container and filter the hints to process only the
 * one of interest
 *-------------------------
 * size_t hint: flag that indicate specific changes in the data container. Allow
 * the method to know if it should take the signal into account or not.
 ***************/
void ProjectData::onPresentationDataChanged(size_t hint)
{
    if (hint & PresentationData::eHintCursorsChanged)
    {
        getTimeAtCursor();

        plotDataContainer->getAmplitudeAtCursor(presentationDataContainer->cursorOneRelativePosition,
                                                presentationDataContainer->cursorTwoRelativePosition);
    }
}
