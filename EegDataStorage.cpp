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
 * This class hold data about a specific .eeg file, including its channels and
 * associated .pos file(s).
 *
 * Author: Thibaut Bertrand
 * thibaut.bertrand@inserm.fr
 ***************/


#include "EegDataStorage.h"


/***************
 * Constructor
 *-------------------------
 ***************/
EegDataStorage::EegDataStorage(QString filename)
{
    eegFilename = filename;
}


//      %%%%%%%%%%%%%%%%%%%%%%%%
//      %%%% METHODS public %%%%
//      %%%%%%%%%%%%%%%%%%%%%%%%


//      ==================
//      %%%% Channels %%%%
//      ==================


void EegDataStorage::getAllLabel()
{
    labelList.clear();

    for (int i_chan = 0; i_chan < channelNb; i_chan++)
    {
        labelList.append(channels[i_chan]->label);
    }
}


/***************
 * Initialize the QCPRange for each channel, depending on their unit (uV, V, fT)
 * and the specified scale for each of those.
 *-------------------------
 * int uVScale: the scale for micro-volt.
 * int VScale: the scale for volt.
 * int fTScale: the scale for femto-Tesla.
 *
 * Note: Actual and default amplitude scales are found in the ScaleData class.
 ***************/
void EegDataStorage::initializeChannelsAmplitudeRange(long int uVScale,
                                                 long int VScale,
                                                 long int fTScale)
{
    for (int i_chan = 0; i_chan < channels.length(); i_chan++)
    {
        if (channels[i_chan]->unit == "uV")
        {
            channels[i_chan]->setRange(uVScale);
        }

        else if (channels[i_chan]->unit == "V")
        {
            channels[i_chan]->setRange(VScale);
        }

        else if (channels[i_chan]->unit == "fT")
        {
            channels[i_chan]->setRange(fTScale);
        }
    }
}


/***************
 * Change the specified channel amplitude range with the new specified scale.
 *-------------------------
 * int channelID: the channel ID which have it's aplitude range to changed.
 * int newAmplitudeScale: the new amplitude scale to apply. The range simply go
 * from (-newScale) to (newScale).
 ***************/
void EegDataStorage::setChannelAmplitudeRange(int channelID,
                                              int newAmplitudeScale)
{
    channels[channelID]->setRange(newAmplitudeScale);
}


//      ==============
//      %%%% Data %%%%
//      ==============


/***************
 * Center the signals of each channels for which the bool is true, so that their
 * data are aligned with their origin.
 ***************/
void EegDataStorage::centerSignal()
{
    double sum;
    double mean;

    dataToDisplayCentered.clear();
    dataToDisplayCentered = dataToDisplay;

    for (int i_chan = 0; i_chan < dataToDisplay.length(); i_chan++)
    {
        sum = 0.;
        mean = 0.;

        sum = std::accumulate(dataToDisplay[i_chan].begin(),
                              dataToDisplay[i_chan].end(), 0.);

        mean = sum / dataToDisplay[i_chan].size();

        for (int j_data = 0; j_data < dataToDisplayCentered[i_chan].length(); j_data++)
        {
            dataToDisplayCentered[i_chan][j_data] -= mean;
        }
    }
}


/***************
 * Get the data of all channels that correspond to the range between the
 * indicated time at first sample, and the end of the page, which is computed by
 * the number of sample to display per page.
 *-------------------------
 * int timeAtFirstSample: the time that correspond to the first sample in the
 * window.
 ***************/
void EegDataStorage::getDataInRange(int timeAtFirstSample)
{
    // Clean slate
    if (!dataToDisplay.empty())
    {
        dataToDisplay.clear();
    }

    dataToDisplay.resize(channelNb);


    // Get the starting sample of the page.
    // As different file can have different sampling frequency, and potentially
    // an offset to align them, each file can have a different starting sample
    // for the considered displayed page
    // timeAtFirstSample is in milliseconds!
    int startingSample = timeAtFirstSample * (sampFreq / 1000);
    updateBorderSamples(startingSample + dataOffset);


    if (currentLowerSample >= 0)
    {
        for (int i_chan = 0; i_chan < channelNb; i_chan++)
        {
            dataToDisplay[i_chan].resize(samplePerPage);
        }

        FileReader::yAxisData(eegFilename, dataToDisplay,
                              currentLowerSample, samplePerPage);
    }

    else
    {
        int intervalToRead = samplePerPage - dataOffset;

        currentLowerSample = 0;

        if (intervalToRead > 0)
        {
            for (int i_chan = 0; i_chan < channelNb; i_chan++)
            {
                dataToDisplay[i_chan].resize(intervalToRead);
            }

            FileReader::yAxisData(eegFilename, dataToDisplay,
                                  currentLowerSample, intervalToRead);

            for (int i_chan = 0; i_chan < channelNb; i_chan++)
            {
                for (int j_offset = 0; j_offset < dataOffset; j_offset++)
                {
                    dataToDisplay[i_chan].insert(0, qQNaN());
                }
            }
        }

        else
        {
            for (int i_chan = 0; i_chan < channelNb; i_chan++)
            {
                for (int j_point = 0; j_point < samplePerPage; j_point++)
                {
                    dataToDisplay[i_chan].insert(0, qQNaN());
                }
            }
        }
    }
}


