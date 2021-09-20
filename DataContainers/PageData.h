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
 * This class is contains data about the time scale, the page number and the
 * actually displayed page.
 *
 * Author: Thibaut Bertrand
 * thibaut.bertrand@inserm.fr
 ***************/


#ifndef PAGEDATA_H
#define PAGEDATA_H


#include <QDebug>
#include <QKeyEvent>


#include "qwidget.h"


class PageData : public QWidget
{
    Q_OBJECT


private:

    // Methods
    //--------------------
    void setTimeFirstSample();


    // Members
    //--------------------
    int defaultTimeScale = 10000; // In milliseconds = 10 seconds


public:

    PageData();


    // Methods
    //--------------------

    // --- Pages
    void pageChange(int newPage);
    void pageChange(QKeyEvent* e);
    void setTotalPageNb(QVector<int> freqList, QVector<int> sampList);

    // --- Time scale
    void incrementTimeScale(int increment);
    void setDefaultTimeScale();
    void setTimeScale(int newTimeScale);


    // Members
    //--------------------

    // --- Time scale
    int minimumTimeScale = 100; // The minimum value the timeScale can have.
    int timeScale = defaultTimeScale; // In milliseconds. The time that is represented by the currently displayed page.


    // --- View mode
    bool isViewFocusedOnEvent = false;

    // Page (classic)
    int actualPage = 1;
    int totalPageNb = 0;
    int timeAtFirstSample = 0; // In miiliseconds. The time that correspond to the first sample of the actually displayed page


    // Event focused
    QVector<QVector<int>> browsableEvents; // Sample | Event ID | File ID
    int currentEventFocusIndex = 0;
    QVector<double> eegFileSamplingFrequency;


    // ===== Hint structure =====
    using Hint = enum size_t
    {
        eHintNothingChanged = 0,
        eHintPageChanged = 1 << 0,
        eHintTimeScaleChanged = 1 << 1,
        eHintPageNumberChanged = 1 << 2,
    };


signals:

    void notifyChange(size_t hint);
};

#endif // PAGEDATA_H
