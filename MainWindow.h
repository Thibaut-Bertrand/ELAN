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
 * Create and manage the main window of the application, as well as every other
 * sub-windows (seting window...). The central widget contain a wrapper that
 * manage multiple layers for the display of data.
 *
 * Author: Thibaut Bertrand
 * thibaut.bertrand@inserm.fr
 ***************/


#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QtWidgets>
#include <QMainWindow>
#include <QListWidget>
#include <vector>
#include <QFileDialog>


#include "ChannelLabels.h"
#include "DataContainers/ProjectData.h"
#include "EventLabels.h"
#include "ProjectManager.h"
#include "SettingWindow.h"
#include "Wrapper.h"


namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{

    Q_OBJECT


private:

    // Methods
    //--------------------
    void clearLayout(QLayout* layout);
    void setWrapper();
    void setEventLabels();
    void setChannelLabels();
    void setFileNamesLegend();


    // Members
    //--------------------

    // --- UI
    Ui::MainWindow* ui;
    SettingWindow* settingWindow;

    // --- Data Containers
    ProjectData* projectData;

    // --- Others
    QWidget* wrapper;
    QWidget* eventLabels;
    QWidget* channelLabels;

    ProjectManager* projectManager;


public:

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


    // Methods (override)
    //--------------------
    bool eventFilter(QObject* obj, QEvent* e);
    void keyPressEvent(QKeyEvent *e);
    void keyReleaseEvent(QKeyEvent *e);


    // Methods
    //--------------------
    bool checkHeaderFile(QString filename);
    void headerFileNotFoundWarning();


    // --- Locking / Unlocking UI
    void eegEnabler(bool areEnabled);
    void eventEnabler(bool areEnabled);


    // Members
    //--------------------
    bool eegFileOpen = false; // If a .eeg file is already open, become true. Allow to ask the user if she wants to load multiple .eeg files at the same time or not


signals:

    // Channels
    //--------------------
    void openChannelSelectionWindow();


    // Cursors
    //--------------------
    void cursorIntervalInput(int cursorInterval, int timeScale);
    void cursorStateChange();


    // Events
    //--------------------
    void addEventOnCursor();
    void sendEventFileName(QString fileName);


    // Files
    //--------------------
    void openFileAlignementWindow();
    void sendFileName(QString fileName);


    // Keyboard
    //--------------------
    void returnKeyboard();
    void takeKeyboard();


    // Pages
    //--------------------
    void pageChange(QKeyEvent *e);
    void pageChange(QString direction);
    void pageToDisplay(int page);


    // Project manager
    //--------------------
    void loadProject(QString filename);
    void loadProjectRequest(ProjectManager* projectManager);
    void saveProjectRequest(ProjectManager* projectManager);


public slots:

    // Cursors
    //--------------------
    void dockShowDataCursor(int cursorOne, int cursorTwo);
    void dockShowIntervalData(int time);


    // Keyboard
    //--------------------
    void getKeyboard();


    // Loading project
    //--------------------
    void eegLoaded();
    void posLoaded();


    // Pages
    //--------------------
    void dockShowReferenceTime();
    void updatePageNb();


private slots:

    // Channels
    //--------------------
    void on_actionSelect_triggered();


    // Cursors
    //--------------------
    void on_clickModeButton_clicked();
    void on_cursorValueButton_clicked();
    void on_dragModeButton_clicked();
    void on_fileAmplitudeButton_clicked();
    void on_resetCursorButton_clicked();


    // Data
    //--------------------
    void on_actionButterfly_triggered();
    void on_actionSignalCentering_triggered();


    // Dock checkboxes option
    //--------------------
    void on_plotOriginCheckbox_clicked();
    void on_butterflyCheckbox_stateChanged(int arg1);
    void on_cursorIntervalCheckbox_clicked();


    // Events
    //--------------------
    void on_actionLoadEventFile_triggered();
    void on_actionManageEvent_triggered();
    void on_actionSaveEventFile_triggered();

    void on_actionManageDisplayedFiles_triggered();
    void on_actionUnloadEventFiles_triggered();

    void on_acceptEventButton_clicked();
    void on_addEventButton_clicked();
    void on_recodeEventButton_clicked();
    void on_rejectEventButton_clicked();


    // Files
    //--------------------
    void on_actionAlignFiles_triggered();
    void on_actionLoadFile_triggered();
    void on_actionUnloadFiles_triggered();
    void on_actionSet_Current_Dir_triggered();


    // Modes
    //--------------------
    void on_eventModeButton_clicked(); // Event mode: you choose a specific event code, and chaging "page" just jump to the next event (the view is centered on the actual event)
    void on_pageModeButton_clicked(); // Classic mode: the start of the next page is the end of the actual one

    void on_actionEventMode_triggered();
    void on_actionPageMode_triggered();


    // Pages
    //--------------------
    void on_actualPageButton_clicked();
    void on_nextPageButton_clicked();
    void on_previousPageButton_clicked();


    // Scales
    //--------------------
    void on_actionScales_triggered();

    void on_actualTimeScaleButton_clicked();
    void on_defaultTimeScaleButton_clicked();
    void on_lowerTimeButton_clicked();
    void on_upperTimeButton_clicked();


    // Others
    //--------------------
    void on_actionDocumentation_triggered();
    void on_actionPreferences_triggered();
    void on_actionQuit_triggered();


    // Project Manager
    //--------------------
    void on_actionLoadProject_triggered();
    void on_actionSaveProject_triggered();


    // HUB
    //--------------------
    void onPageDataChanged(size_t hint);
    void onPlotDataChanged(size_t hint);
    void onProjectDataChanged(size_t hint);
    void on_ticksCheckbox_clicked();
};

#endif // MAINWINDOW_H
