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


#include "PresentationData.h"


/***************
 * Constructor
 ***************/
PresentationData::PresentationData()
{

}


//      %%%%%%%%%%%%%%%%%%%%%%%%
//      %%%% METHODS public %%%%
//      %%%%%%%%%%%%%%%%%%%%%%%%


//      ====================
//      %%%% Background %%%%
//      ====================


/***************
 * Change the color of the background.
 *-------------------------
 * QColor color: the new color to apply.
 ***************/
void PresentationData::setBackgroundColor(QColor color)
{
    backgroundColor = color;

    emit notifyChange(eHintBackgroundChanged); // -> CentralBottomLayer
}


/***************
 * Change the color of the background to its default value.
 ***************/
void PresentationData::setDefaultBackgroundColor()
{
    backgroundColor = defaultBackgroundColor;

    emit notifyChange(eHintBackgroundChanged); // -> CentralBottomLayer
}


//      =================
//      %%%% Cursors %%%%
//      =================


/***************
 * Enable/disable the butterfly plot displaying.
 ***************/
void PresentationData::changeCursorIntervalState()
{
    isCursorIntervalShown = !isCursorIntervalShown;

    emit notifyChange(eHintIntervalChanged); // -> EventCatcherTopLayer
}


/***************
 * Just... hide the cursors...
 * Called when the user click on "Rest", in the cursors part of the MainWindow.
 ***************/
void PresentationData::hideCursors()
{
    cursorOneColor.setAlpha(0);
    cursorTwoColor.setAlpha(0);

    setCursorRelativePosition(-1., -1.);
}


/***************
 * Set the cursors' brushes to the one indicated.
 *-------------------------
 * QColor colorOne: the first cursor new color.
 * QColor colorTwo: the second cursor new color.
 ***************/
void PresentationData::setCursorColor(QColor colorOne, QColor colorTwo)
{
    cursorOneColor = colorOne;
    cursorTwoColor = colorTwo;

    emit notifyChange(eHintCursorsChanged);
}


/***************
 * Set the cursor brushes to their default value.
 ***************/
void PresentationData::setCursorDefaultColor()
{
    cursorOneColor= defaultCursorOneColor;
    cursorTwoColor = defaultCursorTwoColor;

    emit notifyChange(eHintCursorsChanged);
}


/***************
 * Set the new relative position of the cursors. Relative position are in
 * percentage of the window' width, while the absolute position (refered as
 * positionX in other classes), is the absolute position in pixels.
 *-------------------------
 * double positionOne: 1 = first ; 2 = second.
 * double positionTwo: the new cursor position, in percentage of the window'
 * width.
 ***************/
void PresentationData::setCursorRelativePosition(double positionOne,
                                                 double positionTwo)
{
    if (positionOne <= 0. && positionTwo <= 0)
    {
        isCursorOneDisplayed = false;
        isCursorTwoDisplayed = false;
        positionOne = 0.;
        positionTwo = 0.;
        cursorOneColor.setAlpha(0);
        cursorTwoColor.setAlpha(0);
    }

    if (positionOne > 0.)
    {
        isCursorOneDisplayed = true;
        cursorOneColor.setAlpha(255);
    }

    if (positionTwo > 0.)
    {
        isCursorTwoDisplayed = true;
        cursorTwoColor.setAlpha(255);
    }

    cursorOneRelativePosition = positionOne;
    cursorTwoRelativePosition = positionTwo;

    emit notifyChange(eHintCursorsChanged); // -> EventCatcherTopLayer
}


/***************
 * Change the cursor state from left/right click to drag and drop, and
 * vice-versa.
 *-------------------------
 * bool state: the cursor state to apply (false = both click ; true = drag
 * and drop.
 ***************/
void PresentationData::setCursorState(bool state)
{
    dragAndDrop = state;
}


/***************
 * Set the default color of the cursor interval.
 ***************/
void PresentationData::setCursorIntervalDefaultColor()
{
    cursorIntervalColor = defaultCursorIntervalColor;

    emit notifyChange(eHintIntervalChanged); // -> EventCatcherTopLayer
}


/***************
 * Set the cursor interval color to the new specified one.
 * Note: to allow the user to see through the interval, if the color alpha is
 * lower than 75%, it's automatically set to this value.
 *-------------------------
 * QColor color: the new color to apply to the interval.
 ***************/
