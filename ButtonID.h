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


#ifndef BUTTONID_H
#define BUTTONID_H


#include <QDebug>
#include <QPushButton>


class ButtonID : public QPushButton
{
    Q_OBJECT


public:

    ButtonID(int _firsID = 0, int _secondID = 0, QWidget* parent = 0);


    // Methods (override)
    //--------------------
    void mouseReleaseEvent(QMouseEvent *e) override;


    // Methods
    //--------------------
    void setFirstID(int newFirstID);
    void setSecondID(int newSecondID);


    // Members
    //--------------------
    int firstID = 0;
    int secondID = 0;


signals:

    void sendID(int firstID, int secondID);
};

#endif // BUTTONID_H
