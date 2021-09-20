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
 * Display the events from the loaded .pos files. Accepted events will be in the
 * same color as their associated .eeg file. Rejected events will be in red
 * color.
 *
 * Author: Thibaut Bertrand
 * thibaut.bertrand@inserm.fr
 ***************/


#include "CentralMiddleLayerEvents.h"


/***************
 * Constructor
 *-------------------------
 * QSize size: the size of the object.
 * ProjectData* project: the dataContainer of interest for this class.
 ***************/
CentralMiddleLayerEvents::CentralMiddleLayerEvents(QSize size,
                                                   ProjectData* project)
{
    setGeometry(0, 0, size.width(), size.height());

    projectData = project;

    connect(projectData->presentationDataContainer, SIGNAL(notifyChange(size_t)),
            this, SLOT(onPresentationDataChanged(size_t)));
}


/***************
 * Destructor
 ***************/
CentralMiddleLayerEvents::~CentralMiddleLayerEvents()
{

}


//      %%%%%%%%%%%%%%%%%%%%%%%%%%
//      %%%% METHODS override %%%%
//      %%%%%%%%%%%%%%%%%%%%%%%%%%


/***************
 * Overide of the paintEvent method to manage the display of events.
 * cf Qt documentation.
 ***************/
void CentralMiddleLayerEvents::paintEvent(QPaintEvent *e)
{
    // eventInRange:
    // Sample | Accept/Reject


    QWidget::paintEvent(e);
    QPainter event(this);

    double  eventRelativePosition = 0.;
    int eventPositionX = 0;

    for (int i_file = 0; i_file < projectData->plotDataContainer->eegFileNb; i_file++)
    {
        if (projectData->plotDataContainer->eegDataStorage[i_file]->hasEventLoaded)
        {
            for (int j_event = 0; j_event < projectData->plotDataContainer->eegDataStorage[i_file]->eventToDisplay.length(); j_event++)
            {
                eventRelativePosition = abs(projectData->plotDataContainer->eegDataStorage[i_file]->eventToDisplay[j_event].sample - projectData->plotDataContainer->eegDataStorage[i_file]->currentLowerSample);

                eventPositionX = (int)( ( (eventRelativePosition * this->width() / projectData->plotDataContainer->eegDataStorage[i_file]->samplePerPage) ) );


                // If the event is accepted -> plot color
                // If the event is rejected -> RED
                if (projectData->plotDataContainer->eegDataStorage[i_file]->eventToDisplay[j_event].acceptState == 0)
                {
                    event.setPen(QPen(projectData->presentationDataContainer->plotColors[i_file], 1, Qt::SolidLine));
                    event.drawLine(eventPositionX, 0,
                                   eventPositionX, this->height());
                }

                else
                {
                    event.setPen(QPen(projectData->presentationDataContainer->rejectedEventBrush, 1, Qt::SolidLine));
                    event.drawLine(eventPositionX, 0,
                                   eventPositionX, this->height());
                }
            }
        }
    }
}


/***************
 * cf Qt documentation.
 ***************/
void CentralMiddleLayerEvents::resizeEvent(QResizeEvent *e)
{

}


//      %%%%%%%%%%%%%%%%%%%%%%%%
//      %%%% METHODS public %%%%
//      %%%%%%%%%%%%%%%%%%%%%%%%


/***************
 * It's empty but I left it in case other instructions have to be added before
 * the repaint() later. Don't know if I can ask for a repaint outside of the
 * class anyway...
 ***************/
void CentralMiddleLayerEvents::displayEvents()
{
    repaint();
}


//      %%%%%%%%%%%%%%%%%%%%%%%
//      %%%% SLOTS private %%%%
//      %%%%%%%%%%%%%%%%%%%%%%%


/***************
 * Receive info from the data container and filter the hints to process only the
 * one of interest
 *-------------------------
 * size_t hint: flag that indicate specific changes in the data container. Allow
 * the method to know if it should take the signal into account or not.
 ***************/
void CentralMiddleLayerEvents::onPresentationDataChanged(size_t hint)
{
    if (hint & PresentationData::eHintPlotsChanged)
    {
        repaint();
    }
}
