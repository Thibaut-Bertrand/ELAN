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
 * This class is contains data about plots, channels and butterfly plots.
 *
 * Author: Thibaut Bertrand
 * thibaut.bertrand@inserm.fr
 ***************/


#include "PlotData.h"


/***************
 * Constructor
 ***************/
PlotData::PlotData()
{
    readAmplitudeUnitScales();
}


//      %%%%%%%%%%%%%%%%%%%%%%%%%
//      %%%% METHODS private %%%%
//      %%%%%%%%%%%%%%%%%%%%%%%%%


/***************
 * This method will read the key/value pairs in the amplitudeUnitScales.txt file
 * and make them the default scales.
 ***************/
void PlotData::readAmplitudeUnitScales()
{
    QFile file(":/resources/amplitudeUnitScales.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    while (!file.atEnd()) {
        QString line = file.readLine();
        line.remove(QRegExp("[\\n\\t\\r]"));

        QStringList list = line.split(" ");

         for (int i = 0; i < list.count(); i += 2)
         {
             defaultScales[list[i]] = list[i + 1].toInt();
         }
    }
}


//      %%%%%%%%%%%%%%%%%%%%%%%%
//      %%%% METHODS public %%%%
//      %%%%%%%%%%%%%%%%%%%%%%%%


//      =========================
//      %%%% Butterfly plots %%%%
//      =========================


void PlotData::changeButterflyDisplay(bool displayState)
{
    areBtfDipslayed = displayState;

    emit notifyChange(eHintButterflyDisplayChanged);
}


/***************
 * Create the butterflyplot list with the specified data.
 *-------------------------
 * QVector<QString> plotsName: the name list of the plot to create.
 * QVector<QVector<QVector<int>>> filesAndChannelsID: the vector<int> is
 * composed of two values : file ID | channel ID. The vector<vector<int>> is the
 * channels composing one single butterbly plots. And so, the
 * vector<vector<vector<int>>> represent all the butterfly plots.
 ***************/
void PlotData::initializeButterflyList(QVector<QString> plotsName,
                                       QVector<QVector<QVector<int>>> allFilesAndChannelsID)
{
    if (!plotsName.isEmpty())
    {
        butterflyPlotList.clear();

        for (int i_btf = 0; i_btf < plotsName.length(); i_btf++)
        {
            butterfly* newButterfly = new butterfly;

            newButterfly->ID = butterflyPlotList.length();
            newButterfly->name = plotsName[i_btf];
            newButterfly->filesAndChannelsID = allFilesAndChannelsID[i_btf];

            butterflyPlotList.append(newButterfly);
        }

        changeButterflyDisplay(true);
    }
}


//      ==================
//      %%%% Channels %%%%
//      ==================


/***************
 * Change all the channel display state (isDeiplayed), by the new given states.
 * Mainly used after closing the SelectChannelWindow.
 *-------------------------
 * QVector<QVector<bool>> newStates: the new channel states to apply to the
 * channels. First dimension = file; Second dimension = channel.
 ***************/
void PlotData::changeAllChannelStates(QVector<QVector<QString>> presentChannels,
                                      QVector<QVector<bool>> newStates)
{
    for (int i_file = 0; i_file < eegFileNb; i_file++)
    {
        for (int j_chan = 0; j_chan < eegDataStorage[i_file]->channels.length(); j_chan++)
        {
            for (int k_new = 0; k_new < presentChannels[i_file].length(); k_new++)
            {
                if(presentChannels[i_file][k_new] == eegDataStorage[i_file]->channels[j_chan]->label)
                {
                    eegDataStorage[i_file]->channels[j_chan]->isDisplayed = newStates[i_file][j_chan];
                }
            }
        }
    }

    getChannelsToDisplay();

    emit notifyChange(eHintChannelsChanged); // -> CentralMiddleLayerManager
}


/***************
 * Get the channels that have to be displayed. As the user can choose which
 * channel to display or not for each file separatly, this is necessary to know
 * what to do.
 ***************/
