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
 * This widget is used as a base for the event catcher.
 *
 * Author: Thibaut Bertrand
 * thibaut.bertrand@inserm.fr
 ***************/


#include "CentralTopLayer.h"


/***************
 * Constructor
 *-------------------------
 * QSize size: the sizee of the central widget.
 * PresentationData presentation: the data container of interest for this class.
 ***************/
CentralTopLayer::CentralTopLayer(QSize size, PresentationData* presentation)
{
    setGeometry(0, 0, size.width(), size.height());


    //==============================
    // Create an invisible overlay on top of the label of the CentralMiddleLayer
    // Will catch and manage mouse events
    //==============================
    eventLayer = new EventCatcherTopLayer(presentation);

    QVBoxLayout* plotLayer = new QVBoxLayout;

    plotLayer->addWidget(eventLayer);
    plotLayer->setMargin(0);
    plotLayer->setSpacing(0);


    //==============================
    // Connection hub
    //==============================

    // SIGNALs coming from eventLayer (forwarding)
    //--------------------
    connect(this->eventLayer, SIGNAL(sendCursorPosition(int, int)),
            this, SIGNAL(forwardCursorPosition(int, int)));


    // SIGNALs going to eventLayer (forwarding)
    //--------------------
    connect(this, SIGNAL(forwardCursorStateChange()),
            this->eventLayer, SLOT(CursorStateChange()));

    connect(this, SIGNAL(forwardCursorIntervalInput(int, int)),
            this->eventLayer, SLOT(CursorIntervalInput(int, int)));


    setLayout(plotLayer);
}


/***************
 * Destructor
 ***************/
CentralTopLayer::~CentralTopLayer()
{

}
