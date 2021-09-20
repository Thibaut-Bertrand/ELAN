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
 * This class is used when the user wants to change teh code of the currently
 * focused event. Only used when the view is in event focused mode. If the user
 * so desire, it is possible to create a new code before assigning it to the
 * event.
 *
 * Author: Thibaut Bertrand
 * thibaut.bertrand@inserm.fr
 ***************/


#include "RecodeCurrentEventDialog.h"
#include "ui_RecodeCurrentEventDialog.h"


/***************
 * Constructor
 *-------------------------
 * PlotData* plot: the pointer to the data container of interest.
 * int fileID: the file index to which the event is associated with.
 * int eventID: the event index in the file that is currently focused.
 ***************/
RecodeCurrentEventDialog::RecodeCurrentEventDialog(PlotData* plot, int fileID,
                                                   int eventID,
                                                   QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RecodeCurrentEventDialog)
{
    ui->setupUi(this);

    this->setWindowTitle("Recode current event");

    plotData = plot;

    eventFile = fileID;
    targetedEvent = eventID;


    int eventFileIndex = plotData->eegDataStorage[eventFile]->activeEventFileIndex;

    for (int i_event = 0; i_event < plotData->eegDataStorage[eventFile]->eventDataStorage[eventFileIndex]->eventStates.length(); i_event++)
    {
        existingEventCodes.append(plotData->eegDataStorage[eventFile]->eventDataStorage[eventFileIndex]->eventStates[i_event][0]);

        this->ui->eventListComboBox->addItem(QString::number(existingEventCodes[i_event]));
    }
}


/***************
 * Destructor
 ***************/
RecodeCurrentEventDialog::~RecodeCurrentEventDialog()
{
    delete ui;
}


//      %%%%%%%%%%%%%%%%%%%%%%%
//      %%%% SLOTS private %%%%
//      %%%%%%%%%%%%%%%%%%%%%%%


/***************
 * Send the selected code to apply to the event. The newly created code, with
 * its states, will be added to the list.
 *-------------------------
 * int newCode: the newCode to apply to the event. Which is also the newly
 * created code.
 * bool isVisible: is the newly created code visible.
 * bool isBrowsable: is the newly created code browsable.
 ***************/
void RecodeCurrentEventDialog::addNewEvent(int newCode, bool isVisible,
                                           bool isBrowsable)
{
    plotData->recodeCurrentEvent(eventFile, targetedEvent, newCode, true,
                                 isVisible, isBrowsable);

    this->~RecodeCurrentEventDialog();
}


//      ============
//      %%%% UI %%%%
//      ============


/***************
 * Send the selected code to apply to the event.
 ***************/
void RecodeCurrentEventDialog::on_okPushButton_clicked()
{
    int newCode = ui->eventListComboBox->currentText().toInt();

    plotData->recodeCurrentEvent(eventFile, targetedEvent, newCode);

    this->~RecodeCurrentEventDialog();
}


/***************
 * Open a small dialog box to ask the user which code he wants to create.
 ***************/
void RecodeCurrentEventDialog::on_newCodePushButton_clicked()
{
    addEventDialog = new AddEventDialog(existingEventCodes);

    connect(this->addEventDialog, SIGNAL(newEvent(int, bool, bool)),
            this, SLOT(addNewEvent(int, bool, bool)));

    addEventDialog->show();
}
