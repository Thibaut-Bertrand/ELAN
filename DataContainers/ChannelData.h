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
 * This class is contains data about a single channel, it is used by the
 * eegDataStorage to keep track of each individual channels.
 *
 * Author: Thibaut Bertrand
 * thibaut.bertrand@inserm.fr
 ***************/


#ifndef CHANNELDATA_H
#define CHANNELDATA_H


#include "qcustomplot.h"


class ChannelData
{

public:

    ChannelData();


    // Methods
    //--------------------
    void setRange(long int amplitudeScale);
    void setUnit(QString newUnit);


    // Members
    //--------------------
    QString label = "";
    QString unit = ""; // uV, V, fT...
    QString type = ""; // EEG, MEG, other...
    QString region = ""; // Frontal, Occipital, Parietal, Temporal, Central
    QString laterality = ""; // Left, Right, Central

    bool isDisplayed = true; // If true, the channel is displayed (whether it's free, in a butterfly or whatever). If false, it won't appear.

    QCPRange amplitudeRange{0,0};

    QColor color;
};

#endif // CHANNELDATA_H
