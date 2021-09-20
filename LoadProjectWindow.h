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
 * Allow the user to choose which part of the project to load. Unavailable data
 * aren't available to load.
 *
 * Author: Thibaut Bertrand
 * thibaut.bertrand@inserm.fr
 ***************/


#ifndef LOADPROJECTWINDOW_H
#define LOADPROJECTWINDOW_H


#include <QDialog>
#include <QtXml>
#include <QDebug>
#include <QVector>


#include "DataContainers/ProjectData.h"


namespace Ui {
class LoadProjectWindow;
}


class LoadProjectWindow : public QDialog
{
    Q_OBJECT


private:

    // Methods
    //--------------------
    void xmlRead();
    void checkAvailableData();


    // Members
    //--------------------
    QString filenameToRead;
    QList<QCheckBox*> checkBoxList;

    ProjectData* projectData;

    Ui::LoadProjectWindow* ui;


public:

    explicit LoadProjectWindow(QString filename, ProjectData* project, QWidget *parent = nullptr);
    ~LoadProjectWindow();


private slots:

    // --- UI

    // Buttons
    void on_cancelButton_clicked();
    void on_loadProjectButton_clicked();
    void on_selectAllButton_clicked();


    // Checkboxes
    void on_dataFilesCheckbox_stateChanged(int arg1);
    void on_channelStatesCheckbox_stateChanged(int arg1);
    void on_eventFilesCheckbox_stateChanged(int arg1);
    void on_eventStatesCheckbox_stateChanged(int arg1);
    void on_butterflyCheckbox_stateChanged(int arg1);
    void on_pageCheckbox_stateChanged(int arg1);
    void on_timeScaleCheckbox_stateChanged(int arg1);
    void on_viewModeCheckbox_stateChanged(int arg1);
};


#endif // LOADPROJECTWINDOW_H
