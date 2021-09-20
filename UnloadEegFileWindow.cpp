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
 * Open a window to let the user choose which .eeg file to unload.
 *
 * Author: Thibaut Bertrand
 * thibaut.bertrand@inserm.fr
 ***************/


#include "UnloadEegFileWindow.h"
#include "ui_UnloadEegFileWindow.h"


/***************
 * Constructor
 *-------------------------
 * PlotData* plot: the data container of interest for the class.
 ***************/
UnloadEegFileWindow::UnloadEegFileWindow(PlotData* plot, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UnloadEegFileWindow)
{
    ui->setupUi(this);

    plotData = plot;

    for (int i_file = 0; i_file < plotData->eegFileNb; i_file++)
    {
        CheckboxID* checkbox = new CheckboxID(i_file);
        checkbox->setText(plotData->eegDataStorage[i_file]->eegFilename);

        connect(checkbox, SIGNAL(sendInfoShort(int, int)),
                this, SLOT(onCheckboxChanged(int)));

        checkboxList.append(checkbox);

        this->ui->fileLayout->addWidget(checkbox);
    }

    this->ui->fileLayout->addStretch();
}


/***************
 * Destructor
 ***************/
UnloadEegFileWindow::~UnloadEegFileWindow()
{
    delete ui;
}


//      %%%%%%%%%%%%%%%%%%%%%%
//      %%%% SLOTS public %%%%
//      %%%%%%%%%%%%%%%%%%%%%%


/***************
 * Uncheck the box that aren't the one selected.
 *-------------------------
 * int fileID: the new selected file.
 ***************/
void UnloadEegFileWindow::onCheckboxChanged(int fileID)
{
    for (int i_box = 0; i_box < checkboxList.length(); i_box++)
    {
        if (i_box != fileID)
        {
            checkboxList[i_box]->setChecked(false);
        }
    }
}

/***************
 * Send the data and close the window.
 ***************/
void UnloadEegFileWindow::on_applyButton_clicked()
{
    int fileIndex = 0;

    for (int i_box = 0; i_box < checkboxList.length(); i_box++)
    {
        if (checkboxList[i_box]->isChecked())
        {
            fileIndex = i_box;
        }
    }

    emit sendFileToUnload(fileIndex);

    this->~UnloadEegFileWindow();
}


/***************
 * Close the window.
 ***************/
void UnloadEegFileWindow::on_cancelButton_clicked()
{
    this->~UnloadEegFileWindow();
}
