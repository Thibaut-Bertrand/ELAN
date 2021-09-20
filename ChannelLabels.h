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
 * The manager of the channel labels displayer.
 *
 * Author: Thibaut Bertrand
 * thibaut.bertrand@inserm.fr
 ***************/


#ifndef CHANNELLABELS_H
#define CHANNELLABELS_H


#include <QDebug>
#include <QLayout>


#include "qwidget.h"
#include "ChannelLabelsDisplayer.h"
#include "DataContainers/ProjectData.h"


class ChannelLabels : public QWidget
{

    Q_OBJECT


public:

    ChannelLabels(QSize size, ProjectData* project);
    ~ChannelLabels();


    // Members
    //--------------------
    ChannelLabelsDisplayer* displayer;
    ProjectData* projectData;


public slots:

    void onPresentationDataChanged(size_t hint);
    void onPlotDataChanged(size_t hint);
};

#endif // CHANNELLABELS_H
