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
 * This class is contains data about plot colors, themes, and other aesthetic
 * variables.
 *
 *
 * Author: Thibaut Bertrand
 * thibaut.bertrand@inserm.fr
 ***************/


#ifndef PRESENTATIONDATA_H
#define PRESENTATIONDATA_H


#include <QDebug>


#include "qwidget.h"


class PresentationData : public QWidget
{
    Q_OBJECT


private:

    // Members
    //--------------------

    // --- Background
    QColor defaultBackgroundColor = QColor(224, 224, 224);


    // --- Cursors
    QColor defaultCursorOneColor = QColor(0, 154, 207, 0);
    QColor defaultCursorTwoColor = QColor(255, 102, 102, 0);
    QColor defaultCursorIntervalColor = QColor(0, 155, 240, 75);


    // --- Plots
    QColor defaultSingleFileColor = QColor(0, 0, 0);
    QVector<QColor> defaultMultipleFileColors{QColor(143, 204, 0),
                QColor(0, 200, 200), QColor(0, 0, 200), QColor(200, 0, 200),
                QColor(200, 200, 0), QColor(0, 200, 0), QColor(0, 100, 200),
                QColor(140, 0, 200)}; // link to create basic color gradient: https://coolors.co/


public:

    PresentationData();


    // Methods
    //--------------------
    // --- Background
    void setBackgroundColor(QColor color);
    void setDefaultBackgroundColor();


    // --- Cursors
    void changeCursorIntervalState();
    void hideCursors();
    void setCursorColor(QColor colorOne, QColor colorTwo);
    void setCursorDefaultColor();
    void setCursorRelativePosition(double positionOne, double positionTwo);
    void setCursorState(bool state);
    void setCursorIntervalColor(QColor color);
    void setCursorIntervalDefaultColor();


    // --- Horizontal axis
    void setAxesState(bool state);
    void setButterflyAxesPosition(QVector<double> positions,
                                  QVector<QString> labels);
    void setFreeChannelsAxesPosition(QVector<double> positions,
                                     QVector<QString> labels,
                                     QVector<bool> isDisplayed);


    // --- Interval
    void setIntervalColor(QColor color);
    void setIntervalNb(int nb);


    // --- Plots
    void setDefaultPlotColors();
    void setPlotColors(QColor singlePlotColor,
                       QVector<QColor> multiplePlotColors);


    // --- Ticks
    void changeTicksState();


    // Members
    //--------------------
    // --- Background
    QColor backgroundColor = defaultBackgroundColor;


    // --- Cursors
    bool dragAndDrop = false; // Cursors can either use left/right-click, or use only the left click for a drag and drop
    bool isCtrlPressed = false; // allow to display plot information next to the cursor when ctrl is pressed (and hold)

    double cursorOneRelativePosition = 0.;
    double cursorTwoRelativePosition = 0.;

    bool isCursorOneDisplayed = false;
    bool isCursorTwoDisplayed = false;
    bool isCursorIntervalShown = true;

    QColor cursorOneColor = defaultCursorOneColor;
    QColor cursorTwoColor = defaultCursorTwoColor;

    QColor cursorIntervalColor = defaultCursorIntervalColor;


    // --- Events
    bool areEventDisplayed = true;
    QBrush rejectedEventBrush = QColor(255, 0, 0, 255);


    // --- Fonts
    QFont smallTextFont = QFont("Arial", 7);
    QFont normalTextFont = QFont("Arial", 10);
    QFont bigTextFont = QFont("Arial", 14);


    // --- Horizontal axes
    bool areHorizontalAxesShown = true; // The x-axis of the plots

    QVector<double> axesPositions; // The position, in the CentralMiddleLayer
    QVector<QString> axesLabels; // The labels corresponding to the above position


    // --- Interval
    int intervalNb = 10; // Nb of interval that divide the CentralBottomLayer
    QBrush intervalBrush = QBrush(QColor(155, 155, 155, 255));


    // --- Plots
    QColor singleFileColor = defaultSingleFileColor;
    QVector<QColor> plotColors = defaultMultipleFileColors;


