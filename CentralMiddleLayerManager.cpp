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


#include "CentralMiddleLayerManager.h"


/***************
 * Constructor
 *-------------------------
 * QSize size: the size of the window.
 * ProjectData* project: the data container of interest for the class.
 ***************/
CentralMiddleLayerManager::CentralMiddleLayerManager(QSize size,
                                                     ProjectData* project)
{
    projectData = project;

    connect(this->projectData, SIGNAL(notifyChange(size_t)),
            this, SLOT(onProjectDataChanged(size_t)));

    connect(this->projectData->plotDataContainer, SIGNAL(notifyChange(size_t)),
            this, SLOT(onPlotDataChanged(size_t)));


    // Plots Displayer
    //--------------------
    displayer = new CentralMiddleLayerDisplayer(size, projectData);


    connect(this, SIGNAL(forwardCursorPosition(int, int)),
            this->displayer, SLOT(displayPlotInfo(int, int)));

    displayer->setParent(this);


    // Events Displayer
    //--------------------
    eventsDisplayer = new CentralMiddleLayerEvents(size, projectData);

    eventsDisplayer->setParent(this);
}


//      %%%%%%%%%%%%%%%%%%%%%%%%%%
//      %%%% METHODS override %%%%
//      %%%%%%%%%%%%%%%%%%%%%%%%%%


/***************
 * cf Qt documentation.
 ***************/
void CentralMiddleLayerManager::resizeEvent(QResizeEvent *e)
{
    QWidget::resizeEvent(e);

    int oldWidth = e->oldSize().width();

    if (oldWidth >= 0)
    {
        displayer->resize(displayer->width() + e->size().width() - oldWidth,
                        displayer->height() + e->size().height() - e->oldSize().height());

        eventsDisplayer->resize(eventsDisplayer->width() + e->size().width() - oldWidth,
                        eventsDisplayer->height() + e->size().height() - e->oldSize().height());
    }
}


//      %%%%%%%%%%%%%%%
//      %%%% SLOTS %%%%
//      %%%%%%%%%%%%%%%


/***************
 * Receive info from the data container and filter the hints to process only the
 * one of interest
 *-------------------------
 * size_t hint: flag that indicate specific changes in the data container. Allow
 * the method to know if it should take the signal into account or not.
 ***************/
void CentralMiddleLayerManager::onProjectDataChanged(size_t hint)
{
    if (hint & ProjectData::eHintEegFileChanged)
    {
        displayer->setXAxis();
        displayer->setGraphArrangement();
        displayer->setGraphData();
    }

    if (hint & ProjectData::eHintPageChanged)
    {
        displayer->setGraphData();

        eventsDisplayer->displayEvents();
    }

    if (hint & ProjectData::eHintTimeScaleChanged)
    {
        displayer->setXAxis();
        displayer->setGraphArrangement();
    }

    if (hint & ProjectData::eHintAmplitudeScalesChanged)
    {
        displayer->setGraphArrangement();
    }

    if (hint & ProjectData::eHintEegFileUnloaded)
    {
        // Update display
        displayer->setXAxis();
        displayer->setGraphArrangement();
        displayer->setGraphData();
    }
}


/***************
 * Receive info from the data container and filter the hints to process only the
 * one of interest
 *-------------------------
 * size_t hint: flag that indicate specific changes in the data container. Allow
 * the method to know if it should take the signal into account or not.
 ***************/
void CentralMiddleLayerManager::onPlotDataChanged(size_t hint)
{
    if (hint & PlotData::eHintChannelsChanged)
    {
        displayer->setGraphArrangement();
        displayer->setGraphData();
    }

    if (hint & PlotData::eHintEventStateChanged)
    {
        eventsDisplayer->displayEvents();
    }

    if (hint & PlotData::eHintButterflyDisplayChanged)
    {
        displayer->setGraphArrangement();
        displayer->setGraphData();
    }
}
