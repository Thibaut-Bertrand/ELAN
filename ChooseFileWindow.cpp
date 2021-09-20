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
 * Present the user with a list of files, asking her-him to choose one of them,
 * then return which file was selected.
 *
 * Author: Thibaut Bertrand
 * thibaut.bertrand@inserm.fr
 ***************/


#include "ChooseFileWindow.h"
#include "ui_ChooseFileWindow.h"


/***************
 * Constructor
 *-------------------------
 * QVector<QString> fileList: the list of files to choose from.
 ***************/
ChooseFileWindow::ChooseFileWindow(QVector<QString> fileList,
                                   QString userMessage,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChooseFileWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("Choose a file");

    for (int i_file = 0; i_file < fileList.length(); i_file++)
    {
        this->ui->fileCombobox->addItem(fileList[i_file]);
    }

    this->ui->textToUserLabel->setText(userMessage);
}


/***************
 * Destructor
 ***************/
ChooseFileWindow::~ChooseFileWindow()
{
    delete ui;
}


//      %%%%%%%%%%%%%%%%%%%%%%%
//      %%%% SLOTS private %%%%
//      %%%%%%%%%%%%%%%%%%%%%%%


/***************
 * Call the destructor to close the window.
 ***************/
void ChooseFileWindow::on_cancelButton_clicked()
{
    this->~ChooseFileWindow();
}


/***************
 * Send the index of the selected file, then close the window.
 ***************/
void ChooseFileWindow::on_selectButton_clicked()
{
    emit sendFileIndex(this->ui->fileCombobox->currentIndex());
    emit sendFileName(this->ui->fileCombobox->currentText());

    this->~ChooseFileWindow();
}
