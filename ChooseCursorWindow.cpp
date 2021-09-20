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
 * Ask the user on which cursor to add the event. This window should only be
 * displayed when the two cursors are present. Otherwise, it's useless to ask in
 * the first place.
 *
 * Author: Thibaut Bertrand
 * thibaut.bertrand@inserm.fr
 ***************/


#include "ChooseCursorWindow.h"
#include "ui_ChooseCursorWindow.h"


/***************
 * Constructor
 *-------------------------
 * PlotData* plot: the data container concerning the plots, files and events.
 * PresentationData* presentation: the data container concerning the cursors.
 ***************/
ChooseCursorWindow::ChooseCursorWindow(PlotData* plot, PresentationData* presentation,
                                       QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChooseCursorWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("Add event on cursor");


    plotData = plot;
    presentationData = presentation;


    // Cursors
    //--------------------

    // Only the first cursor is present = no choice
    if (presentationData->cursorOneRelativePosition != 0
            && presentationData->cursorTwoRelativePosition == 0)
    {
        this->ui->cursorCombobox->addItem("First cursor");
        this->ui->cursorCombobox->setEnabled(false);
    }

    // Or only the second = no choice
    else if (presentationData->cursorOneRelativePosition == 0
             && presentationData->cursorTwoRelativePosition != 0)
    {
        this->ui->cursorCombobox->addItem("Second cursor");
        this->ui->cursorCombobox->setEnabled(false);
    }

    // Both are here = choice
    else if (presentationData->cursorOneRelativePosition != 0
             && presentationData->cursorTwoRelativePosition != 0)
    {
        this->ui->cursorCombobox->addItem("First cursor");
        this->ui->cursorCombobox->addItem("Second cursor");
    }


    // Files
    //--------------------
    for (int i_file = 0; i_file < plot->eegFileNb; i_file++)
    {
        if (plot->eegDataStorage[i_file]->hasEventLoaded)
        {
            this->ui->fileCombobox->addItem(plot->eegDataStorage[i_file]->eegFilename);
            fileIndexes.append(i_file);
        }
    }

    connect(this->ui->fileCombobox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(changeProposedEventCode(int)));

    this->ui->fileCombobox->setCurrentIndex(0);
    changeProposedEventCode(0);
}


/***************
 * Destructor
 ***************/
ChooseCursorWindow::~ChooseCursorWindow()
{
    delete ui;
}


//      %%%%%%%%%%%%%%%%%%%%%%%
//      %%%% SLOTS private %%%%
//      %%%%%%%%%%%%%%%%%%%%%%%


//      ================
//      %%%% Others %%%%
//      ================


/***************
 * Change the proposed event code depending on the selected event file.
 *-------------------------
 * int fileID: the selected event file index.
 ***************/
void ChooseCursorWindow::changeProposedEventCode(int fileID)
{
    this->ui->codeCombobox->clear();

    int eventFileIndex = plotData->eegDataStorage[fileID]->activeEventFileIndex;

    for (int i_code = 0; i_code < plotData->eegDataStorage[fileID]->eventDataStorage[eventFileIndex]->eventStates.length(); i_code++)
    {
        this->ui->codeCombobox->addItem(QString::number(plotData->eegDataStorage[fileID]->eventDataStorage[eventFileIndex]->eventStates[i_code][0]));
    }
}


//      ============
//      %%%% UI %%%%
//      ============


/***************
 * Send the selected cursor' position and close the window.
 ***************/
void ChooseCursorWindow::on_OkPushButton_clicked()
{
    double cursorPosition = 0.;

    if (this->ui->cursorCombobox->currentText() == "First cursor")
    {
        cursorPosition = presentationData->cursorOneRelativePosition;
    }

    else if (this->ui->cursorCombobox->currentText() == "Second cursor")
    {
        cursorPosition = presentationData->cursorTwoRelativePosition;
    }

    int fileID = fileIndexes[this->ui->fileCombobox->currentIndex()];

    int eventCode = this->ui->codeCombobox->currentText().toInt();


    emit sendNewEventInfo(cursorPosition, fileID, eventCode);

    this->~ChooseCursorWindow();
}


/***************
 * Call the destructor to close the window.
 ***************/
void ChooseCursorWindow::on_CancelButton_clicked()
{
    this->~ChooseCursorWindow();
}
