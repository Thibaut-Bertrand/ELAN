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


/* ########################################################################## */
/* Custom Widget with an overriden paintEvent method to display the event     */
/* codes.                                                                     */
/*                                                                            */
/* Author: Thibaut Bertrand                                                   */
/* thibaut.bertrand@inserm.fr                                                 */
/* ########################################################################## */


#ifndef EVENTLABELSDISPLAYER_H
#define EVENTLABELSDISPLAYER_H


#include <QWidget>


#include "DataContainers/ProjectData.h"
#include "qcustomplot.h"


class EventLabelsDisplayer : public QWidget
{

    Q_OBJECT

public:
    EventLabelsDisplayer(QSize size, ProjectData* project);
    ~EventLabelsDisplayer();

    // Methods (override)
    //--------------------
    void paintEvent(QPaintEvent *e);


    // Members
    //--------------------
    QVector<QVector<QVector<int>>> eventToDisplay; // EventSample | Event Code
    int focusedEvent = -1;

    ProjectData* projectData;


public slots:

    void newEvents(QVector<QVector<QVector<int>>> eventList);
    void onPresentationDataChanged(size_t hint);
};

#endif // EVENTLABELSDISPLAYER_H
