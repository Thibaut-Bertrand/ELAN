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
 * This widget catch and manage input coming from the user, notably the
 * interactions withe the cursors.
 *
 * Author: Thibaut Bertrand
 * thibaut.bertrand@inserm.fr
 ***************/


#ifndef EVENTCATCHERTOPLAYER_H
#define EVENTCATCHERTOPLAYER_H


#include <QWidget>
#include <QDebug>
#include <QMouseEvent>
#include <QVector>
#include <QRubberBand>


#include "qpainter.h"
#include "DataContainers/PresentationData.h"


class EventCatcherTopLayer : public QWidget
{
    Q_OBJECT


private:

    // Methods
    //--------------------
    void intervalHighlight(double start, double end);
    void setCursorPositon();


    // Members
    //--------------------
    bool cursorOneDisplayed = false; // if true: cursorOne has been placed
    bool cursorTwoDisplayed = false;

    QPainter p1;
    QPainter p2;

    QPoint dragOrigin;
    QRubberBand* dragRubberBand;

    PresentationData* presentationData;


public:

    EventCatcherTopLayer(PresentationData* presentation);
    ~EventCatcherTopLayer();


    // Methods (override)
    //--------------------
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void paintEvent(QPaintEvent *e);
    void resizeEvent(QResizeEvent *e);


signals:

    void sendCursorPosition(int x, int y);


public slots:

    void CursorIntervalInput(int interval, int timeScale);
    void CursorStateChange();

    void onPresentationDataChanged(size_t hint);
};

#endif // EVENTCATCHERTOPLAYER_H
