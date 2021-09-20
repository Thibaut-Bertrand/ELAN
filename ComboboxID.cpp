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


#include "ComboboxID.h"


/***************
 * Constructor
 *-------------------------
 * int chan: the first ID for the button.
 * int file: the second ID for the button.
 ***************/
ComboboxID::ComboboxID(int chan, int file, QWidget* parent)
    : QComboBox(parent)
{
    IDchan = chan;
    IDfile = file;

    connect(this, SIGNAL(activated(int)),
            this, SLOT(onCurrentIndexChanged()));

    connect(this, SIGNAL(currentTextChanged(QString)),
            this, SLOT(onCurrentIndexChanged()));
}


//      %%%%%%%%%%%%%%%%%%%%%%%%
//      %%%% METHODS public %%%%
//      %%%%%%%%%%%%%%%%%%%%%%%%


/***************
 * Add items.
 *-------------------------
 * QVector<QString> text: the items to add.
 ***************/
void ComboboxID::addText(QVector<QString> text)
{
    for (int i = 0; i < text.length(); i++)
    {
        this->addItem(text[i]);
    }
}


/***************
 * Add items.
 *-------------------------
 * QVector<QLabel*> labels: the items to add.
 ***************/
void ComboboxID::addText(QVector<QLabel*> labels)
{
    for (int i = 0; i < labels.length(); i++)
    {
        this->addItem(labels[i]->text());
    }
}


/***************
 * Add an item to the combobox.
 *-------------------------
 * QString string: the new item to add.
 ***************/
void ComboboxID::addText(QString string)
{
    this->addItem(string);
}


/***************
 * Change the first ID.
 *-------------------------
 * int newIDchan: the new first ID.
 ***************/
void ComboboxID::setIDchan(int newIDchan)
{
    IDchan = newIDchan;
}


/***************
 * Change the second ID.
 *-------------------------
 * int newIDfile: the new second ID.
 ***************/
void ComboboxID::setIDfile(int newIDfile)
{
    IDfile = newIDfile;
}


//      %%%%%%%%%%%%%%%%%%%%%%%
//      %%%% SLOTS private %%%%
//      %%%%%%%%%%%%%%%%%%%%%%%


/***************
 * Called when the index change..
 ***************/
void ComboboxID::onCurrentIndexChanged()
{
    emit currentIndexChanged(IDchan, IDfile);
}