void PlotData::getChannelsToDisplay()
{
    uniqueChannelDisplayedList.clear();
    channelToDisplayNb = 0;

    uniqueChannelDisplayedList.resize(uniqueChannelNb);
    uniqueChannelDisplayedList.fill(false);


    for (int i_file = 0; i_file < eegFileNb; i_file++)
    {
        for (int j_chan = 0; j_chan < eegDataStorage[i_file]->channelNb; j_chan++)
        {
            for (int k_unique = 0; k_unique < uniqueChannelNb; k_unique++)
            {
                if (uniqueChannelLabelList[k_unique] == eegDataStorage[i_file]->channels[j_chan]->label
                        && eegDataStorage[i_file]->channels[j_chan]->isDisplayed)
                {
                    uniqueChannelDisplayedList[k_unique] = true;

                    break;
                }
            }
        }
    }

    channelToDisplayNb = uniqueChannelDisplayedList.count(true);
}


/***************
 * Get one unique iteration of a channel accross all file. If a channel appears
 * in more than one file, it will on be present one time in the created lists.
 * As their name's suggest it, those lists are used to know how many UNIQUE
 * channel there is.
 * There two lists: Label (QString, channel name) and Unit (QString, channel
 * unit).
 ***************/
void PlotData::getTotalUniqueChannels()
{
    // First, a clean slate
    uniqueChannelLabelList.clear(); // QString
    uniqueChannelUnitList.clear(); // QString


    // To accelerate things, directly take all channels from the first file.
    for (int i_chan = 0; i_chan < eegDataStorage[0]->channels.length(); i_chan++)
    {
        uniqueChannelLabelList.append(eegDataStorage[0]->channels[i_chan]->label);
        uniqueChannelUnitList.append(eegDataStorage[0]->channels[i_chan]->unit);
    }


    // Then, if there's more file beyond the first one, check if their channel is present or not
    for (int i_file = 1; i_file < eegDataStorage.length(); i_file++)
    {
        // Then, for each of their channels
        for (int j_chan = 0; j_chan < eegDataStorage[i_file]->channels.length(); j_chan++)
        {
            if (!uniqueChannelLabelList.contains(eegDataStorage[i_file]->channels[j_chan]->label))
            {
                uniqueChannelLabelList.append(eegDataStorage[i_file]->channels[j_chan]->label);
                uniqueChannelUnitList.append(eegDataStorage[i_file]->channels[j_chan]->unit);
            }
        }
    }

    uniqueChannelNb = uniqueChannelLabelList.length();

    getChannelsToDisplay();
}


/***************
 * Change the amplitude scale of all the channels from all the loaded .eeg
 * files. The provided list must contain a value for each of the channel of each
 * file.
 *-------------------------
 * QVector<QVector<int>> newScales: list of all the amplitude scales to apply to
 * all channels of all the file. This list comes from the ScaleWindow window.
 ***************/
void PlotData::setChannelsAmplitudeScale(QVector<QVector<int>> newScales)
{
    for (int i_file = 0; i_file < eegFileNb; i_file++)
    {
        for (int j_chan = 0; j_chan < eegDataStorage[i_file]->channelNb; j_chan++)
        {
            eegDataStorage[i_file]->setChannelAmplitudeRange(j_chan, newScales[i_file][j_chan]);
        }
    }

    emit notifyChange(eHintAmplitudeScalesChanged); // -> ProjectData
}


//      ===================
//      %%%% EEG files %%%%
//      ===================


/***************
 * Get the amplitude values of all channel at the specified position.
 *-------------------------
 * double cursorOneRelativePosition: the relative position of the first cursor,
 * in percentage of the widht of the central widget.
 * double cursorTwoRelativePosition: the relative position of the second cursor,
 * in percentage of the widht of the central widget.
 ***************/
