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
 * This class manage the displaying of event code and of time ticks on top of
 * the plot frame. The ticks are positionned on a regular interval, and each
 * event present in the plot frame will have its code displayed.
 *
 * Author: Thibaut Bertrand
 * thibaut.bertrand@inserm.fr
 ***************/


#include "EventLabels.h"


/***************
 * Constructor
 ***************/
EventLabels::EventLabels(QSize size, ProjectData* project)
{
    this->resize(size);


    projectData = project;

    connect(this->projectData, SIGNAL(notifyChange(size_t)),
            this, SLOT(onProjectDataChanged(size_t)));

    connect(this->projectData->presentationDataContainer, SIGNAL(notifyChange(size_t)),
            this, SLOT(onPresentationDataChanged(size_t)));


    eventDisplayer = new EventLabelsDisplayer(size, projectData);

    connect(this, SIGNAL(forwardDisplayEventCode(QVector<QVector<QVector<int>>>)),
                this->eventDisplayer, SLOT(newEvents(QVector<QVector<QVector<int>>>)));


    // Set the eventDisplayer as a widget of the layout so that it can take into account the resizing event.
    QHBoxLayout *layout = new QHBoxLayout;
    this->setLayout(layout);
    layout->setMargin(0);
    layout->addWidget(eventDisplayer);
}


/***************
 * Destructor
 ***************/
EventLabels::~EventLabels()
{

}


//      %%%%%%%%%%%%%%%%%%%%%%
//      %%%% SLOTS public %%%%
//      %%%%%%%%%%%%%%%%%%%%%%


/***************
 * Receive info from the data container and filter the hints to process only the
 * one of interest
 *-------------------------
 * size_t hint: flag that indicate specific changes in the data container. Allow
 * the method to know if it should take the signal into account or not.
 ***************/
void EventLabels::onPresentationDataChanged(size_t hint)
{
    if (hint & PresentationData::eHintTicksChanged)
    {
        eventDisplayer->repaint();
    }
}


/***************
 * Receive info from the data container and filter the hints to process only the
 * one of interest
 *-------------------------
 * size_t hint: flag that indicate specific changes in the data container. Allow
 * the method to know if it should take the signal into account or not.
 ***************/
void EventLabels::onProjectDataChanged(size_t hint)
{
    if (hint & ProjectData::eHintPageChanged)
    {
        eventDisplayer->repaint();
    }
}
