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
 * Create a small window to allow the user to change the scale of multiple
 * channels at the same time.
 *
 * Author: Thibaut Bertrand
 * thibaut.bertrand@inserm.fr
 ***************/


#include "ChangeMultipleAmplitudeScales.h"
#include "ui_ChangeMultipleAmplitudeScales.h"


/***************
 * Constructor
***************/
ChangeMultipleAmplitudeScales::ChangeMultipleAmplitudeScales(int fileNb,
                                                             QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChangeMultipleAmplitudeScales)
{
    ui->setupUi(this);

    this->setWindowTitle("Set multiple scales");


    CheckboxID* allFileCheckbox = new CheckboxID();
    allFileCheckbox->setText("All files");
    this->ui->fileLayout->addWidget(allFileCheckbox);

    connect(allFileCheckbox, SIGNAL(stateChanged(int)),
            this, SLOT(allFileSelected(int)));


    for (int i_file = 0; i_file < fileNb; i_file++)
    {
        CheckboxID* fileCheckbox = new CheckboxID(i_file);
        fileCheckbox->setText("File " + QString::number(i_file + 1));
        this->ui->fileLayout->addWidget(fileCheckbox);
        fileCheckboxes.append(fileCheckbox);
    }

    this->ui->fileLayout->addStretch();
    this->ui->fileLayout->setSpacing(3);


    this->ui->amplitudeLineEdit->setValidator(intValidator);
}


/***************
 * Destructor
***************/
ChangeMultipleAmplitudeScales::~ChangeMultipleAmplitudeScales()
{
    delete ui;
}


//      %%%%%%%%%%%%%%%%%%%%%%%
//      %%%% SLOTS private %%%%
//      %%%%%%%%%%%%%%%%%%%%%%%


//      ====================
//      %%%% Checkboxes %%%%
//      ====================


void ChangeMultipleAmplitudeScales::allFileSelected(int state)
{
    if (state == 0)
    {
        // Unchecked -> Enable the others and uncheck them
        for (int i_file = 0; i_file < fileCheckboxes.length(); i_file++)
        {
            fileCheckboxes[i_file]->setEnabled(true);
            fileCheckboxes[i_file]->setChecked(false);
        }
    }

    else if (state == 2)
    {
        // Checked -> Disable the others and check them
        for (int i_file = 0; i_file < fileCheckboxes.length(); i_file++)
        {
            fileCheckboxes[i_file]->setEnabled(false);
            fileCheckboxes[i_file]->setChecked(true);
        }
    }
}


//      ============
//      %%%% UI %%%%
//      ============


/***************
 * Send the changes to be made to the channels amplitude scales.
***************/
void ChangeMultipleAmplitudeScales::on_applyButton_clicked()
{
    // Get file indexes
    QVector<int> filesID;

    for (int i_file = 0; i_file < fileCheckboxes.length(); ++i_file)
    {
        if (fileCheckboxes[i_file]->isChecked())
        {
            filesID.append(i_file);
        }
    }

    // Get channels' type
    QString channelType = this->ui->channeCombobox->currentText();

    // Get the new amplitude
    QString newAmplitudeScale = this->ui->amplitudeLineEdit->text();


    emit sendNewScales(filesID, channelType, newAmplitudeScale);
}


/***************
 * Just call the destructor to close the window.
***************/
void ChangeMultipleAmplitudeScales::on_closeButton_clicked()
{
    this->~ChangeMultipleAmplitudeScales();
}
