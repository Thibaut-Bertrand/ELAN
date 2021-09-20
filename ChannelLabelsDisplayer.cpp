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


#include "ChannelLabelsDisplayer.h"


/***************
 * Constructor
 *-------------------------
 * QSize size: the size of the parent.
 ***************/
ChannelLabelsDisplayer::ChannelLabelsDisplayer(QSize size)
{
    this->resize(size);
}


/***************
 * Destructor
 ***************/
ChannelLabelsDisplayer::~ChannelLabelsDisplayer()
{

}


//      %%%%%%%%%%%%%%%%%%%%%%%%%%%%
//      %%%% METHODS (override) %%%%
//      %%%%%%%%%%%%%%%%%%%%%%%%%%%%


/***************
 * Place the channel labels of the left widget of the central widget, from the
 * MainWindow. Each label have a position.
 ***************/
void ChannelLabelsDisplayer::paintEvent(QPaintEvent *e)
{
    QWidget::paintEvent(e);
    QPainter labelPainter(this);
    QPainter amplitudePainter(this);

    labelPainter.setFont(font);
    amplitudePainter.setFont(font);


    if (!labelList.isEmpty() && !labelPositions.isEmpty())
    {
        for (int i_label = 0; i_label < labelList.size(); i_label++)
        {
            labelPainter.drawText(0, labelPositions[i_label] - 12,
                                  this->width(), 10, Qt::AlignRight,
                                  labelList[i_label]);


            if (!channelAmplitude.isEmpty()
                    && channelAmplitude.length() == labelPositions.length())
            {
                amplitudePainter.drawText(0, labelPositions[i_label] - 12,
                                          this->width(), 10, Qt::AlignLeft,
                                          channelAmplitude[i_label]);
            }
        }
    }
}


//      %%%%%%%%%%%%%%%%%%%%%%
//      %%%% SLOTS public %%%%
//      %%%%%%%%%%%%%%%%%%%%%%


/***************
 * Request the update of the channel amplitudes on the left side of the central
 * widget.
 *-------------------------
 * QVector<QString> amplitudes: the list of the channel amplitudes.
 ***************/
void ChannelLabelsDisplayer::updateChannelAmplitudes(QVector<QString> amplitudes)
{
    channelAmplitude = amplitudes;

    repaint();
}


/***************
 * Request the update of the channel labels on the left side of the central
 * widget.
 *-------------------------
 * QVector<QString> newLabelList: the list of the channel labels.
 * QVector<double> graphPositions: the associated position for each label.
 ***************/
void ChannelLabelsDisplayer::updateChannelLabels(QVector<QString> newLabelList,
                                                 QVector<double> graphPositions)
{
    labelList = newLabelList;
    labelPositions = graphPositions;

    repaint();
}
