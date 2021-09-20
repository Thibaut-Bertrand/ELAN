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


#ifndef UNLOADEEGFILEWINDOW_H
#define UNLOADEEGFILEWINDOW_H


#include <QDialog>


#include "CheckboxID.h"
#include "DataContainers/PlotData.h"


namespace Ui {
class UnloadEegFileWindow;
}


class UnloadEegFileWindow : public QDialog
{
    Q_OBJECT


private:

    // Members
    //--------------------
    QVector<CheckboxID*> checkboxList;

    PlotData* plotData;

    Ui::UnloadEegFileWindow *ui;


public:

    explicit UnloadEegFileWindow(PlotData* plot, QWidget *parent = nullptr);
    ~UnloadEegFileWindow();




signals:

    void sendFileToUnload(int fileIndex);


private slots:

    void onCheckboxChanged(int fileID);

    void on_applyButton_clicked();
    void on_cancelButton_clicked();

};

#endif // UNLOADEEGFILEWINDOW_H