void PlotData::getAmplitudeAtCursor(double cursorOneRelativePosition,
                                    double cursorTwoRelativePosition)
{
    amplitudesAtFirstCursor.clear();
    amplitudesAtSecondCursor.clear();

    if (fileIndexForAmplitude > eegFileNb - 1)
    {
        fileIndexForAmplitude = 0;
    }


    if (eegFileNb > 0)
    {
        // Initialize the vectors
        for (int i_uniq = 0; i_uniq < uniqueChannelNb; i_uniq++)
        {
            // -1 means theres no value to display.
            amplitudesAtFirstCursor.append(-1);
            amplitudesAtSecondCursor.append(-1);
        }


        // Get the sample at the given position
        int firstSample = eegDataStorage[fileIndexForAmplitude]->dataToDisplay[0].length()
                * (cursorOneRelativePosition / 100);
        int secondSample = eegDataStorage[fileIndexForAmplitude]->dataToDisplay[0].length()
                * (cursorTwoRelativePosition / 100);


        // Get the amplitude of the samples
        // But as different files = different channels
        // We first need to iterate through all possible channels

        for (int i_chan = 0; i_chan < eegDataStorage[fileIndexForAmplitude]->channelNb; i_chan++)
        {
            for (int j_uniq = 0; j_uniq < uniqueChannelLabelList.length(); j_uniq++)
            {
                if (uniqueChannelLabelList[j_uniq] == eegDataStorage[fileIndexForAmplitude]->channels[i_chan]->label)
                {
                    if (cursorOneRelativePosition > 0.)
                    {
                        double amplitude = eegDataStorage[fileIndexForAmplitude]->dataToDisplay[i_chan][firstSample];
                        amplitudesAtFirstCursor[j_uniq] = amplitude;
                    }

                    if (cursorTwoRelativePosition > 0.)
                    {
                        double amplitude = eegDataStorage[fileIndexForAmplitude]->dataToDisplay[i_chan][secondSample];
                        amplitudesAtSecondCursor[j_uniq] = amplitude;
                    }
                }
            }
        }
    }

    emit notifyChange(eHintAmpliAtChannelChanged);
}


/***************
 * Request to all EegDataStorage to get the data for the new page. Also, if the
 * user wants channel' signals to be centered, so be it.
 *-------------------------
 * int timeAtFirstSample: the time at the first sample that have to be
 * displayed.
 ***************/
void PlotData::getDataForNewPage(int timeAtFirstSample)
{
    for (int i_file = 0; i_file < eegFileNb; i_file++)
    {
        eegDataStorage[i_file]->getDataInRange(timeAtFirstSample);

        if (isSignalCentered)
        {
            eegDataStorage[i_file]->centerSignal();
        }
    }
}


/***************
 * This method will load and initialize an eegDataStorage class with the
 * provided file path.
 *-------------------------
 * QString filename: the .eeg file path used to load the file.
 ***************/
void PlotData::loadEegFile(QString eegFilename)
{
    EegDataStorage* newDataStorage = new EegDataStorage(eegFilename);

    std::tuple<int, double> tuple(FileReader::readAllFile(eegFilename,
                                                          newDataStorage->channels));
    newDataStorage->sampleNb = std::get<0>(tuple);
    newDataStorage->sampFreq = std::get<1>(tuple);
    newDataStorage->channelNb = newDataStorage->channels.length();
    newDataStorage->getAllLabel();
//    newDataStorage->updateChannelPosition();


    newDataStorage->channelList.clear();
    for (int i_chan = 0; i_chan < newDataStorage->channels.length(); i_chan++)
    {
        newDataStorage->channelList.append(newDataStorage->channels[i_chan]->label);
    }



    eegFileSamplingFrequency.append(newDataStorage->sampFreq);

    eegDataStorage.append(newDataStorage);
    eegFileNb += 1;
    eegFileNameList.append(eegFilename);
}


/***************
 * Called when the user want to change the time scale. This method will update
 * the number of sample per pageeach file have to display. As each file can have
 * it's own sampling frequency, the only thing that matter is how many they have
 * to display, not the time scale per se.
 *-------------------------
 * int newTimeScale: the new time scale, in milliseconds, used to update the
 * number of sample per page.
 ***************/
void PlotData::timeScaleChanged(int newTimeScale)
{
    for (int i_file = 0; i_file < eegFileNb; i_file++)
    {
        eegDataStorage[i_file]->updateSamplePerPage(newTimeScale);
    }
}


/***************
 * This method will unload the specified .eeg file.
 *-------------------------
 * int index: the index of QVector<eegDataStorage> corresponding to the .eeg
 * file requested to unload.
 ***************/
