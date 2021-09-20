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


#include "EventDataStorage.h"


/***************
 * Constructor
 *-------------------------
 * QString filename: the path of the event file to read.
 ***************/
EventDataStorage::EventDataStorage(QString filename)
{
    eventFilename = filename;

    // eventDataList:
    // Event Sample | Event Code | Reject
    // The Vector will be automatically sorted in FileReader
    FileReader::ReadPosFile(eventFilename, eventDataList);
    areEventsLoaded = true;

    getEventsCodes();
}


//      %%%%%%%%%%%%%%%%%%%%%%%%
//      %%%% METHODS public %%%%
//      %%%%%%%%%%%%%%%%%%%%%%%%


//      =================================
//      %%%% Getting events and info %%%%
//      =================================


/***************
 * Return a 2D Vector of the visible events present in the indicated range.
 *-------------------------
 * int lowerSample: the first sample on the displayed window.
 * in upperSample: the last sample on the displayed window
 ***************/
QVector<QVector<int>> EventDataStorage::eventsInRange(int lowerSample,
                                                      int upperSample)
{
    // eventDataList:
    // Sample | Code | Accept/Reject

    // eventStates:
    // Event Code | Visible | Browsable

    // eventsInRange:
    // Sample | Code | Accept/Reject
    // Same as eventDataList, but only the visble one in the specified range!


    QVector<QVector<int>> eventsInRange;

    int i_event = 0;

    // While the scanned event ISN'T above the UPPER limit of the window
    while (eventDataList[i_event][0] < upperSample)
    {
        // If the scanned event IS above the LOWER limit of the window
        if (eventDataList[i_event][0] > lowerSample)
        {
            // We look at the visibility state of the event
            for (int j_visib = 0; j_visib < eventStates.size(); j_visib++)
            {
                // If the event is visible, we add its index to the list of the displayed events
                if (eventStates[j_visib][0] == eventDataList[i_event][1]
                        && eventStates[j_visib][1] == 1)
                {
                    QVector<int> event;

                    event.append(eventDataList[i_event][0]);
                    event.append(eventDataList[i_event][1]);
                    event.append(eventDataList[i_event][2]);

                    eventsInRange.append(event);
                }
            }
        }

        i_event++;
    }

    return eventsInRange;
}


/***************
 * Take the eventDataList and collect the event code, keeping only one of each
 * and sorting them. Then, add two column, indicating whether the events are
 * visible and/or browsable.
 ***************/
void EventDataStorage::getEventsCodes()
{
    // eventDataList:
    // Sample | Code | Reject

    // eventCodesAndStatesList:
    // Event Code | Visible | Browsable


    QVector<int> eventsCodesList;

    // Here we only want the second column (Code), so eventDatalist[i][1].
    for (int i = 0; i < eventDataList.size(); i++)
    {
        // If eventCodeList empty, then we automatically take the first one.
        if (eventsCodesList.isEmpty())
        {
            eventsCodesList.append(eventDataList[i][1]);
        }

        // We only take the codes that aren't in the list yet.
        else if (!eventsCodesList.contains(eventDataList[i][1]))
        {
            eventsCodesList.append(eventDataList[i][1]);
        }
    }

    // Sort the list
    std::sort(eventsCodesList.begin(), eventsCodesList.end());


    // Now, we put the list into a 2D QVector that will also contains the states
    // of the events (visisble or not (1 or 0) and browsable or not (1 or 0).
    // Default states are at 1 (visible and browsable).
    eventStates.resize(eventsCodesList.size());

    for (int i = 0; i < eventStates.size(); i++)
    {
        eventStates[i].resize(3);

        eventStates[i][0] = eventsCodesList[i]; // Event Code
        eventStates[i][1] = 1; // Visible: 1 = true, 0 = false
        eventStates[i][2] = 1; // Browsable: 1 = true, 0 = false
    }

    uniqueEventNb = eventStates.length();
}



/***************
 * Get all the browsable events of the file. Return a vector containing each of
 * those events. Each event is composed of its sample position and its ID (index
 * in the eventDataList). This list, combined with the lists of others
 * eventDataStorage is used by the PageData data container when the user wants
 * its view to be focused on events.
 ***************/
QVector<QVector<int>> EventDataStorage::getBrowsableEvents()
{
    QVector<QVector<int>> browsableEvents;

    for (int i_event = 0; i_event < eventDataList.length(); i_event++)
    {
        for (int j_brows = 0; j_brows < eventStates.length(); j_brows++)
        {
            if (eventDataList[i_event][1] == eventStates[j_brows][0]
                    && eventStates[j_brows][2] == 1)
            {
                browsableEvents.append(QVector<int> {eventDataList[i_event][0], i_event});
            }
        }
    }

    return browsableEvents;
}


//      =====================
//      %%%% Edit events %%%%
//      =====================


//      %%%% Add %%%%


/***************
 * Add a new event with the specified code. This method is called when the user
 * wants to apply the changes s-he made in the EventWindow, or when creating a
 * new event in the MainWindow.
 * This event is added in the eventStatesList, with default states. This event
 * is just a code, it does not have an instance in the eventDataList yet, as
 * it's the user that'll have to do it (unless s-he create the new code while
 * placing a new event in the MainWindow).
 *-------------------------
 * int eventCodeToAdd: simply the code of the new event to create. Should not be
 * a code that already exist. Superior to 0.
 ***************/
void EventDataStorage::addNewEvent(int eventCodeToAdd)
{
    // Changes made on: eventStatesList
    eventStates.append(QVector<int>{eventCodeToAdd, 1, 1});
    uniqueEventNb = eventStates.length();
}


/***************
 * Add an event to the specified position with the specified code. The code must
 * exist.
 *-------------------------
 * int eventSample: the position, in sample, of the event.
 * int eventCode: the code of the event to add.
 ***************/
