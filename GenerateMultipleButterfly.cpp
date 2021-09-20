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
 * When the user want to automatically generetate multiple butterfly plots (when
 * in the ButterflyOrganization), this class will take care of creating the
 * window that will ask him/her which graph to generate. If the user want a
 * finer control on what's inside each graph, he should just use the default way
 * to create new butterfly.
 *
 * Author: Thibaut Bertrand
 * thibaut.bertrand@inserm.fr
 ***************/


#include "GenerateMultipleButterfly.h"
#include "ui_GenerateMultipleButterfly.h"


/***************
 * Constructor
 *-------------------------
 * QVector<QString> fileList: the list of the loaded data file.
 ***************/
GenerateMultipleButterfly::GenerateMultipleButterfly(QVector<QString> fileList,
                                                     QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GenerateMultipleButterfly)
{
    ui->setupUi(this);

    this->setWindowTitle("Generate multiple plots");


    for (int i_file = 0; i_file < fileList.length(); i_file++)
    {
        QString file = "File " + QString::number(i_file + 1) + ": " + fileList[i_file];
        this->ui->fileCombobox->addItem(file);
    }


    this->countBtfNb();
}


/***************
 * Destructor
 ***************/
GenerateMultipleButterfly::~GenerateMultipleButterfly()
{
    delete ui;
}


//      %%%%%%%%%%%%%%%%%%%%%%%%%
//      %%%% METHODS private %%%%
//      %%%%%%%%%%%%%%%%%%%%%%%%%


/***************
 * When selecting different options, this methods will give informations on how
 * many butterfly plots will be created when the user click on "Apply".
 ***************/
void GenerateMultipleButterfly::countBtfNb()
{
    int btfNb;

    if (this->ui->typeCombobox->currentText() == "EEG")
    {
        if (this->ui->channelsCombobox->currentText() == "Laterality")
        {
            btfNb = 2;
        }

        else if (this->ui->channelsCombobox->currentText() == "Regions")
        {
            btfNb = 4;
        }

        else
        {
            btfNb = 8;
        }
    }


    else
    {
        if (this->ui->channelsCombobox->currentText() == "Laterality")
        {
            btfNb = 2;
        }

        else if (this->ui->channelsCombobox->currentText() == "Regions")
        {
            btfNb = 5;
        }

        else
        {
            btfNb = 10;
        }
    }


    this->ui->plotNumberLabel->setText(QString::number(btfNb));
}


//      %%%%%%%%%%%%%%%%%%%%%%%
//      %%%% SLOTS private %%%%
//      %%%%%%%%%%%%%%%%%%%%%%%


/***************
 * Send the info to the ButterflyOrganization class in order to generate the
 * plots.
 ***************/
void GenerateMultipleButterfly::on_applyButton_clicked()
{
    int file = this->ui->fileCombobox->currentIndex();
    QString type = this->ui->typeCombobox->currentText();
    QString channels = this->ui->channelsCombobox->currentText();

    emit(plotToGenerate(file, type, channels));

    this->~GenerateMultipleButterfly();
}


/***************
 * Just close the window and cancel everything.
 ***************/
void GenerateMultipleButterfly::on_canceButton_clicked()
{
    this->~GenerateMultipleButterfly();
}


/***************
 * Update the number of planned butterfly plots.
 *-------------------------
 * int index: index of the channel type selected.
 ***************/
void GenerateMultipleButterfly::on_channelsCombobox_currentIndexChanged(int index)
{
    countBtfNb();
}


/***************
 * Update the number of planned butterfly plots.
 *-------------------------
 * int index: index of the channel type selected.
 ***************/
void GenerateMultipleButterfly::on_typeCombobox_currentIndexChanged(int index)
{
    countBtfNb();
}