void PlotData::unloadEegFile(int fileIndex)
{
    if (!eegDataStorage.isEmpty()
            && fileIndex >= 0 && fileIndex < eegDataStorage.size())
    {
        eegDataStorage.remove(fileIndex);
        eegFileSamplingFrequency.remove(fileIndex);
        eegFileNb -= 1;
    }
}


//      =====================
//      %%%% Event files %%%%
//      =====================


/***************
 * Simply check if there's at least one event file loaded.
 ***************/
bool PlotData::areEventsLoaded()
{
    for (int i_file = 0; i_file < eegFileNb; i_file++)
    {
        if (eegDataStorage[i_file]->hasEventLoaded)
        {
            return true;
        }
    }

    return false;
}

/***************
 * Load the specified event file while associating it to the specified .eeg
 * file.
 *-------------------------
 * QString eventFilename: simply the file name of the event file to load.
 * int eegFileIndex: the .eeg file index of eegDatastorage to which associate
 * the event file.
 ***************/
void PlotData::loadEventFile(QString eventFilename, int eegFileIndex)
{
    eegDataStorage[eegFileIndex]->loadEventFile(eventFilename);

    requestEventToDisplay();
}


/***************
 * Request to all .eeg file that are associated with an event file to get the
 * list of events that are present in the current page range.
 ***************/
void PlotData::requestEventToDisplay()
{
    for (int i_file = 0; i_file < eegFileNb; i_file++)
    {
        if (eegDataStorage[i_file]->hasEventLoaded)
        {
            eegDataStorage[i_file]->getEventToDisplay();
        }
    }
}


/***************
 * The user is able to save a .pos file by clicking on "Event -> Save Event
 * File". This method will ask the user which file have to be saved.
 ***************/
void PlotData::requestPosFileSaving()
{
    QVector<QString> eventFileList;

    for (int i_file = 0; i_file < eegFileNb; i_file++)
    {
        if (eegDataStorage[i_file]->hasEventLoaded)
        {
            int fileIndex = eegDataStorage[i_file]->activeEventFileIndex;
            eventFileList.append(eegDataStorage[i_file]->eventDataStorage[fileIndex]->eventFilename);
        }
    }

    QString message = "Choose which .pos file you want to save.";

    chooseFile = new ChooseFileWindow(eventFileList, message);

    connect(chooseFile, SIGNAL(sendFileName(QString)),
            this, SLOT(savePosFile(QString)));

    chooseFile->show();
}


/***************
 * Search the eeg file that correspond to the specified one (and that have event
 * loaded), and send the new event codes to add.
 *-------------------------
 * int eegFileID: the eeg file that contain the event file targeted by the
 * changes.
 * QVector<int> eventCodesToAdd: the list of event codes to add. They will have
 * default visibility and browsability states.
 ***************/
void PlotData::addEvents(int eegFileID, QVector<int> eventCodesToAdd)
{
    eegDataStorage[eegFileID]->addEvents(eventCodesToAdd);
}


/***************
 * When the user apply the changes in the EventWindow, this method is called.
 * It will automatically search the correct .eeg file and give them the needed
 * information to apply on their loaded .pos file.
 * When all changes are applied, it will call the method to get the updated data
 * to display on the screen.
 ***************/
void PlotData::applyChangesToEventFiles()
{
    int listIndex = 0;

    for (int i_file = 0; i_file < eegFileNb; i_file++)
    {
        if (eegDataStorage[i_file]->hasEventLoaded)
        {
            // Delete events
            eegDataStorage[i_file]->deleteEvents(listOfEventsToChange[listIndex].eventCodesToDelete);


            // Add events
            addEvents(i_file, listOfEventsToChange[listIndex].eventCodesToAdd);


            // Recode events
            recodeEvents(i_file, listOfEventsToChange[listIndex].eventCodesToRecode);


            // Visibility/Browsability event states
            eegDataStorage[i_file]->setEventVisibBrowsStates(listOfEventsToChange[listIndex].remainingEventStates);


            listIndex++;
        }
    }

    emit(notifyChange(eHintLoadedEventFileChanged)); // -> ProjectData
}


