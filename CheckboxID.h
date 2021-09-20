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


#ifndef CHECKBOXID_H
#define CHECKBOXID_H


#include <QCheckBox>
#include <QDebug>


class CheckboxID : public QCheckBox
{
    Q_OBJECT

public:


    CheckboxID(int IDnumber, QString BoxType = "", QWidget* parent = nullptr);
    CheckboxID(int firstID, int secondID, QString type = "", QWidget* parent = nullptr);
    CheckboxID(int firstID, int secondID, int thirdID, QWidget* parent = nullptr);
    CheckboxID(QWidget* parent = nullptr);


    // Methods (override)
    //--------------------
    void enterEvent(QEvent* e);
    void mousePressEvent(QMouseEvent* e);


    // Methods
    //--------------------
    void changeCheckState();
    void setID(int newID);
    void setNewState(bool state);


    // Members
    //--------------------
    QString type = ""; // visible or browsable

    int ID = 0;
    int secondID = 0;
    int thirdID = 0;

    bool isMousePressed = false;


signals:

    void sendInfo(int ID, QString type, int secondID);
    void sendInfoShort(int ID, int secondID);
    void sendThreeID(int first, int second, int third);


private slots:

    void controlPress();
    void controlRelease();
};

#endif // CHECKBOXID_H
