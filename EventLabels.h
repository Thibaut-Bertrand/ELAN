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
 * This class manage the displaying of event code and of time ticks on top of
 * the plot frame. The ticks are positionned on a regular interval, and each
 * event present in the plot frame will have its code displayed.
 *
 * Author: Thibaut Bertrand
 * thibaut.bertrand@inserm.fr
 ***************/


#ifndef EVENTLABELS_H
#define EVENTLABELS_H


#include "qwidget.h"
#include "EventLabelsDisplayer.h"
#include "DataContainers/ProjectData.h"


class EventLabels : public QWidget
{
    Q_OBJECT


public:

    EventLabels(QSize size, ProjectData* project);
    ~EventLabels();


    // Members
    //--------------------
    QBrush brush;
    int width;

    bool displayTicks = true;

    EventLabelsDisplayer* eventDisplayer;
    ProjectData* projectData;


signals:

    void forwardDisplayEventCode(QVector<QVector<QVector<int>>>);


public slots:

    void onPresentationDataChanged(size_t hint);
    void onProjectDataChanged(size_t hint);
};

#endif // EVENTLABELS_H
