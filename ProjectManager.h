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
 * This class will simply manage request the initialization of two windoww: the
 * Saproject window and the LoadProject window. As their name suggest it, their
 * are opened when the user wants to save or load a project, that is, part or
 * data that are actually present.
 * Those data can be, for example: the data file loaded, the event file loaded,
 * the states of the channels and/or events, color themes, view position...
 *
 * Author: Thibaut Bertrand
 * thibaut.bertrand@inserm.fr
 ***************/


#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H


#include <QPalette>
#include <QString>
#include <QtWidgets>
#include <QVector>


#include "DataContainers/ProjectData.h"
#include "LoadProjectWindow.h"
#include "SaveProjectWindow.h"


class ProjectManager : public QWidget
{
    Q_OBJECT


private:

    // Members
    //--------------------
    ProjectData* projectData;

    SaveProjectWindow* saveProjectWindow;
    LoadProjectWindow* loadProjectWindow;


public:

    ProjectManager(ProjectData* project);
    ~ProjectManager();

    // Methods
    //--------------------
    void SaveProject();
    void LoadProject(QString filename);

};


#endif // PROJECTMANAGER_H
