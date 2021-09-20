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
 * This widget is used as a base for the event catcher.
 *
 * Author: Thibaut Bertrand
 * thibaut.bertrand@inserm.fr
 ***************/


#ifndef CENTRALTOPLAYER_H
#define CENTRALTOPLAYER_H


#include <QDebug>
#include <QtWidgets>


#include "DataContainers/PresentationData.h"
#include "EventCatcherTopLayer.h"
#include "qpainter.h"


class CentralTopLayer : public QWidget
{

    Q_OBJECT

public:

    CentralTopLayer(QSize size, PresentationData* presentation);
    ~CentralTopLayer();


    // Members
    //--------------------
    EventCatcherTopLayer *eventLayer;


signals:

    // From Event Layer
    //--------------------
    void forwardCursorsInterval(double one, double two);
    void forwardCursorPosition(double, QString);
    void forwardCursorPosition(int, int);


    // To Event Layer
    //--------------------
    void forwardResetCursors();
    void forwardCursorStateChange();
    void forwardCursorIntervalInput(int, int);
};

#endif // CENTRALTOPLAYER_H