/***************
 * Change the event state of the active event file from the targeted data file.
 *-------------------------
 * QVector<QVector<bool>> eventStates: the state of the events. Each event have
 * a Visible bool and a Browsable bool.
 * int fileIndex: the data file that have the targeted event file.
 ***************/
void PlotData::changeEventStates(QVector<QVector<bool>> eventStates,
                                 int fileIndex)
{
    eegDataStorage[fileIndex]->setEventVisibBrowsStates(eventStates);


    emit(notifyChange(eHintLoadedEventFileChanged)); // -> ProjectData
}


/***************
 * Change the accept/reject state of the specified event from the specified
 * .eeg file (which contain the .pos file to which the event is attached).
 *-------------------------
 * int eegFileID: the eeg file that contain the event file targeted by the
 * changes.
 * int eventID: the event that needs it accept/reject state to be changed.
 * bool state: either accepted (true), or rejected (false).
 ***************/
void PlotData::changeEventAcceptState(int eegFileID, int eventID, bool state)
{
    eegDataStorage[eegFileID]->changeEventState(eventID, state);

    emit notifyChange(eHintEventStateChanged); // -> CentralMiddleLayerManager
}


/***************
 * Change the listed event codes by the new specified codes.
 *-------------------------
 * int eegFileID: the eeg file that contain the event file targeted by the
 * changes.
 * QVector<int> originalEventCodes: the original event codes that needs to be
 * changed.
 * QVector<int> newEventCodes: the new event codes.
 ***************/
void PlotData::recodeEvents(int eegFileID, QVector<QVector<int>> eventCodesToRecode)
{
    eegDataStorage[eegFileID]->recodeEvents(eventCodesToRecode);
}


/***************
 * Get al the browsable events from all the loaded and active .pos files. Return
 * a list with the file and event ID and the sample of each of those events.
 * This list is used by the PageData class to determine which event is currently
 * focused and which is the next/previous.
 ***************/
QVector<QVector<int>> PlotData::getAllBrowsableEvents()
{
    QVector<QVector<int>> allBrowsableEvents;

    for (int i_file = 0; i_file < eegFileNb; i_file++)
    {
        // Get the events of each file
        if (eegDataStorage[i_file]->hasEventLoaded)
        {
            QVector<QVector<int>> browsableEvents = eegDataStorage[i_file]->getBrowsableEvents();

            // Insert the file ID for each of those event
            for (int j_event = 0; j_event < browsableEvents.length(); j_event++)
            {
                browsableEvents[j_event].append(i_file);
            }

            // Append to the big list
            allBrowsableEvents.append(browsableEvents);
        }
    }

    // Sort the vector in the samples order before returning it
    FileReader::SortVector(allBrowsableEvents);

    return allBrowsableEvents;
}


/***************
 * Change the currently focused event code, after requesting which will be the
 * new code to the user.
 *-------------------------
 * int fileID: the file to which the event is associated.
 * int eventID: the event which is the target to the recoding.
 * int newEventCode: the code to apply to the currently focused event.
 ***************/
void PlotData::recodeCurrentEvent(int fileID, int eventID, int newEventCode,
                                  bool isNewCode, bool isVisible,
                                  bool isBrowsable)
{
    // If it's just a simple event renaming
    if (!isNewCode)
    {
        // Just change the event' code
        eegDataStorage[fileID]->changeEventCode(eventID, newEventCode);
    }

    // If it's an event renaming as well as a code creation
    else
    {
        // Add the new code
        int visible, browsable;
        isVisible ? visible = 1 : visible = 0;
        isBrowsable ? browsable = 1 : browsable = 0;
        eegDataStorage[fileID]->eventDataStorage[eegDataStorage[fileID]->activeEventFileIndex]->eventStates.append(QVector<int>{newEventCode, visible, browsable});

        // Change the event' code
        eegDataStorage[fileID]->changeEventCode(eventID, newEventCode);
    }

    emit(notifyChange(eHintLoadedEventFileChanged)); // -> ProjectData
}


//      %%%%%%%%%%%%%%%%%%%%%%
//      %%%% SLOTS public %%%%
//      %%%%%%%%%%%%%%%%%%%%%%


//      =============
//      %%%% Eeg %%%%
//      =============


