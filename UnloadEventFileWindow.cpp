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
 * Let the user chose an event file to unload.
 *
 * Author: Thibaut Bertrand
 * thibaut.bertrand@inserm.fr
 ***************/


#include "UnloadEventFileWindow.h"
#include "ui_UnloadEventFileWindow.h"


/***************
 * Constructor
 *-------------------------
 * PlotData* plot: the data container of interest for the class.
 ***************/
UnloadEventFileWindow::UnloadEventFileWindow(PlotData* plot, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UnloadEventFileWindow)
{
    ui->setupUi(this);

    plotData = plot;


    for (int i_file = 0; i_file < plotData->eegFileNb; i_file++)
    {
        if (plotData->eegDataStorage[i_file]->hasEventLoaded)
        {
            QString filename = plotData->eegDataStorage[i_file]->eegFilename;

            this->ui->fileCombobox->addItem(filename);
        }
    }

    this->ui->fileCombobox->setCurrentIndex(0);
    on_fileCombobox_currentIndexChanged(0);
}


/***************
 * Destructor
 ***************/
UnloadEventFileWindow::~UnloadEventFileWindow()
{
    delete ui;
}


//      %%%%%%%%%%%%%%%%%%%%%%%%%
//      %%%% METHODS private %%%%
//      %%%%%%%%%%%%%%%%%%%%%%%%%


/***************
 * Clear the layout.
 *-------------------------
 * QLayout *layout: the layout to clear.
 ***************/
void UnloadEventFileWindow::clearLayout(QLayout *layout)
{
    QLayoutItem *item;

    while((item = layout->takeAt(0)))
    {
        if (item->layout())
        {
            clearLayout(item->layout());

            delete item->layout();
        }

        if (item->widget())
        {
           delete item->widget();
        }

        delete item;
    }
}


/***************
 * Create the event file checkboxes.
 ***************/
void UnloadEventFileWindow::setEventFileCheckboxes()
{
    clearLayout(this->ui->eventLayout);
    eventFileCheckboxList.clear();

    for (int i_eventFile = 0; i_eventFile < plotData->eegDataStorage[currentEegFileIndex]->eventDataStorage.length(); i_eventFile++)
    {
        CheckboxID* checkbox = new CheckboxID(currentEegFileIndex, i_eventFile);

        QString filename = plotData->eegDataStorage[currentEegFileIndex]->eventDataStorage[i_eventFile]->eventFilename;
        checkbox->setText(filename);

        connect(checkbox, SIGNAL(sendInfoShort(int, int)),
                this, SLOT(onCheckboxChanged(int, int)));

        eventFileCheckboxList.append(checkbox);
        this->ui->eventLayout->addWidget(checkbox);
    }

    this->ui->eventLayout->addStretch();
}


//      %%%%%%%%%%%%%%%%%%%%%%%
//      %%%% SLOTS private %%%%
//      %%%%%%%%%%%%%%%%%%%%%%%


/***************
 * Uncheck the checkboxes that aren't the one selected
 *-------------------------
 * int eegID: data file index.
 * int eventID: event file index.
 ***************/
void UnloadEventFileWindow::onCheckboxChanged(int eegID, int eventID)
{
    if (eventFileCheckboxList[eventID]->isChecked())
    {
        for (int i_box = 0; i_box < eventFileCheckboxList.length(); i_box++)
        {
            if (i_box != eventID)
            {
                eventFileCheckboxList[i_box]->setChecked(false);
            }
        }
    }
}


/***************
 * Send the data and close the window.
 ***************/
void UnloadEventFileWindow::on_applyButton_clicked()
{
    int eventID = 0;
    for (int i_box = 0; i_box < eventFileCheckboxList.length(); i_box++)
    {
        if (eventFileCheckboxList[i_box]->isChecked())
        {
            eventID = i_box;
        }
    }

    emit (sendFileToUnload(currentEegFileIndex, eventID));

    this->~UnloadEventFileWindow();
}


/***************
 * Close the window.
 ***************/
void UnloadEventFileWindow::on_cancelButton_clicked()
{
    this->~UnloadEventFileWindow();
}


/***************
 * Update the current index.
 *-------------------------
 * int index: the index of the combobox.
 ***************/
void UnloadEventFileWindow::on_fileCombobox_currentIndexChanged(int index)
{
    for (int i_file = 0; i_file < plotData->eegFileNb; i_file++)
    {
        if (plotData->eegDataStorage[i_file]->eegFilename == this->ui->fileCombobox->currentText())
        {
            currentEegFileIndex = i_file;
        }
    }

    setEventFileCheckboxes();
}
