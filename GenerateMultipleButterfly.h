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


#ifndef GENERATEMULTIPLEBUTTERFLY_H
#define GENERATEMULTIPLEBUTTERFLY_H


#include <QDialog>


namespace Ui {
class GenerateMultipleButterfly;
}


class GenerateMultipleButterfly : public QDialog
{
    Q_OBJECT


private:

    // Methods
    //--------------------
    void countBtfNb();


    // Members
    //--------------------
    Ui::GenerateMultipleButterfly *ui;


public:

    explicit GenerateMultipleButterfly(QVector<QString> fileList,
                                       QWidget *parent = nullptr);
    ~GenerateMultipleButterfly();




signals:

    void plotToGenerate(int file, QString type, QString channels);


private slots:

    // UI
    void on_applyButton_clicked();
    void on_canceButton_clicked();
    void on_channelsCombobox_currentIndexChanged(int index);
    void on_typeCombobox_currentIndexChanged(int index);
};

#endif // GENERATEMULTIPLEBUTTERFLY_H