/***************
 * When multiple .eeg files are loaded, and associated with .pos files, they
 * don't necesseraly have their event 0 set at the same time (for example, the
 * first file can have it's event 0 at 10 seconds, and the second at 20
 * seconds). For that reason, it is necessary to apply an offset to the data in
 * order to align the event 0 of all the files.
 *-------------------------
 * int eventID: the event index that will serve as the "0" for the data file.
 ***************/
void EegDataStorage::setDataOffset(int eventID)
{
    int eventSample = eventDataStorage[activeEventFileIndex]->eventDataList[eventID][0];

    dataOffset = eventSample;
}


//      ================================
//      %%%% Time scale and samples %%%%
//      ================================


/***************
 * Update the number of sample that have to be displayed on the screen.
 * Depends on the time scale of the page, and the sampling frequency of the
 * file.
 *-------------------------
 * int pageTimeScale: the time scale of the page, in milliseconds.
 ***************/
void EegDataStorage::updateSamplePerPage(int pageTimeScale)
{
    samplePerPage = (sampFreq / 1000) * pageTimeScale; // /1000 to get it in samp/msec instead of samp/sec

    updateBorderSamples(currentLowerSample);
}


/***************
 * Update the lower and upper sample that are displayed on the page. This is
 * mainly useful to know if a particular event have to be displayed (its sample
 * is in the range) or not.
 ***************/
void EegDataStorage::updateBorderSamples(int lowerSample)
{
    // Prevent to have values under or above the limit
    if (lowerSample < 0)
    {
        lowerSample = 0;
    }

    else if (lowerSample + samplePerPage > sampleNb)
    {
        lowerSample = sampleNb - samplePerPage;
    }


    // Update the sample limit
    currentLowerSample = lowerSample;
    currentUpperSample = currentLowerSample + samplePerPage;
}


//      ====================================
//      %%%% .pos and events management %%%%
//      ====================================


/***************
 * Get all the event, if any, that are present in the actually displayed page.
 ***************/
void EegDataStorage::getEventToDisplay()
{
    if (!eventDataStorage.isEmpty())
    {
        QVector<QVector<int>> eventList = eventDataStorage[activeEventFileIndex]->eventsInRange(currentLowerSample, currentUpperSample);

        eventToDisplay.clear();

        for (int i_event = 0; i_event < eventList.length(); i_event++)
        {
            event newEvent;

            newEvent.sample = eventList[i_event][0];
            newEvent.code = eventList[i_event][1];
            newEvent.acceptState = eventList[i_event][2];

            eventToDisplay.append(newEvent);
        }
    }
}


/***************
 * Load a .pos file.
 * Each .eeg file can have multiple .pos file, but only one .pos file per .eeg
 * file can be displayed.
 *-------------------------
 * QString eventFilename: the path of the file to load.
 ***************/
void EegDataStorage::loadEventFile(QString eventFilename)
{
    // If there's no event file loaded
    if (!hasEventLoaded)
    {
        EventDataStorage* dataStorage = new EventDataStorage(eventFilename);

        eventDataStorage.append(dataStorage);

        hasEventLoaded = true;

        activePosFiles.append(true); // First .pos file always displayed
    }

    else
    {
        EventDataStorage* dataStorage = new EventDataStorage(eventFilename);

        eventDataStorage.append(dataStorage);

        activePosFiles.append(false); // Subcequent .pos files aren't displayed by default
    }
}


void EegDataStorage::saveActivePosFile(bool replace, QString filename)
{
    if (!replace && filename != "")
    {
        eventDataStorage[activeEventFileIndex]->eventFileSave(filename);
    }

    else if (replace)
    {
        filename = eventDataStorage[activeEventFileIndex]->eventFilename;
        eventDataStorage[activeEventFileIndex]->eventFileSave(filename);
    }
}


/***************
 * Unload the specified .pos file. Then update the active event file. If there's
 * none left, tell that there's isn't any event laoded.
 *-------------------------
 * int eventFileIndex: the event file index to unload.
 ***************/
