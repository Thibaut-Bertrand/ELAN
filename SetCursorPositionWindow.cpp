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
 * Ask to the user where to pu the first and second cursor. S-he can also set an
 * interval value. All the values are automatically checked and adjusted
 * depending of the actual time scale
 *
 * Author: Thibaut Bertrand
 * thibaut.bertrand@inserm.fr
 ***************/


#include "SetCursorPositionWindow.h"
#include "ui_SetCursorPositionWindow.h"


/***************
 * Constructor
 *-------------------------
 * PresentationData* presentation: the data container of interest.
 ***************/
SetCursorPositionWindow::SetCursorPositionWindow(PresentationData* presentation,
                                                 int timeScale,
                                                 QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetCursorPositionWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("Set cursors position");

    presentationData = presentation;

    actualTimeScale = timeScale;

    this->ui->actualTimeScaleLabel->setText(QString::number(actualTimeScale));

    this->ui->firstCursorLineEdit->setValidator(new QIntValidator(0,
                                                                  actualTimeScale,
                                                                  this));
    this->ui->secondCursorLineEdit->setValidator(new QIntValidator(0,
                                                                   actualTimeScale,
                                                                   this));

    this->ui->firstCursorLineEdit->setText("0");
    this->ui->secondCursorLineEdit->setText("0");
}


/***************
 * Destuctor
 ***************/
SetCursorPositionWindow::~SetCursorPositionWindow()
{
    delete ui;
}


//      %%%%%%%%%%%%%%%%%%%%%%%
//      %%%% SLOTS private %%%%
//      %%%%%%%%%%%%%%%%%%%%%%%


/***************
 * Send the data to place the cursors on the screen.
 ***************/
void SetCursorPositionWindow::on_applyButton_clicked()
{
    double firstRelativePosition = (double)( this->ui->firstCursorLineEdit->text().toInt() * 100 ) / actualTimeScale;
    double secondTwoRelativePosition = (double)( this->ui->secondCursorLineEdit->text().toInt() * 100 ) / actualTimeScale;

    double firstCursor = firstRelativePosition;
    double secondCursor = secondTwoRelativePosition;


    if (firstRelativePosition > secondTwoRelativePosition)
    {
        firstCursor = secondTwoRelativePosition;
        secondCursor = firstRelativePosition;
    }


    presentationData->setCursorRelativePosition(firstCursor, secondCursor);
}


/***************
 * Call the destructor to close the window.
 ***************/
void SetCursorPositionWindow::on_closeButton_clicked()
{
    this->~SetCursorPositionWindow();
}


/***************
 * When the user chane the value in the first line edit, it automatically
 * update the interval value.
 ***************/
void SetCursorPositionWindow::on_firstCursorLineEdit_textChanged(const QString &arg1)
{
    interval = qFabs(this->ui->firstCursorLineEdit->text().toInt()
                     - this->ui->secondCursorLineEdit->text().toInt());

    this->ui->intervalLabel->setText(QString::number(interval));
}


/***************
 * When the user chane the value in the second line edit, it automatically
 * update the interval value.
 ***************/
void SetCursorPositionWindow::on_secondCursorLineEdit_textChanged(const QString &arg1)
{
    interval = qFabs(this->ui->firstCursorLineEdit->text().toInt()
                     - this->ui->secondCursorLineEdit->text().toInt());

    this->ui->intervalLabel->setText(QString::number(interval));
}
