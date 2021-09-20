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
 * Allow the user to choose which part of the project to save. Unavailable data
 * aren't available to save.
 *
 * Author: Thibaut Bertrand
 * thibaut.bertrand@inserm.fr
 ***************/


#ifndef SAVEPROJECTWINDOW_H
#define SAVEPROJECTWINDOW_H


#include <QDialog>
#include <QtXml>


#include "CheckboxID.h"
#include "DataContainers/ProjectData.h"


namespace Ui {
class SaveProjectWindow;
}


class SaveProjectWindow : public QDialog
{

    Q_OBJECT


private:

    // Methods
    //--------------------
    void checkAvailableData();
    void xmlWrite();


    // Members
    //--------------------
    QList<QCheckBox*> checkBoxList;

    ProjectData* projectData;

    Ui::SaveProjectWindow* ui;


public:

    explicit SaveProjectWindow(ProjectData* project, QWidget *parent = nullptr);
    ~SaveProjectWindow();


private slots:

    // --- UI

    // Button
    void on_saveButton_clicked();
    void on_cancelButton_clicked();
    void on_selectAllButton_clicked();


    // Checkboxes
    void on_dataFilesCheckbox_stateChanged(int arg1);
    void on_eventFilesCheckbox_stateChanged(int arg1);
    void on_pageCheckbox_stateChanged(int arg1);
};

#endif // SAVEPROJECTWINDOW_H
