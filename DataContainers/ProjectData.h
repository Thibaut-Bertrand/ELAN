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


#ifndef PROJECTDATA_H
#define PROJECTDATA_H


#include "AssociateEegPosFileWindow.h"
#include "ButterflyOrganization.h"
#include "ChooseActiveEventFileWindow.h"
#include "ChooseCursorWindow.h"
#include "ChooseFileWindow.h"
#include "EventWindow.h"
#include "PageData.h"
#include "PlotData.h"
#include "PresentationData.h"
#include "qwidget.h"
#include "RecodeCurrentEventDialog.h"
#include "RecodeEventDialog.h"
#include "ScaleWindow.h"
#include "SelectAlignmentReferenceWindow.h"
#include "SelectChannelWindow.h"
#include "SetCursorPositionWindow.h"
#include "UnloadEegFileWindow.h"
#include "UnloadEventFileWindow.h"


class ProjectData : public QWidget
{
    Q_OBJECT


private:

    // Members
    //--------------------

    // --- Windows
    AssociateEegPosFileWindow* associationWindow;
    ButterflyOrganization* butterflyCreationWindow;
    ChooseActiveEventFileWindow* activeEventFileWindow;
    ChooseCursorWindow* chooseCursorWindow;
    ChooseFileWindow* chooseFile;
    EventWindow* eventWindow;
    RecodeCurrentEventDialog* recodeCurrentEventWindow;
    ScaleWindow* scaleWindow;
    SelectAlignmentReferenceWindow* selectAlignmentWindow;
    SelectChannelWindow* selectChannelWindow;
    SetCursorPositionWindow* setCursorWindow;
    UnloadEegFileWindow* unloadEegWindow;
    UnloadEventFileWindow* unloadEventWindow;


public:

    ProjectData();


    // Methods
    //--------------------

    // --- Data
    void getDataForNewPage();
    void getTimeAtCursor();
    void goToPage();
    void horizontalAxesChanged(QVector<double> positions);


    // --- EEG
    void changeSignalCentering(bool isSignalCentered);
    void eegFileLoading(QString eegFilename);
    void selectFileForAmplitude();


    // --- Events
    void addEventOnCursor();
    void changeCurrentEventCode();
    void changeCurrentEventState(bool isAccepted);
    void requestPosFileSaving();
    void selectEventFileAssociation(QString eventFilename);


    // --- View modes
    void setClassicViewMode();
    void setEventFocusedViewMode();


    // --- Windows
    void openActiveEventFileWindow();
    void openButterflyCreationWindow();
    void openEventWindow();
    void openScaleWindow();
    void openSelectAlignementWindow();
    void openSelectChannelWindow();
    void openSetCursorWindow();
    void openUnloadEegFileWindow();
    void openUnloadEventFileWindow();


    // Members
    //--------------------
    QString projectCurrentDirectory = NULL;

    PageData* pageDataContainer = NULL;
    PlotData* plotDataContainer = NULL;
    PresentationData* presentationDataContainer = NULL;


    // ===== Hint structure =====
    using Hint = enum size_t
    {
        eHintNothingChanged = 0,
        eHintPageChanged = 1 << 0,
        eHintTimeScaleChanged = 1 << 1,
        eHintAmplitudeScaleChanged = 1 << 2,
        eHintEegFileChanged = 1 << 3,
        eHintAmplitudeScalesChanged = 1 << 4,
        eHintEegFileUnloaded = 1 << 5,
    };


signals:

    void sendCursorTimes(int cursorOne, int cursorTwo);

    void notifyChange(size_t hint);
    void test();


public slots:

    // --- Eeg file
    void eegFileUnloading(int fileIndex);
    void setTargetFileAmpli(int fileIndex);


    // --- Events
    void eventFileLoading(QString eventFilename, int eegFileIndex);


    // --- HUB
    void onPageDataChanged(size_t hint);
    void onPlotDataChanged(size_t hint);
    void onPresentationDataChanged(size_t hint);
};

#endif // PROJECTDATA_H
