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
 * Allow the user to choose which event file are active for each data file.
 *
 * Author: Thibaut Bertrand
 * thibaut.bertrand@inserm.fr
 ***************/


#ifndef CHOOSEACTIVEEVENTFILEWINDOW_H
#define CHOOSEACTIVEEVENTFILEWINDOW_H


#include <QDialog>


#include "CheckboxID.h"
#include "DataContainers/PlotData.h"


namespace Ui {
class ChooseActiveEventFileWindow;
}


class ChooseActiveEventFileWindow : public QDialog
{
    Q_OBJECT


private:

    // Members
    //--------------------
    PlotData* plotData;

    Ui::ChooseActiveEventFileWindow *ui;


public:

    explicit ChooseActiveEventFileWindow(PlotData* plot, QWidget *parent = nullptr);
    ~ChooseActiveEventFileWindow();


    // Members
    //--------------------
    QVector<QVector<CheckboxID*>> fileCheckboxesList;


signals:

    void activeEventFiles(QVector<int> activeEventFileList);


private slots:

    void onCheckboxClicked(int dataFileID, int eventFileID);

    void on_applyButton_clicked();
    void on_cancelButton_clicked();
};

#endif // CHOOSEACTIVEEVENTFILEWINDOW_H
