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
 * Custom Widget with an overriden paintEvent method to display the event codes.
 *
 * Author: Thibaut Bertrand
 * thibaut.bertrand@inserm.fr
 ***************/


#include "EventLabelsDisplayer.h"


/***************
 * Constructor
 *-------------------------
 * ProjectData* project: the data container of interest for the class.
 ***************/
EventLabelsDisplayer::EventLabelsDisplayer(QSize size, ProjectData* project)
{
    this->resize(size);

    projectData = project;

    connect(projectData->presentationDataContainer, SIGNAL(notifyChange(size_t)),
            this, SLOT(onPresentationDataChanged(size_t)));
}


/***************
 * Destructor
 ***************/
EventLabelsDisplayer::~EventLabelsDisplayer()
{

}


//      %%%%%%%%%%%%%%%%%%%%%%%%%%
//      %%%% METHODS override %%%%
//      %%%%%%%%%%%%%%%%%%%%%%%%%%


/***************
 * Display the event codes on top of the corresponding line.
 *-------------------------
 * QPaintEvent *e: cf Qt documentation.
 ***************/
void EventLabelsDisplayer::paintEvent(QPaintEvent *e)
{
    QWidget::paintEvent(e);

    double eventRelativePosition = 0.;
    int eventPositionX = 0;
    int eventPositionY = 0;
    QString label;


    for (int i_file = 0; i_file < projectData->plotDataContainer->eegFileNb; i_file++)
    {
        if (projectData->plotDataContainer->eegDataStorage[i_file]->hasEventLoaded)
        {
            QPainter painter(this); // This one's for normal events.
            QPainter boldPainter(this); // This one's only for the focused event (when in event focused view). Can't apply different fonts to the same painter.

            painter.setPen(projectData->presentationDataContainer->plotColors[i_file]);
            painter.setFont(QFont("Arial", 9));

            boldPainter.setPen(projectData->presentationDataContainer->plotColors[i_file]);
            boldPainter.setFont(QFont("Arial", 11, QFont::Bold));

            for (int j_event = 0; j_event < projectData->plotDataContainer->eegDataStorage[i_file]->eventToDisplay.length(); j_event++)
            {
                eventRelativePosition = abs(projectData->plotDataContainer->eegDataStorage[i_file]->eventToDisplay[j_event].sample - projectData->plotDataContainer->eegDataStorage[i_file]->currentLowerSample);

                eventPositionX = (int)( ( (eventRelativePosition * this->width() / projectData->plotDataContainer->eegDataStorage[i_file]->samplePerPage) ) - 10);

                eventPositionY = (i_file + 1) * 10;

                label = QString::number(projectData->plotDataContainer->eegDataStorage[i_file]->eventToDisplay[j_event].code);

                if (eventRelativePosition == 5000
                        && projectData->pageDataContainer->isViewFocusedOnEvent)
                {
                    boldPainter.drawText(eventPositionX, eventPositionY, label);
                }

                else
                {
                    painter.drawText(eventPositionX, eventPositionY, label);
                }
            }
        }
    }


    if (projectData->presentationDataContainer->areTicksDisplayed)
    {
        int timeScale = projectData->pageDataContainer->timeScale;
        int graduationSeparation = projectData->presentationDataContainer->graduationSeparation;
        int tickNb = timeScale / graduationSeparation;

        QPainter painterTick(this);
        for (int i_tick = 0; i_tick < tickNb; i_tick++)
        {
            if (i_tick % 10 == 0)
            {
                painterTick.drawLine(((graduationSeparation * i_tick) * this->size().width() ) / timeScale, this->height() - 10,
                                 ((graduationSeparation * i_tick) * this->size().width() ) / timeScale, this->height());
            }

            else
            {
                painterTick.drawLine(((graduationSeparation * i_tick) * this->size().width() ) / timeScale, this->height() - 5,
                                 ((graduationSeparation * i_tick) * this->size().width() ) / timeScale, this->height());
            }
        }
    }
}


//      %%%%%%%%%%%%%%%%%%%%%%
//      %%%% SLOTS public %%%%
//      %%%%%%%%%%%%%%%%%%%%%%


/***************
 * Get and display new events
 *-------------------------
 * QVector<QVector<QVector<int>>> eventList: the list of event (sample and code)
 * to display for each active event file.
 ***************/
void EventLabelsDisplayer::newEvents(QVector<QVector<QVector<int>>> eventList)
{
    // eventList:
    // Event Sample | Event Code


    eventToDisplay.clear();
    eventToDisplay.resize(eventList.size());

    for (int i_file = 0; i_file < eventList.length(); i_file++)
    {
        eventToDisplay[i_file].resize(eventList[i_file].length());

        for (int j_event = 0; j_event < eventList[i_file].length(); j_event++)
        {
            eventToDisplay[i_file][j_event].resize(2);

            eventToDisplay[i_file][j_event][0] = eventList[i_file][j_event][0];
            eventToDisplay[i_file][j_event][1] = eventList[i_file][j_event][1];
        }
    }

    repaint();
}


/***************
 * Receive info from the data container and filter the hints to process only the
 * one of interest
 *-------------------------
 * size_t hint: flag that indicate specific changes in the data container. Allow
 * the method to know if it should take the signal into account or not.
 ***************/
void EventLabelsDisplayer::onPresentationDataChanged(size_t hint)
{
    if (hint & PresentationData::eHintPlotsChanged)
    {
        repaint();
    }
}
