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

#ifndef UNLOADEVENTFILEWINDOW_H
#define UNLOADEVENTFILEWINDOW_H


#include <QDialog>


#include "CheckboxID.h"
#include "DataContainers/PlotData.h"


namespace Ui {
class UnloadEventFileWindow;
}

class UnloadEventFileWindow : public QDialog
{
    Q_OBJECT


private:

    // Methods
    //--------------------
    void clearLayout(QLayout *layout);
    void setEventFileCheckboxes();


    // Members
    //--------------------
    int currentEegFileIndex = 0;
    QVector<CheckboxID*> eventFileCheckboxList;

    PlotData* plotData;

    Ui::UnloadEventFileWindow *ui;


public:

    explicit UnloadEventFileWindow(PlotData* plot, QWidget *parent = nullptr);
    ~UnloadEventFileWindow();


signals:

    void sendFileToUnload(int eegID, int eventID);


private slots:

    void onCheckboxChanged(int eegID, int eventID);

    void on_applyButton_clicked();
    void on_cancelButton_clicked();
    void on_fileCombobox_currentIndexChanged(int index);


};

#endif // UNLOADEVENTFILEWINDOW_H
