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


#include "ChannelData.h"


/***************
 * Constructor
 ***************/
ChannelData::ChannelData()
{

}


//      %%%%%%%%%%%%%%%%%%%%%%%%
//      %%%% METHODS public %%%%
//      %%%%%%%%%%%%%%%%%%%%%%%%


/***************
 * Initialize the QCPRange for the channel, depending on its unit (uV, V, fT)
 * and the specified scale for each of those.
 *-------------------------
 * int uVScale: the scale for micro-volt.
 * int VScale: the scale for volt.
 * int fTScale: the scale for femto-Tesla.
 *
 * Note: Actual and default amplitude scales are found in the ScaleData class.
 ***************/
void ChannelData::setRange(long int amplitudeScale)
{
    amplitudeRange.lower = -amplitudeScale;
    amplitudeRange.upper = amplitudeScale;
}


/***************
 * Set the unit of the channel, as well as its type.
 *-------------------------
 * QString newUnit: the new unit to set. uV and fT are EEG and MEG respectively.
 * Other units are generally in V.
 ***************/
void ChannelData::setUnit(QString newUnit)
{
    unit = newUnit;

    if (unit == "uV")
    {
        type = "EEG";
    }

    else if (unit == "fT")
    {
        type = "MEG";
    }

    else
    {
        type = "other";
    }
}
