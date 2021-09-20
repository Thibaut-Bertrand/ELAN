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
 * A simple subclass of QComboBox that add an ID member. With that, it is
 * possible to create multiple spin box in a loop while still being able to
 * differentiate them.
 *
 * Author: Thibaut Bertrand
 * thibaut.bertrand@inserm.fr
 ***************/


#ifndef COMBOBOXID_H
#define COMBOBOXID_H


#include <QComboBox>
#include <QDebug>
#include <QLabel>


class ComboboxID : public QComboBox
{
    Q_OBJECT

public:

    ComboboxID(int chan, int file = 0, QWidget* parent = nullptr);


    // Methods
    //--------------------
    void addText(QVector<QString> text);
    void addText(QVector<QLabel*> labels);
    void addText(QString string);
    void setIDchan(int newIDchan);
    void setIDfile(int newIDfile);


    // Members
    //--------------------
    int IDchan;
    int IDfile;


signals:

    void currentIndexChanged(int IDchan, int IDfile);


private slots:

    void onCurrentIndexChanged();
};

#endif // COMBOBOXID_H
