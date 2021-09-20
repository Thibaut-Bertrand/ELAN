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


#ifndef CENTRALBOTTOMLAYER_H
#define CENTRALBOTTOMLAYER_H


#include <QPalette>
#include <QWidget>


#include "qpainter.h"
#include "DataContainers/PresentationData.h"


class CentralBottomLayer : public QWidget
{
    Q_OBJECT


public:

    CentralBottomLayer(QSize size, PresentationData* presentation);
    ~CentralBottomLayer();


    // Methods (override)
    //--------------------
    void paintEvent(QPaintEvent *e);


    // Methods
    //--------------------
    void updateBackground();


    // Members
    //--------------------
    QPalette palette;
    QPainter painter;

    PresentationData* presentationDataContainer;


public slots:

    void onDataContainerChanged(size_t hint);
};

#endif // CENTRALBOTTOMLAYER_H
