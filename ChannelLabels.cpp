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
 * The manager of the channel labels displayer.
 *
 * Author: Thibaut Bertrand
 * thibaut.bertrand@inserm.fr
 ***************/


#include "ChannelLabels.h"


/***************
 * Constructor
 *-------------------------
 * QSize size: the size to set for the object.
 * ProjectData* project: the data container of interest for the class.
 ***************/
ChannelLabels::ChannelLabels(QSize size, ProjectData* project)
{
    this->resize(size);

    projectData = project;

    connect(this->projectData->presentationDataContainer, SIGNAL(notifyChange(size_t)),
            this, SLOT(onPresentationDataChanged(size_t)));

    connect(this->projectData->plotDataContainer, SIGNAL(notifyChange(size_t)),
            this, SLOT(onPlotDataChanged(size_t)));


    displayer = new ChannelLabelsDisplayer(size);

    QVBoxLayout *layout = new QVBoxLayout;
    this->setLayout(layout);
    layout->addWidget(displayer);
}


/***************
 * Destructor
 ***************/
ChannelLabels::~ChannelLabels()
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
void ChannelLabels::onPresentationDataChanged(size_t hint)
{
    if (hint & PresentationData::eHintHorizontalAxesChanged)
    {
        displayer->updateChannelLabels(projectData->presentationDataContainer->axesLabels,
                                       projectData->presentationDataContainer->axesPositions);
    }
}


/***************
 * Receive info from the data container and filter the hints to process only the
 * one of interest
 *-------------------------
 * size_t hint: flag that indicate specific changes in the data container. Allow
 * the method to know if it should take the signal into account or not.
 ***************/
void ChannelLabels::onPlotDataChanged(size_t hint)
{
    if (hint & PlotData::eHintAmpliAtChannelChanged)
    {
        QVector<QString> amplitudes;

        if (!projectData->plotDataContainer->amplitudesAtFirstCursor.isEmpty())
        {
            for (int i_chan = 0; i_chan < projectData->plotDataContainer->amplitudesAtFirstCursor.length(); i_chan++)
            {
                if (projectData->plotDataContainer->amplitudesAtFirstCursor[i_chan] == -1.)
                {
                    amplitudes.append(" ");
                }

                else
                {
                    amplitudes.append(QString::number(projectData->plotDataContainer->amplitudesAtFirstCursor[i_chan]));
                }
            }
        }

        displayer->updateChannelAmplitudes(amplitudes);
    }
}
