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
 * The wrapper contain multiple layers that are used to manage different aspects
 * of the central widget, allowing the display of the data.
 * The Bottom Layer manage the background.
 * The Middle Layer manage the data that are displayed.
 * The Top Layer manage the cursors and the event catcher.
 *
 * Author: Thibaut Bertrand
 * thibaut.bertrand@inserm.fr
 ***************/


#include "Wrapper.h"


/***************
 * Constructor
 ***************/
Wrapper::Wrapper(QSize size, ProjectData* projectData)
{
    //==============================
    // Create the 3 layers composing the MainWindow
    //==============================

    // CentralBottomLayer : Background
    //--------------------
    bottomLayer = new CentralBottomLayer(size,
                                         projectData->presentationDataContainer);
    bottomLayer->setParent(this);


    // CentralMiddleLayer : Plots and channel labels, events (data) display
    //--------------------
    middleLayer = new CentralMiddleLayerManager(size, projectData);

    QHBoxLayout *layout = new QHBoxLayout;
    this->setLayout(layout);
    layout->setMargin(0);
    layout->addWidget(middleLayer);


    // CentralBottomLayer : Events (inputs) catcher and cursors management
    //--------------------
    topLayer = new CentralTopLayer(size, projectData->presentationDataContainer);
    topLayer->setParent(this);


    //==============================
    // Connection hub
    //==============================

    // SIGNALs COMING from TOPLAYER
    //--------------------

    // Cursors
    //----------
    connect(this->topLayer, SIGNAL(forwardCursorPosition(int, int)),
            this->middleLayer, SIGNAL(forwardCursorPosition(int, int)));


    // SIGNALs forwarding
    //--------------------

    // ----- Toward TOPLAYER -----
    connect(this, SIGNAL(forwardCursorStateChange()),
            this->topLayer, SIGNAL(forwardCursorStateChange()));

    connect(this, SIGNAL(forwardCursorIntervalInput(int, int)),
            this->topLayer, SIGNAL(forwardCursorIntervalInput(int, int)));
}


//      %%%%%%%%%%%%%%%%%%%%%%%%%%
//      %%%% METHODS override %%%%
//      %%%%%%%%%%%%%%%%%%%%%%%%%%


/***************
 * cf Qt documentation.
 ***************/
void Wrapper::resizeEvent(QResizeEvent *e)
{
    QWidget::resizeEvent(e);

    int oldWidth = e->oldSize().width();

    if (oldWidth >= 0)
    {
        bottomLayer->resize(bottomLayer->width() + e->size().width() - oldWidth,
                        bottomLayer->height() + e->size().height() - e->oldSize().height());

        topLayer->resize(topLayer->width() + e->size().width() - oldWidth,
                        topLayer->height() + e->size().height() - e->oldSize().height());
    }
}
