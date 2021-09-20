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


#ifndef EEGDATASTORAGE_H
#define EEGDATASTORAGE_H


#include <QString>
#include <QVector>


#include "DataContainers/ChannelData.h"
#include "EventDataStorage.h"
#include "FileReader.h"


class EegDataStorage
{

public:

    EegDataStorage(QString filename);


    // Methods
    //--------------------

    // --- Channels
    void getAllLabel();
    void initializeChannelsAmplitudeRange(long int uVScale, long int VScale, long int fTScale);
    void setChannelAmplitudeRange(int channelID, int newAmplitudeScale);


    // --- Data
    void centerSignal();
    void getDataInRange(int timeAtFirstSample);
    void setDataOffset(int eventID);


    // --- Time scale an Samples
    void updateSamplePerPage(int pageTimeScale);
    void updateBorderSamples(int lowerSample);


    // --- .pos and events management
    void getEventToDisplay();
    void loadEventFile(QString filename);
    void saveActivePosFile(bool replace, QString filename = "");
    void unloadPosFile(int eventFileIndex);

    void addEvents(QVector<int> eventCodesToAdd);
    void addEventOnCursor(double relativePosition, int eventCode);
    void deleteEvents(QVector<int> eventCodesToDelete);
    void recodeEvents(QVector<QVector<int>> eventCodesToRecode);
    void setEventVisibBrowsStates(QVector<QVector<bool>> newEventStates);

    QVector<QVector<int>> getBrowsableEvents();
    void changeEventCode(int eventID, int newCode);
    void changeEventState(int eventID, bool state);


    // Members
    //--------------------

    // --- Files
    QString eegFilename;
    QString entFilename; // Important if the header file isn't in the same location as the .eeg file.


    // --- Samples
    int sampleNb = 0;
    int samplePerPage = 0; // Same as the timeScale just above. Renamed it for clearer understanding, while I adapt the code. The timeScale member will be deleted after.
    int currentLowerSample = 0; // The lower sample displayed on the page. Depends on the samplePerge, which depends on the timescale and the sampling frequency
    int currentUpperSample = 0; // The upper sample displayed on the page. Depends on the samplePerge, which depends on the timescale and the sampling frequency


    // --- Channels
    int channelNb = 0;
    int dataOffset = 0;
    QVector<ChannelData*> channels;
    QVector<QString> channelList;


    // --- Sampling frequency
    double sampFreq = 0.;


    // --- Events
    bool hasEventLoaded = false;
    QVector<EventDataStorage*> eventDataStorage;
    QVector<bool> activePosFiles;
    int activeEventFileIndex = 0;

    // This struct concern only the event that are present in the current page and that have to be displayed
    struct event
    {
        int sample = 0;
        int code = 0;
        int acceptState = 1;
    };

    QVector<event> eventToDisplay;


    // --- Data
    QVector<QVector<double>> dataToDisplay; // Contains the data that are in range to be displayed
    QVector<QVector<double>> dataToDisplayCentered;
    QVector<QString> labelList;
};

#endif // EEGDATASTORAGE_H