void PresentationData::setCursorIntervalColor(QColor color)
{
    cursorIntervalColor = color;

    // To be sure the user can see through
    if (cursorIntervalColor.alpha() > 75)
    {
        cursorIntervalColor.setAlpha(75);
    }

    emit notifyChange(eHintIntervalChanged); // -> EventCatcherTopLayer
}


//      =========================
//      %%%% Horizontal axis %%%%
//      =========================


/***************
 * Change the visibility state of the plot x-axes.
 *-------------------------
 * bool state: the new x-axes state.
 ***************/
void PresentationData::setAxesState(bool state)
{
    areHorizontalAxesShown = state;

    emit notifyChange(eHintHorizontalAxesChanged); // -> CentralBottomLayer && ChannelLabels
}


/***************
 * Set the axes position of each unique plots from the CentralMiddleLayer (free
 * channels and butterfly plots). Those are the x-axes of the plots.
 *-------------------------
 * QVector<double> positions: simply the butterflu plot positions given by the
 * CentralMiddleLayer.
 * QVector<QString> labels: the label list corresponding to the positions.
 ***************/
void PresentationData::setButterflyAxesPosition(QVector<double> positions,
                                                QVector<QString> labels)
{
    axesPositions.clear();
    axesLabels.clear();

    axesPositions = positions;
    axesLabels = labels;

    emit notifyChange(eHintHorizontalAxesChanged); // -> CentralBottomLayer && ChannelLabels
}


/***************
 * Set the axes position of each unique plots from the CentralMiddleLayer (free
 * channels and butterfly plots). Those are the x-axes of the plots.
 *-------------------------
 * QVector<double> positions: simply the free channel positions given by the
 * CentralMiddleLayer.
 * QVector<QString> labels: the label list corresponding to the positions.
 * QVector<bool> isDisplayed: whether or not the channel is displayed.
 ***************/
void PresentationData::setFreeChannelsAxesPosition(QVector<double> positions,
                                                   QVector<QString> labels,
                                                   QVector<bool> isDisplayed)
{
    axesPositions.clear();
    axesLabels.clear();

    axesPositions = positions;

    for (int i_chan = 0; i_chan < isDisplayed.length(); i_chan++)
    {
        if (isDisplayed[i_chan])
        {
            axesLabels.append(labels[i_chan]);
        }
    }

    emit notifyChange(eHintHorizontalAxesChanged); // -> CentralBottomLayer && ChannelLabels
}


//      ==================
//      %%%% Interval %%%%
//      ==================


/***************
 * Change the color of interval' brush.
 *-------------------------
 * QColor color: the new color to apply.
 ***************/
void PresentationData::setIntervalColor(QColor color)
{
    intervalBrush.setColor(color);

    emit notifyChange(eHintIntervalChanged); // -> CentralBottomLayer
}


/***************
 * Change the interval number to be displayed.
 *-------------------------
 * int nb: the new interval number to display.
 ***************/
void PresentationData::setIntervalNb(int nb)
{
    intervalNb = nb;

    emit notifyChange(eHintIntervalChanged); // -> CentralBottomLayer
}


//      ===============
//      %%%% Plots %%%%
//      ===============


/***************
 * Set the plot colors to their default values.
 ***************/
void PresentationData::setDefaultPlotColors()
{
    singleFileColor = defaultSingleFileColor;
    plotColors = defaultMultipleFileColors;

    emit notifyChange(eHintPlotsChanged); // -> CentralMiddleLayerDisplayer && -> CentralMiddleLayerEvents
}


/***************
 * Set the plot colors to the specified values.
 *-------------------------
 * QColor singlePlotColor: the color to apply to the plot when only one file is
 * loaded.
 * QVector<QColor> multiplePlotColor: the color to apply to the plots when
 * multiple files are loaded.
 ***************/
void PresentationData::setPlotColors(QColor singlePlotColor,
                                     QVector<QColor> multiplePlotColors)
{
    singleFileColor = singlePlotColor;
    plotColors = multiplePlotColors;

    emit notifyChange(eHintPlotsChanged); // -> CentralMiddleLayerDisplayer && -> CentralMiddleLayerEvents
}


void PresentationData::changeTicksState()
{
    areTicksDisplayed = !areTicksDisplayed;

    emit notifyChange(eHintTicksChanged);
}
