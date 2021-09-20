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
 * A simple subclass of QSpinBox that add an ID member. With that, it is
 * possible to create multiple spin box in a loop while still being able to
 * differentiate them.
 *
 * Author: Thibaut Bertrand
 * thibaut.bertrand@inserm.fr
 ***************/


#ifndef SPINBOXID_H
#define SPINBOXID_H


#include <QDebug>
#include <QDoubleSpinBox>


class SpinBoxID : public QDoubleSpinBox
{
    Q_OBJECT


public:

    SpinBoxID(int chan, int file = 0, QWidget* parent = nullptr);


    // Methods
    //--------------------
    void setIDchan(int newIDchan);
    void setIDfile(int newIDfile);


    // Members
    //--------------------
    int IDchan;
    int IDfile;
};

#endif // SPINBOXID_H
