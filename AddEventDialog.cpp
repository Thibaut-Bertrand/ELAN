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
 * A simple dialog box to add a new event to the list.
 *
 * Author: Thibaut Bertrand
 * thibaut.bertrand@inserm.fr
 ***************/


#include "AddEventDialog.h"
#include "ui_AddEventDialog.h"


/***************
 * Constructor
 * This one is used by the EventWindow class, which is used to manage events
 * accross all the loaded file.
 *-------------------------
 * int _fileID: the file to which the event is associated.
 * int _eventID: the ID of the future new event.
 * QVector<int> existingEvent: list of the currently existing event codes in the
 * targeted file.
 ***************/
AddEventDialog::AddEventDialog(int _fileID, int _eventID,
                               QVector<int> existingEvent, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddEventDialog)
{
    ui->setupUi(this);

    this->setWindowTitle("Add new event");

    existingEventCodes = existingEvent;

    ui->codeLineEdit->setValidator(new QIntValidator(0, 1000, this));

    ui->visibleCheckBox->setChecked(true);
    ui->browsableCheckBox->setChecked(true);


    connect(ui->visibleCheckBox, SIGNAL(stateChanged(int)),
            this, SLOT(visibleCheckBoxStateChanged()));

    connect(ui->browsableCheckBox, SIGNAL(stateChanged(int)),
            this, SLOT(browsableCheckBoxStateChanged()));

    fileID = _fileID;
    eventID = _eventID;
}


/***************
 * Constructor
 * This one is used by the RecodeCurrentEventDialog class, which is used to
 * manage only one single events: the one that is currently focused (when the
 * view mode is event focused).
 *-------------------------
 * QVector<int> existingEvent: list of the currently existing event codes in the
 * targeted file.
 ***************/
AddEventDialog::AddEventDialog(QVector<int> existingEvent, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddEventDialog)
{
    ui->setupUi(this);

    existingEventCodes = existingEvent;

    ui->codeLineEdit->setValidator(new QIntValidator(0, 1000, this));

    ui->visibleCheckBox->setChecked(true);
    ui->browsableCheckBox->setChecked(true);


    connect(ui->visibleCheckBox, SIGNAL(stateChanged(int)),
            this, SLOT(visibleCheckBoxStateChanged()));

    connect(ui->browsableCheckBox, SIGNAL(stateChanged(int)),
            this, SLOT(browsableCheckBoxStateChanged()));
}


/***************
 * Destructor
 ***************/
AddEventDialog::~AddEventDialog()
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
bool AddEventDialog::doCodeExist(int newEventCode)
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
 * When an event is not set to be visible, it cannot be browsable. This method
 * will uncheck the browsable checkbox when the user uncheck the visible
 * checkbox.
 ***************/
void AddEventDialog::visibleCheckBoxStateChanged()
{
    if (!ui->visibleCheckBox->isChecked())
    {
        ui->browsableCheckBox->setChecked(false);
    }
}


/***************
 * When an event is set to be browsable, it has to be visible. This method will
 * set the visible checkbox as checked when the browsable checkbox is checked by
 * the user.
 ***************/
void AddEventDialog::browsableCheckBoxStateChanged()
{
    if (ui->browsableCheckBox->isChecked())
    {
        ui->visibleCheckBox->setChecked(true);
    }
}


/***************
 * Check if the specified event exist or not. Then, send information to add the
 * new event, with its code, and visibility and browsability states.
 ***************/
void AddEventDialog::on_doneButton_clicked()
{
    // Check if lineEdit empty
    if (ui->codeLineEdit->text().isEmpty())
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("No event name");
        msgBox.setText("You must give a code to the new event.");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
    }

    else
    {
        // Check if event already exist
        if (doCodeExist(ui->codeLineEdit->text().toInt()))
        {
            QMessageBox msgBox;
            msgBox.setWindowTitle("Name already taken");
            msgBox.setText("This event already exist. Choose another code.");
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.exec();
        }

        else
        {
            int newEventCode = ui->codeLineEdit->text().toInt();


            bool visibleState = false;

            if (ui->visibleCheckBox->isChecked())
            {
                visibleState = true;
            }


            bool browsableState = false;

            if (ui->browsableCheckBox->isChecked())
            {
                browsableState = true;
            }


            emit newEvent(fileID, eventID, newEventCode, visibleState,
                          browsableState);

            emit newEvent(newEventCode, visibleState, browsableState);


            this->~AddEventDialog();
        }
    }
}


/***************
 * Just call the Destructor to close the window.
 ***************/
void AddEventDialog::on_cancelButton_clicked()
{
    this->~AddEventDialog();
}
