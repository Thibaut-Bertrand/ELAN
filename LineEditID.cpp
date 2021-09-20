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
 * A simple subclass of QLineEdit that add an ID member. With that, it is
 * possible to create multiple line edit box in a loop while still being able
 * to differentiate them.
 *
 * Author: Thibaut Bertrand
 * thibaut.bertrand@inserm.fr
 ***************/


#include "LineEditID.h"


/***************
 * Constructor
 *-------------------------
 * int _firstID: the first ID...
 ***************/
LineEditID::LineEditID(int _firstID)
{
    firstID = _firstID;

    connect(this, SIGNAL(textChanged(QString)),
            this, SLOT(sendData(QString)));
}



/***************
 * Used by the ChannelWindow
 *-------------------------
 * int _firstID: the firs ID.
 * int _secondID: the second ID.
 * QString _type: the type.
 ***************/
LineEditID::LineEditID(int _firstID, int _secondID, QString _type)
{
    firstID = _firstID;
    secondID = _secondID;
    type = _type;
}


//      %%%%%%%%%%%%%%%%%%%%%%%%
//      %%%% METHODS public %%%%
//      %%%%%%%%%%%%%%%%%%%%%%%%


/***************
 * Change the ID.
 *-------------------------
 * int newFirstID: the new first ID.
 ***************/
void LineEditID::setID(int newFirstID)
{
    firstID = newFirstID;
}


//      %%%%%%%%%%%%%%%%%%%%%%%
//      %%%% SLOTS private %%%%
//      %%%%%%%%%%%%%%%%%%%%%%%


/***************
 * Send data when the text is changed.
 *-------------------------
 * QString text: the new text.
 ***************/
void LineEditID::sendData(QString text)
{
    emit(sendTextChanged(firstID, text));
}
