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
 * A simple subclass of QPushButton that add an ID member. With that, it is
 * possible to create multiple button in a loop while still being able to
 * differentiate them.
 *
 * Author: Thibaut Bertrand
 * thibaut.bertrand@inserm.fr
 ***************/


#include "ButtonID.h"


/***************
 * Constructor
 *-------------------------
 * int _firstID: the first ID for the button.
 * int _secondID: the second ID for the button.
 ***************/
ButtonID::ButtonID(int _firstID, int _secondID, QWidget *parent) :
    QPushButton(parent)
{
    firstID = _firstID;
    secondID = _secondID;
}


//      %%%%%%%%%%%%%%%%%%%%%%%%%%
//      %%%% METHODS override %%%%
//      %%%%%%%%%%%%%%%%%%%%%%%%%%


/***************
 * cf Qt documentation.
 ***************/
void ButtonID::mouseReleaseEvent(QMouseEvent *e)
{
    emit sendID(firstID, secondID);
}


//      %%%%%%%%%%%%%%%%%%%%%%%%
//      %%%% METHODS public %%%%
//      %%%%%%%%%%%%%%%%%%%%%%%%


/***************
 * Change the first ID.
 *-------------------------
 * int newFirstID: the new first ID.
 ***************/
void ButtonID::setFirstID(int newFirstID)
{
    firstID = newFirstID;
}


/***************
 * Change the second ID.
 *-------------------------
 * int newSecondID: the new second ID.
 ***************/
void ButtonID::setSecondID(int newSecondID)
{
    secondID = newSecondID;
}