    /*
     * PROTOTYPE COLORS (didn"t find a good algorithm to generate color for a
     * diverging scale while keeping them distinct, with a smooth transition,
     * colorblind-proof... I get them here:
     * https://vis4.net/palettes/#/150|d|00429d,96ffea,ffffe0|ffffe0,ff005e,93003a|1|1)
     */
    QVector<QString> btfColorList{"#00429d", "#09449e", "#10479f", "#1649a0",
                                  "#1b4ca2", "#1f4ea3", "#2351a4", "#2653a5",
                                  "#2a56a6", "#2d58a7", "#305ba9", "#335daa",
                                  "#3660ab", "#3862ac", "#3b65ad", "#3e67ae",
                                  "#406aaf", "#436db0", "#456fb2", "#4872b3",
                                  "#4a74b4", "#4d77b5", "#4f7ab6", "#517cb7",
                                  "#547fb8", "#5681b9", "#5884ba", "#5b87bb",
                                  "#5d89bc", "#5f8cbe", "#618fbf", "#6491c0",
                                  "#6694c1", "#6896c2", "#6b99c3", "#6d9cc4",
                                  "#6f9ec5", "#72a1c6", "#74a4c7", "#77a6c8",
                                  "#79a9c9", "#7bacca", "#7eafcb", "#80b1cc",
                                  "#83b4cd", "#85b7ce", "#88b9cf", "#8bbcd0",
                                  "#8dbfd1", "#90c1d1", "#93c4d2", "#95c7d3",
                                  "#98c9d4", "#9bccd5", "#9ecfd6", "#a1d1d7",
                                  "#a4d4d8", "#a7d7d8", "#aad9d9", "#addcda",
                                  "#b1dfdb", "#b4e1dc", "#b8e4dc", "#bbe6dd",
                                  "#bfe9de", "#c3ebde", "#c7eedf", "#cbf0df",
                                  "#d0f3e0", "#d4f5e0", "#daf7e1", "#dff9e1",
                                  "#e5fbe1", "#ecfde1", "#f4fee1", "#fffcdd",
                                  "#fff8db", "#fff5d8", "#fff1d5", "#ffeed3",
                                  "#ffead0", "#ffe7ce", "#ffe3cb", "#ffe0c8",
                                  "#ffdcc6", "#ffd9c3", "#ffd5c0", "#ffd1be",
                                  "#ffcebb", "#ffcab9", "#ffc7b6", "#ffc3b3",
                                  "#ffbfb1", "#ffbcae", "#ffb8ab", "#ffb4a9",
                                  "#ffb0a6", "#ffaca3", "#ffa9a1", "#ffa59e",
                                  "#ffa19b", "#ff9d98", "#ff9996", "#fe9593",
                                  "#fd9291", "#fc8e8e", "#fb8b8c", "#fa878a",
                                  "#f88487", "#f78085", "#f67d83", "#f47981",
                                  "#f3767e", "#f2727c", "#f06f7a", "#ee6b78",
                                  "#ed6875", "#eb6473", "#e96171", "#e75d6f",
                                  "#e65a6d", "#e4566b", "#e25369", "#e04f67",
                                  "#dd4c65", "#db4863", "#d94561", "#d7425f",
                                  "#d43e5d", "#d23b5b", "#d03759", "#cd3457",
                                  "#ca3155", "#c82d53", "#c52a52", "#c22650",
                                  "#bf234e", "#bc1f4c", "#b91c4b", "#b61849",
                                  "#b31547", "#b01246", "#ad0e44", "#a90a43",
                                  "#a60741", "#a20440", "#9e023e", "#9b013d",
                                  "#97003b", "#93003a"};


    // --- Ticks
    bool areTicksDisplayed = true;
    int tickSeparation = 10; // Nb of pixel that seperate tick on the top border of the CentralBottomLayer
    int graduationSeparation = 100; // In ms. The time between each graduation of the EventLabels class


    // ===== Hint structure =====
    using Hint = enum size_t
    {
        eHintNothingChanged = 0,
        eHintBackgroundChanged = 1 << 0,
        eHintCursorsChanged = 1 << 1,
        eHintEventsChanged = 1 << 2,
        eHintHorizontalAxesChanged = 1 << 3,
        eHintIntervalChanged = 1 << 4,
        eHintTicksChanged = 1 << 5,
        eHintPlotsChanged = 1 << 6,
    };


signals:

    void notifyChange(size_t hint);
};

#endif // PRESENTATIONDATA_H
