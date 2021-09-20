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
 * A simple dialog box to change the code of an existing event.
 *
 * Author: Thibaut Bertrand
 * thibaut.bertrand@inserm.fr
 ***************/


#include "RecodeEventDialog.h"
#include "ui_RecodeEventDialog.h"


/***************
 * Constructor
 *-------------------------
 * int code: original event code that the user wants to change.
 * int
 ***************/
RecodeEventDialog::RecodeEventDialog(int originalCode, int originalID,
                                     QVector<int> eventCodes, int file,
                                     QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RecodeEventDialog)
{
    ui->setupUi(this);

    this->setWindowTitle("Recode event");

    originalEventCode = originalCode;
    originalEventID = originalID;
    existingEventCodes = eventCodes;
    fileID = file;

    ui->eventCodeLineEdit->setValidator(new QIntValidator(0, 1000, this));
}


/***************
 * Destructor
 ***************/
RecodeEventDialog::~RecodeEventDialog()
{
    delete ui;
}


//      %%%%%%%%%%%%%%%%%%%%%%%%%
//      %%%% METHODS private %%%%
//      %%%%%%%%%%%%%%%%%%%%%%%%%


/***************
 * Check if the code choosen by the user don't already exist.
 *-------------------------
 * int newEventCode: the new event code selected by the user.
 *
 * return: true if it exist, false if not
 ***************/
bool RecodeEventDialog::doCodeExist(int newEventCode)
{
    for (int i_event = 0; i_event < existingEventCodes.length(); i_event++)
    {
        if (newEventCode == existingEventCodes[i_event])
        {
            return true;
        }
    }

    return false;
}


//      %%%%%%%%%%%%%%%%%%%%%%%
//      %%%% SLOTS private %%%%
//      %%%%%%%%%%%%%%%%%%%%%%%


/***************
 * First, check if the code already exist in the provided list or not. Then,
 * return the new code.
 ***************/
void RecodeEventDialog::on_applyButton_clicked()
{
    if (ui->eventCodeLineEdit->text().isEmpty())
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Empty");
        msgBox.setText("Please enter a code or click on \"Cancel\"");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
    }

    else
    {
        int newEventCode = ui->eventCodeLineEdit->text().toInt();

        if (doCodeExist(newEventCode))
        {
            QMessageBox msgBox;
            msgBox.setWindowTitle("Code already taken");
            msgBox.setText("This event already exist. Choose another code.");
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.exec();
        }

        else
        {
            emit sendNewCode(fileID, originalEventID, newEventCode);

            this->~RecodeEventDialog();
        }
    }
}


/***************
 * Just call teh Destructor.
 ***************/
void RecodeEventDialog::on_cancelButton_clicked()
{
    this->~RecodeEventDialog();
}
