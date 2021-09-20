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
 * Open a window to let the user choose to which .eeg file to associate the
 * event file.
 *
 * Author: Thibaut Bertrand
 * thibaut.bertrand@inserm.fr
 ***************/


#include "AssociateEegPosFileWindow.h"
#include "ui_AssociateEegPosFileWindow.h"


/***************
 * Constructor
 *-------------------------
 * QVector<QString> eegFilenameList: the list of the loaded data file.
 * QString eventFilename: the path of the event file.
 ***************/
AssociateEegPosFileWindow::AssociateEegPosFileWindow(QVector<QString> eegFilenameList,
                                                     QString eventFilename,
                                                     QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AssociateEegPosFileWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("File association");

    eventFile = eventFilename;

    for (int i = 0; i < eegFilenameList.length(); i++)
    {
        this->ui->fileSelectComboBox->addItem(eegFilenameList[i]);
    }

}


/***************
 * Destructor
 ***************/
AssociateEegPosFileWindow::~AssociateEegPosFileWindow()
{
    delete ui;
}


//      %%%%%%%%%%%%%%%%%%%%%%%
//      %%%% SLOTS private %%%%
//      %%%%%%%%%%%%%%%%%%%%%%%


/***************
 * Close the window.
 ***************/
void AssociateEegPosFileWindow::on_cancelButton_clicked()
{
    this->~AssociateEegPosFileWindow();
}


/***************
 * Change the index when the user select another item.
 ***************/
void AssociateEegPosFileWindow::on_fileSelectComboBox_currentIndexChanged(int index)
{
    fileIndex = index;
}


/***************
 * Send data and close the window.
 ***************/
void AssociateEegPosFileWindow::on_okButton_clicked()
{
    emit(sendFileIndex(eventFile, fileIndex)); // -> PlotData

    this->~AssociateEegPosFileWindow();
}
