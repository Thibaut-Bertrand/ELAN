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
 *
 * Author: Thibaut Bertrand
 * thibaut.bertrand@inserm.fr
 ***************/


#ifndef CENTRALMIDDLELAYERMANAGER_H
#define CENTRALMIDDLELAYERMANAGER_H


#include <QDebug>
#include <QVector>
#include <QString>


#include "CentralMiddleLayerDisplayer.h"
#include "CentralMiddleLayerEvents.h"
#include "DataContainers/ProjectData.h"



class CentralMiddleLayerManager : public QWidget
{
    Q_OBJECT


public:

    CentralMiddleLayerManager(QSize size, ProjectData* projectData);


    // Methods (override)
    //--------------------
    void resizeEvent(QResizeEvent* e);


    ProjectData* projectData;

    CentralMiddleLayerDisplayer* displayer;
    CentralMiddleLayerEvents* eventsDisplayer;


signals:

    void forwardCursorPosition(int, int);



public slots:

    void onProjectDataChanged(size_t hint);
    void onPlotDataChanged(size_t hint);
};

#endif // CENTRALMIDDLELAYERMANAGER_H
