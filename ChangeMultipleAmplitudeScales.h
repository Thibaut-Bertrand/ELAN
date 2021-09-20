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


#ifndef CHANGEMULTIPLEAMPLITUDESCALES_H
#define CHANGEMULTIPLEAMPLITUDESCALES_H


#include <QDialog>
#include <QIntValidator>


#include "CheckboxID.h"


namespace Ui {
class ChangeMultipleAmplitudeScales;
}


class ChangeMultipleAmplitudeScales : public QDialog
{
    Q_OBJECT


private:

    QIntValidator* intValidator = new QIntValidator(0, 100000000, this);
    QVector<CheckboxID*> fileCheckboxes;

    Ui::ChangeMultipleAmplitudeScales *ui;


public:

    explicit ChangeMultipleAmplitudeScales(int fileNb,
                                           QWidget *parent = nullptr);
    ~ChangeMultipleAmplitudeScales();


private slots:

    // --- Checkboxes

    void allFileSelected(int state);


    // --- UI
    void on_applyButton_clicked();
    void on_closeButton_clicked();


signals:

    void sendNewScales(QVector<int> filesID, QString channelType,
                       QString newAmplitudeScale);
};

#endif // CHANGEMULTIPLEAMPLITUDESCALES_H
