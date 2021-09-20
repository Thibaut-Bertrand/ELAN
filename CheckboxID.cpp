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
 * A simple subclass of QCheckBox that add an ID member. With that, it is
 * possible to create multiple button in a loop while still being able to
 * differentiate them.
 *
 * Author: Thibaut Bertrand
 * thibaut.bertrand@inserm.fr
***************/


#include "CheckboxID.h"



// TODO: Just make single Constructor and adapt the whole code for it. It's ugly...


/***************
 * Constructor
 *-------------------------
 * int IDnumber: first ID.
 * QString BoxType: type.
 ***************/
CheckboxID::CheckboxID(int IDnumber, QString BoxType, QWidget *parent) : QCheckBox(parent)
{
    ID = IDnumber;
    type = BoxType;
}


/***************
 * Constructor
 *-------------------------
 * int firstIDnumber: first ID.
 * int secondIDnumber: second ID.
 * QString BoxType: type.
 ***************/
CheckboxID::CheckboxID(int firstIDnumber, int secondIDnumber, QString boxType, QWidget *parent) : QCheckBox(parent)
{
    ID = firstIDnumber;
    secondID = secondIDnumber;
    type = boxType;
}


/***************
 * Constructor
 *-------------------------
 * int firstIDnumber: first ID.
 * int secondIDnumber: second ID.
 * int thirdIDnumber: third ID.
 ***************/
CheckboxID::CheckboxID(int firstIDnumber, int secondIDnumber, int thirdIDnumber, QWidget *parent) : QCheckBox(parent)
{
    ID = firstIDnumber;
    secondID = secondIDnumber;
    thirdID = thirdIDnumber;
}


/***************
 * Constructor
 ***************/
CheckboxID::CheckboxID(QWidget* parent) : QCheckBox(parent)
{

}


//      %%%%%%%%%%%%%%%%%%%%%%%%%%
//      %%%% METHODS override %%%%
//      %%%%%%%%%%%%%%%%%%%%%%%%%%


/***************
 * cf Qt documentation
 ***************/
void CheckboxID::enterEvent(QEvent* e)
{
    if (isMousePressed)
    {
        changeCheckState();
    }
}


/***************
 * cf Qt documentation
 ***************/
void CheckboxID::mousePressEvent(QMouseEvent* e)
{
    changeCheckState();

    emit sendInfo(ID, type, secondID);
    emit sendInfoShort(ID, secondID);
    emit sendThreeID(ID, secondID, thirdID);
}




//      %%%%%%%%%%%%%%%%%%%%%%%%
//      %%%% METHODS public %%%%
//      %%%%%%%%%%%%%%%%%%%%%%%%


/***************
 * Inverse the check state of the box.
 ***************/
void CheckboxID::changeCheckState()
{
    if (this->checkState() == Qt::Unchecked)
    {
        this->setCheckState(Qt::Checked);
    }

    else
    {
        this->setCheckState(Qt::Unchecked);
    }
}


/***************
 * Just change the ID.
 *-------------------------
 * int newID: the new ID.
 ***************/
void CheckboxID::setID(int newID)
{
    ID = newID;
}


/***************
 * Ok, this one seems weird.
 * In the EventWindwo, I need two slots to be called when the checkbox is
 * interacted with. Thing is one of them just need the first two, and the other
 * the three of them. The problem come from the checkboxMultipleStateChange()
 * method of the EventWindow, as it should only be called once, for the other
 * checkboxes to be updated properly.
 *
 * I know it's weird, just wanted something that was working correctly...
 *-------------------------
 * bool state: the new check state.
 ***************/
void CheckboxID::setNewState(bool state)
{
    this->setChecked(state);

    emit(sendInfoShort(ID, secondID));
}


//      %%%%%%%%%%%%%%%%%%%%%%%
//      %%%% SLOTS private %%%%
//      %%%%%%%%%%%%%%%%%%%%%%%


/***************
 * Called when the user press the Ctrl key.
 ***************/
void CheckboxID::controlPress()
{
    isMousePressed = true;
}


/***************
 * Called when the user release the Ctrl key.
 ***************/
void CheckboxID::controlRelease()
{
    isMousePressed = false;
}