void EventDataStorage::addNewEvent(int eventSample, int eventCode)
{
    int index = 0;

    while (eventDataList[index][0] < eventSample)
    {
        index++;
    }

    eventDataList.insert(index - 1, QVector<int>{eventSample, eventCode, 0});
}

//      %%%% Delete %%%%


/***************
 * Delete the specified event from the eventDataList and the eventStates. This
 * method is called when the user apply the changes s-he made in the
 * EventWindow.
 *-------------------------
 * int eventCodeToDelete: simply the event code the user wants to delete. Should
 * be a code that exist. Superior to 0.
 ***************/
void EventDataStorage::deleteEvent(int eventCodeToDelete)
{
    // Changes made on: eventStatesList and eventDataList

    // eventStates
    for (int i_event = 0; i_event < eventStates.length(); i_event++)
    {
        if (eventStates[i_event][0] == eventCodeToDelete)
        {
            eventStates.remove(i_event);
            uniqueEventNb = eventStates.length();
            break;
        }
    }


    // eventDataList
    for (int i_event = eventDataList.length() - 1; i_event >= 0; i_event--)
    {
        if (eventDataList[i_event][1] == eventCodeToDelete)
        {
            eventDataList.remove(i_event);
        }
    }
}


//      %%%% Recode %%%%


/***************
 * Replace the specified event code by the specified new one, in both
 * eventDataList and eventStates. Called after the user apply its changes in the
 * EventWindow.
 *-------------------------
 * int originalEventCode : the original code that the user wants to change. Must
 * be an existing code. Superior to 0.
 * int newEventCode: the new code selected by the user. Must be a code that does
 * not already exist. Superior to 0.
 ***************/
void EventDataStorage::recodeEvent(int originalEventCode, int newEventCode)
{
    // Changes made on: eventStatesList and eventDataList
    // eventStates
    for (int i_event = 0; i_event < eventStates.length(); i_event++)
    {
        if (eventStates[i_event][0] == originalEventCode)
        {
            eventStates[i_event][0] = newEventCode;
            break;
        }
    }


    // eventDataList
    for (int i_event = 0; i_event < eventDataList.length(); i_event++)
    {
        if (eventDataList[i_event][1] == originalEventCode)
        {
            eventDataList[i_event][1] = newEventCode;
        }
    }
}


//      %%%% States %%%%


/***************
 * Change the state of the currently focused event.
 * The view should be on event focus mode.
 *-------------------------
 ***************/
void EventDataStorage::eventChangeAcceptedState(int eventIndex, int eventState)
{
    // eventsToFocusList (just the ones with the same code):
    // Sample | index of the event in eventDataList

    // eventDataList (all the events of the file):
    // Sample | Event Code | Accept/Reject


    // Change the state
    eventDataList[eventIndex][2] = eventState;
}



/***************
 * When loading a project, this will update eventCodesAndStatesList.
 *-------------------------
 * QVector<QVector<int>> loadingList: the list of states to laod.
 ***************/
void EventDataStorage::loadEventStates(QVector<QVector<int>> loadingList)
{
    // eventCodesAndStateList and loadingList:
    // | Event Code | Visible | Browsable |

    eventStates.clear();
    eventStates.resize(loadingList.size());

    for (int i = 0; i < eventStates.size(); i++)
    {
        eventStates[i].resize(3);
        eventStates[i][0] = loadingList[i][0];
        eventStates[i][1] = loadingList[i][1];
        eventStates[i][2] = loadingList[i][2];
    }

    uniqueEventNb = eventStates.length();
}




/***************
 * Change the browsability state of the specified event with the new specified
 * state.
 * Note: A browsable event must be visible.
 *-------------------------
 * int eventCode: the event which state has to be changed.
 * int newBrowsabilityState: the new visibiility of the event.
 ***************/
void EventDataStorage::setEventBrowsabilityState(int eventCode, bool newBrowsabilityState)
{
    // Changes made on: eventStatesList
    int newState = 0;
    newBrowsabilityState ? newState = 1 : newState = 2;

    // Changes made on: eventStatesList
    for (int i_event = 0; i_event < eventStates.length(); i_event++)
    {
        if (eventStates[i_event][0] == eventCode)
        {
            eventStates[i_event][2] = newState;
            break;
        }
    }
}


/***************
 * Change the visibility state of the specified event with the new specified
 * state.
 * Note: an invisible event cannot be browsable.
 *-------------------------
 * int eventCode: the event which state has to be changed.
 * int newVisibiilityState: the new visibiility of the event.
 ***************/
void EventDataStorage::setEventVisibilityState(int eventCode, bool newVisibilityState)
{
    // Changes made on: eventStatesList
    int newState = 0;
    newVisibilityState ? newState = 1 : newState = 2;

    // Changes made on: eventStatesList
    for (int i_event = 0; i_event < eventStates.length(); i_event++)
    {
        if (eventStates[i_event][0] == eventCode)
        {
            eventStates[i_event][1] = newState;
            break;
        }
    }
}


//      %%%%%%%%%%%%%%%%%%%%%%
//      %%%% SLOTS public %%%%
//      %%%%%%%%%%%%%%%%%%%%%%


/***************
 * Save the eventDataList in the specified file.
 *-------------------------
 * QString filename: the path of the save file.
 ***************/
void EventDataStorage::eventFileSave(QString filename)
{
    // Create a new file
    QFile file(filename);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);

    for (int i = 0; i < eventDataList.length(); i++)
    {
        // Sample
        out << eventDataList[i][0] << "     ";

        // Code
        out << eventDataList[i][1] << "     ";

        // Accepted (0) / Rejected (1)
        out << eventDataList[i][2] << "\n";
    }

    file.close();
}
