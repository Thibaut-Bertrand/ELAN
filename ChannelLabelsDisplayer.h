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
 * Display the channel labels and the amplitude values at first cursor.
 *
 * Author: Thibaut Bertrand
 * thibaut.bertrand@inserm.fr
 ***************/


#ifndef CHANNELLABELSDISPLAYER_H
#define CHANNELLABELSDISPLAYER_H


#include <QWidget>
#include <QDebug>
#include <QPainter>


class ChannelLabelsDisplayer : public QWidget
{

    Q_OBJECT


public:

    ChannelLabelsDisplayer(QSize size);
    ~ChannelLabelsDisplayer();


    // Methods (override)
    //--------------------
    void paintEvent(QPaintEvent *e);


    // Members
    //--------------------
    QFont font = QFont("Arial",8);

    QVector<QString> labelList;
    QVector<double> labelPositions;
    QVector<QString> channelAmplitude;


public slots:

    void updateChannelAmplitudes(QVector<QString> amplitudes);
    void updateChannelLabels(QVector<QString> newLabelList,
                             QVector<double> graphPositions);
};

#endif // CHANNELLABELSDISPLAYER_H
