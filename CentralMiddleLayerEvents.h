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
 * Display the events from the loaded .pos files. Accepted events will be in the
 * same color as their associated .eeg file. Rejected events will be in red
 * color.
 *
 * Author: Thibaut Bertrand
 * thibaut.bertrand@inserm.fr
 ***************/


#ifndef CENTRALMIDDLELAYEREVENTS_H
#define CENTRALMIDDLELAYEREVENTS_H


#include "DataContainers/ProjectData.h"
#include "ProjectManager.h"


class CentralMiddleLayerEvents : public QWidget
{

    Q_OBJECT


public:

    CentralMiddleLayerEvents(QSize size, ProjectData* project);
    ~CentralMiddleLayerEvents();


    // Methods (override)
    //--------------------
    void paintEvent(QPaintEvent *e);
    void resizeEvent(QResizeEvent *e);


    // Methods
    //--------------------
    void displayEvents();


    // Members
    //--------------------

    // Events
    int eventNb;
    QVector<QVector<QVector<int>>> eventInRange; // List the sample (position) of the event in range to be displayed
    QPainter painter;

    // Others
    ProjectData* projectData;


private slots:

    void onPresentationDataChanged(size_t hint);
};

#endif // CENTRALMIDDLELAYEREVENTS_H
