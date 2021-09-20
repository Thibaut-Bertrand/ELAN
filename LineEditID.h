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


#ifndef LINEEDITID_H
#define LINEEDITID_H


#include <QLineEdit>


class LineEditID : public QLineEdit
{
    Q_OBJECT


public:

    LineEditID(int _firstID);
    LineEditID(int _firstID, int _secondID, QString _type);


    // Methods
    //--------------------
    void setID(int newID);


    // Members
    //--------------------
    int firstID = 0;
    int secondID = 0;

    QString type = "";


signals:

    void sendTextChanged(int ID, QString text);


private slots:

    void sendData(QString text);
};

#endif // LINEEDITID_H
