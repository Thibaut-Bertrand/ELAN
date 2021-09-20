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
 * This widget manage and display the background of the Central Widget.
 *
 * Author: Thibaut Bertrand
 * thibaut.bertrand@inserm.fr
***************/


#include "CentralBottomLayer.h"


/***************
 * Constructor
 *-------------------------
 * QSize size: the size of the central widget.
 * PresentationData presentation: the dataContainer of interest for this class.
***************/
CentralBottomLayer::CentralBottomLayer(QSize size,
                                       PresentationData* presentation)
{
    setGeometry(0, 0, size.width(), size.height());


    presentationDataContainer = presentation;

    connect(this->presentationDataContainer, SIGNAL(notifyChange(size_t)),
            this, SLOT(onDataContainerChanged(size_t)));


    // Set background color
    //--------------------
    palette.setColor(QPalette::Background,
                     presentationDataContainer->backgroundColor);
    setAutoFillBackground(true);
    setPalette(palette);
}


/***************
 * Destructor
***************/
CentralBottomLayer::~CentralBottomLayer()
{

}


//      %%%%%%%%%%%%%%%%%%%%%%%%%%
//      %%%% METHODS override %%%%
//      %%%%%%%%%%%%%%%%%%%%%%%%%%


/***************
 * This paint a dashed line at regular interval.
 *-------------------------
 * QPaintEvent e:
 ***************/
void CentralBottomLayer::paintEvent(QPaintEvent *e)
{
    QWidget::paintEvent(e);
    QPainter painter(this);
    painter.setPen(QPen(presentationDataContainer->intervalBrush, 1,
                        Qt::DashLine));
    // TODO: If needed, make the pen as a member of the data container, so that the user can change more things about it (style, size...)


    // Intervals
    //--------------------
    int interval = this->width() / presentationDataContainer->intervalNb;

    for (int i = 1; i < 10; i++) // 9 lines = 10 intervals
    {
        painter.drawLine(interval * i, 0,
                         interval * i, this->height()); // x1, y1, x2, y2
    }


    // Horizontal axes
    //--------------------
    if (presentationDataContainer->areHorizontalAxesShown
            && !presentationDataContainer->axesPositions.isEmpty())
    {
        for (int i_chan = 0; i_chan < presentationDataContainer->axesPositions.length(); i_chan++)
        {
            painter.drawLine(0, presentationDataContainer->axesPositions[i_chan],
                             this->width(), presentationDataContainer->axesPositions[i_chan]);
        }
    }
}


//      %%%%%%%%%%%%%%%%%%%%%%%%
//      %%%% METHODS public %%%%
//      %%%%%%%%%%%%%%%%%%%%%%%%


/***************
 * Change the background color on the user request
 ***************/
void CentralBottomLayer::updateBackground()
{
    palette.setColor(QPalette::Background,
                     presentationDataContainer->backgroundColor);
    setAutoFillBackground(true);
    setPalette(palette);
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
void CentralBottomLayer::onDataContainerChanged(size_t hint)
{
    if (hint & PresentationData::eHintBackgroundChanged)
    {
        updateBackground();
    }

    else if (hint & PresentationData::eHintHorizontalAxesChanged
             || hint & PresentationData::eHintIntervalChanged)
    {
        repaint();
    }
}
