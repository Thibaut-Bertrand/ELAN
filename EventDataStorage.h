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
 * This class hold data about a specific .pos file, including the events samples
 * and states
 *
 * Author: Thibaut Bertrand
 * thibaut.bertrand@inserm.fr
 ***************/


#ifndef EVENTDATASTORAGE_H
#define EVENTDATASTORAGE_H


#include <QString>
#include <QVector>

#include "FileReader.h"


class EventDataStorage : public QWidget
{
    Q_OBJECT


public:

    EventDataStorage(QString filename);


    // Methods
    //--------------------

    // --- Getting events and info
    QVector<QVector<int>> eventsInRange(int lowerSample, int upperSample);
    void getEventsCodes();
    QVector<QVector<int>> getBrowsableEvents();

    // --- Edit events

    // Add
    void addNewEvent(int eventCodeToAdd);
    void addNewEvent(int eventSample, int eventCode);

    // Delete
    void deleteEvent(int eventCodeToDelete);

    // Recode
    void recodeEvent(int originalEventCode, int newEventCode);

    // States
    void eventChangeAcceptedState(int eventIndex, int eventState);
    void loadEventStates(QVector<QVector<int>> loadingList);
    void setEventBrowsabilityState(int eventCode, bool newBrowsabilityState);
    void setEventVisibilityState(int eventCode, bool newVisibilityState);



    // Members
    //--------------------
    QString eventFilename;

    bool areEventsLoaded;
    bool isFileActive = false;


    // Event Sample | Event Code | Accept/Reject 0/1
    // All the events
    QVector<QVector<int>> eventDataList;
    int closestEventToFocus = 0; // This is the index of eventDataList. When in event focus page mode, instead of copying or reading all the list of all the file all the time, I just take the closes


    // Event Code | Visible (1 = true, 0 = false) | Browsable (1 = true, 0 = false)
    // Only one line per code
    int uniqueEventNb = 0;
    QVector<QVector<int>> eventStates;


signals:

    void sendUpdatedEventStates(QVector<QVector<int>> updatedStates);
    void sendEventsToFocusOn(QVector<QVector<int>> eventsToFocus);

    void requestEventRedisplay(); // the CentralMiddleLayerManager will receive this request if the user apllied changes to the events (add/remove/recode, state change)

    void forwardKeyboardReleased();


public slots:

    void eventFileSave(QString filename);
};

#endif // EVENTDATASTORAGE_H
