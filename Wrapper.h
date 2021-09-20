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
 * The wrapper contain multiple layers that are used to manage different aspects
 * of the central widget, allowing the display of the data.
 * The Bottom Layer manage the background.
 * The Middle Layer manage the data that are displayed.
 * The Top Layer manage the cursors and the event catcher.
 *
 * Author: Thibaut Bertrand
 * thibaut.bertrand@inserm.fr
 ***************/


#ifndef WRAPPER_H
#define WRAPPER_H


#include <QDebug>
#include <QResizeEvent>
#include <QWidget>


#include "CentralBottomLayer.h"
#include "CentralMiddleLayerManager.h"
#include "CentralTopLayer.h"
#include "DataContainers/ProjectData.h"


class Wrapper : public QWidget
{

    Q_OBJECT


private:

    // Members
    //--------------------
    QWidget *bottomLayer; // Background
    QWidget *middleLayer; // Plots
    CentralTopLayer *topLayer; // Cursors, events


public:

    Wrapper(QSize size, ProjectData* projectData);


    // Methods (override)
    //--------------------
    void resizeEvent(QResizeEvent *e) override;


signals:

    // From Middle
    //--------------------

    // Cursors
    //----------
    void forwardIntervalData(int, int);
    void forwardDataCursor(int, int, QString);


    // Pages
    //----------
    void forwardSamplePerPage(int);


    // From Top
    //--------------------
    void forwardKeyboardReleased();


    // Forwarding
    //--------------------

    // ----- To Middle -----

    // File name
    //----------
    void forwardFileName(QString);
    void forwardEventFileName(QString);


    // Pages
    //----------
    void forwardPageChange(QKeyEvent *e);
    void forwardPageChange(QString);
    void forwardPageToDisplay(int);


    // ----- To Top -----
    void forwardResetCursors();
    void forwardCursorStateChange();
    void forwardCursorIntervalInput(int, int);
};

#endif // WRAPPER_H