void EegDataStorage::unloadPosFile(int eventFileIndex)
{
    eventDataStorage.remove(eventFileIndex);
    activePosFiles.remove(eventFileIndex);

    if (activeEventFileIndex >= eventFileIndex)
    {
        activeEventFileIndex - 1 < 0 ?
                    activeEventFileIndex = 0 :
                    activeEventFileIndex -= 1;
    }

    if (eventDataStorage.isEmpty())
    {
        hasEventLoaded = false;
    }
}


/***************
 * Add the specified events to the active .pos file.
 *-------------------------
 * QVector<int> eventCodesToAdd: the event codes to add.
 ***************/
void EegDataStorage::addEvents(QVector<int> eventCodesToAdd)
{
    for (int i_event = 0; i_event < eventCodesToAdd.length(); i_event++)
    {
        eventDataStorage[activeEventFileIndex]->addNewEvent(eventCodesToAdd[i_event]);
    }
}


/***************
 * Add the specified events to the active .pos file, at the specified relative
 * position. Called when the user want to add and event on one of the cursor
 * position.
 *-------------------------
 * double relativePosition: the relative position of the new event in the
 * actually displayed page. In percentage of the displayed page.
 * int eventCode: the code of the new event.
 ***************/
void EegDataStorage::addEventOnCursor(double relativePosition, int eventCode)
{
    int eventSample = currentLowerSample + ( (samplePerPage * relativePosition) / 100 );

    eventDataStorage[activeEventFileIndex]->addNewEvent(eventSample, eventCode);
}


/***************
 * Delete the specified events from the active .pos file.
 *-------------------------
 * QVector<int> eventCodesToDelete: the event codes to delete.
 ***************/
void EegDataStorage::deleteEvents(QVector<int> eventCodesToDelete)
{
    for (int i_event = 0; i_event < eventCodesToDelete.length(); i_event++)
    {
        eventDataStorage[activeEventFileIndex]->deleteEvent(eventCodesToDelete[i_event]);
    }
}


/***************
 * Recode the specified events from the active .pos file.
 *-------------------------
 * QVector<int> eventCodesToRecode: the event to recode (include the original
 * and new codes).
 ***************/
void EegDataStorage::recodeEvents(QVector<QVector<int>> eventCodesToRecode)
{
    for (int i_event = 0; i_event < eventCodesToRecode.length(); i_event++)
    {
        eventDataStorage[activeEventFileIndex]->recodeEvent(eventCodesToRecode[i_event][0],
                                                            eventCodesToRecode[i_event][1]);
    }
}


/***************
 * Change teh visibility and browsability of the active .pos file event.
 *-------------------------
 * QVector<int> eventCodes: the event list.
 * QVector<QVector<bool>> newEventStates: the visibility and browsability states
 * of the events
 ***************/
void EegDataStorage::setEventVisibBrowsStates(QVector<QVector<bool>> newEventStates)
{
    for (int i_event = 0; i_event < newEventStates.length(); i_event++)
    {
        int eventCode = eventDataStorage[activeEventFileIndex]->eventStates[i_event][0];

        eventDataStorage[activeEventFileIndex]->setEventVisibilityState(eventCode,
                                                                        newEventStates[i_event][0]);
        eventDataStorage[activeEventFileIndex]->setEventBrowsabilityState(eventCode,
                                                                          newEventStates[i_event][1]);
    }
}


/***************
 * Simply ask the active eventDataStorage to return all of its browsable events.
 ***************/
QVector<QVector<int>> EegDataStorage::getBrowsableEvents()
{
    return eventDataStorage[activeEventFileIndex]->getBrowsableEvents();
}



/***************
 * Apply the specified new code to the specicified event. The code must exist.
 * This is only used to change one single event.
 *-------------------------
 * int eventID: the ID of the event in the active .pos file.
 * bool state: the state to apply to the event
 ***************/
void EegDataStorage::changeEventCode(int eventID, int newCode)
{
    eventDataStorage[activeEventFileIndex]->eventDataList[eventID][1] = newCode;
}


/***************
 * Change the specified event accept/reject state. This is only used to change
 * one single event.
 *-------------------------
 * int eventID: the ID of the event in the active .pos file.
 * bool state: the state to apply to the event
 ***************/
void EegDataStorage::changeEventState(int eventID, bool state)
{
    int stateToApply;
    state ? stateToApply = 0 : stateToApply = 1;
    eventDataStorage[activeEventFileIndex]->eventDataList[eventID][2] = stateToApply;
    getEventToDisplay();
}