/***************
 * Set the offset of the files that have event loaded. If a file doesn't have an
 * event file, then it can't be realigned ('cause there's no mark to use as a
 * reference).
 *-------------------------
 * QVector<int> selectedEvents: the events seelcted to serve as a 0 for each
 * data file associated with an event file.
 ***************/
void PlotData::setEegOffset(QVector<int> selectedEvents)
{
    int currentEventFile = 0;

    for (int i_file = 0; i_file < eegFileNb; i_file++)
    {
        if (eegDataStorage[i_file]->hasEventLoaded)
        {
            eegDataStorage[i_file]->setDataOffset(selectedEvents[currentEventFile]);
            currentEventFile++;
        }
    }

    emit notifyChange(eHintOffsetChanged); // -> ProjectData
}


//      ================
//      %%%% Events %%%%
//      ================


/***************
 * Add an event on the specified position, with the specified code, within the
 * specified file.
 *-------------------------
 * double cursorRelativePosition: the position on which add the new event.
 * int fileID: the file to which add teh event.
 * int eventCode: the code of the new event.
 ***************/
void PlotData::addEventOnCursor(double cursorRelativePosition, int fileID,
                                int eventCode)
{
    eegDataStorage[fileID]->addEventOnCursor(cursorRelativePosition, eventCode);

    emit(notifyChange(eHintLoadedEventFileChanged)); // -> ProjectData
}


/***************
 * Change teh active event files for all .eeg file that have at least one event
 * file active.
 *-------------------------
 * QVector<int>: the index list of the event file to activate.
 ***************/
void PlotData::changeActiveEventFiles(QVector<int> eventFileIndex)
{
    int currentEventFile = 0;

    for (int i_file = 0; i_file < eegFileNb; i_file++)
    {
        if (eegDataStorage[i_file]->hasEventLoaded)
        {
            eegDataStorage[i_file]->activeEventFileIndex = eventFileIndex[currentEventFile];
            currentEventFile++;
        }
    }

    emit(notifyChange(eHintLoadedEventFileChanged)); // -> ProjectData
}


/***************
 * First, get the data file index that contain the event file the user wants to
 * save.
 * Then, ask the user if he wants to overwrite the orignal .pos file or if s-he
 * prefer to make a new one.
 *-------------------------
 * int fileIndex: the active .pos file that is targeted by the user.
 ***************/
void PlotData::savePosFile(QString filename)
{
    // Get the data file index
    int dataFileIndex;

    for (int i_file = 0; i_file < eegFileNb; i_file++)
    {
        if (eegDataStorage[i_file]->hasEventLoaded)
        {
            int activeIndex = eegDataStorage[i_file]->activeEventFileIndex;

            if (eegDataStorage[i_file]->eventDataStorage[activeIndex]->eventFilename
                    == filename)
            {
                dataFileIndex = i_file;
            }
        }
    }

    QMessageBox msgBox;
    msgBox.setWindowTitle("Save .pos file.");
    msgBox.setText("Do you want to create a new .pos file?\n"
                   "If so, click 'Yes'.\n"
                   "Otherwise, click 'No' to replace the orignal. Warning: "
                   "this action is irreversible!");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);

    int ret = msgBox.exec();

    if (ret == QMessageBox::Yes)
    {
        // Get the new filename
        QFileDialog dialog;
        QString newFilename = dialog.getSaveFileName(this, tr("Choose save location"),
                                                     QDir::currentPath() + "/untitled.pos",
                                                     tr(".pos"));

        eegDataStorage[dataFileIndex]->saveActivePosFile(false, newFilename);
    }

    else if (ret == QMessageBox::No)
    {
        eegDataStorage[dataFileIndex]->saveActivePosFile(true);
    }

}


/***************
 * Unload the specified event file from the specified .eeg file.
 *-------------------------
 * int eegID: the .eeg file to which the event file is associated with.
 * int eventID: the event file index to unload.
 ***************/
void PlotData::unloadEventFile(int eegID, int eventID)
{
    eegDataStorage[eegID]->unloadPosFile(eventID);

    emit(notifyChange(eHintLoadedEventFileChanged)); // -> ProjectData
}
