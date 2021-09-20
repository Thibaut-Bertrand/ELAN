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


#ifndef PLOTDATA_H
#define PLOTDATA_H


#include <tuple>


#include "ChooseFileWindow.h"
#include "EegDataStorage.h"
#include "FileReader.h"
#include "qwidget.h"


class PlotData : public QWidget
{
    Q_OBJECT


private:

    // Methods
    //--------------------
    void readAmplitudeUnitScales();


    // Members
    //--------------------
    ChooseFileWindow* chooseFile;


public:

    PlotData();


    // Methods
    //--------------------

    // --- Butterfly plots
    void changeButterflyDisplay(bool displayState);
    void initializeButterflyList(QVector<QString> plotsName,
                                 QVector<QVector<QVector<int>>> allFilesAndChannelsID);


    // --- Channels
    void changeAllChannelStates(QVector<QVector<QString>> presentChannels,
                                QVector<QVector<bool>> newStates);
    void getChannelsToDisplay();
    void getTotalUniqueChannels();
    void setChannelsAmplitudeScale(QVector<QVector<int>> newScales);


    // --- EEG files
    void getAmplitudeAtCursor(double cursorOneRelativePosition,
                              double cursorTwoRelativePosition);
    void getDataForNewPage(int timeAtFirstSample);
    void loadEegFile(QString eegFilename);
    void timeScaleChanged(int newTimeScale);
    void unloadEegFile(int fileIndex);


    // --- Event files
    bool areEventsLoaded();
    void loadEventFile(QString eventFilename, int eegFileIndex);
    void requestEventToDisplay();
    void requestPosFileSaving();

    void addEvents(int eefFileID, QVector<int> eventCodesToAdd);
    void applyChangesToEventFiles();
    void changeEventStates(QVector<QVector<bool>> eventStates, int fileIndex);
    void changeEventAcceptState(int eegFileID, int eventID, bool state);
    void recodeEvents(int eegFileID, QVector<QVector<int>> eventCodesToRecode);

    QVector<QVector<int>> getAllBrowsableEvents();
    void recodeCurrentEvent(int fileID, int eventID, int newEventCode,
                            bool isNewCode = false, bool isVisible = false,
                            bool isBrowsable = false);


    // Members
    //--------------------

    // --- Butterfly ---
    bool areBtfDipslayed = false;

    struct butterfly
    {
        int ID;
        QString name;
        QVector<QVector<int>> filesAndChannelsID; // File | Channel
    };

    QVector<butterfly*> butterflyPlotList;


    // --- EEG data ---
    // Those list contain one iteration of each unique channel across all file
    // Those list are used for the display order. The first chan in the list will be displayed at the top.
    double defaultUVAmplitudeScale = 100.; // micro-volt, EEG
    double defaultVAmplitudeScale = 100000.; // volt, EOG...
    double defaultFTAmplitudeScale = 100000000.; // femto-tesla, MEG
    QMap<QString, int> defaultScales;


    int uniqueChannelNb = 0;
    int channelToDisplayNb = 0;
    QVector<QString> uniqueChannelLabelList; // Used by the channel label displayer on the left, and the scale window
    QVector<QString> uniqueChannelUnitList; // Used by the scale window
    QVector<bool> uniqueChannelDisplayedList; // Used by the scale window and the channel window, but might change it to allow control over channel all all individual files

    bool isSignalCentered = false;


    int eegFileNb = 0;
    QVector<QString> eegFileNameList;
    QVector<double> eegFileSamplingFrequency;
    QVector<EegDataStorage*> eegDataStorage;


    int fileIndexForAmplitude = 0; // The target file to get amplitude values at cursor position
    QVector<double> amplitudesAtFirstCursor;
    QVector<double> amplitudesAtSecondCursor;


    // --- Event ---
    // This list is use for a particular situation: when the user want to apply
    // changes to event from the EventWindow. This way, only one method is
    // needed (without having to give a thousand variables), and at its end, it
    // ask to get the new data to display only once.
    struct eventsToChange
    {
        int eegFileID; // The .eeg file that contain the targeted .pos file.
        QVector<int> eventCodesToDelete; // The list of event code to delete.
        QVector<int> eventCodesToAdd; // The list of event code to add.
        QVector<QVector<int>> eventCodesToRecode; // The list of event to rename with the new code.
        QVector<QVector<bool>> remainingEventStates; // The list of existing event (after deleting and adding) visibility and browsability states.
    };

    QVector<eventsToChange> listOfEventsToChange;


    // --- Samples ---
    bool isMiddleSampleReference = false;
    int referenceSamples;


    // ===== Hint structure =====
    using Hint = enum size_t
    {
        eHintNothingChanged = 0,
        eHintButterflyDisplayChanged = 1 << 0,
        eHintChannelsChanged = 1 << 1,
        eHintLoadedEventFileChanged = 1 << 2,
        eHintEventStateChanged = 1 << 3,
        eHintAmplitudeScalesChanged = 1 << 4,
        eHintOffsetChanged = 1 << 5,
        eHintAmpliAtChannelChanged = 1 << 6,
    };


signals:

    void notifyChange(size_t hint);


public slots:

    // --- Eeg
    void setEegOffset(QVector<int> selectedEvents);

    // --- Events
    void addEventOnCursor(double cursorRelativePosition, int fileID,
                          int eventCode);
    void changeActiveEventFiles(QVector<int> eventFileIndex);
    void savePosFile(QString filename);
    void unloadEventFile(int eegID, int eventID);
};

#endif // PLOTDATA_H
