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
 * Create and manage the Event Window, which is used to modify the visibility
 * and browsability state of events, as well as to modify or delete existing
 * event or create new ones.
 * A visible event is, obviously, visible on the sreen.
 * A browsable event is always visible, and is an event that the user can use to
 * focus the view in the screen, so that it can be used instead of the page
 * view.
 *
 * Author: Thibaut Bertrand
 * thibaut.bertrand@inserm.fr
 ***************/


#ifndef EVENTWINDOW_H
#define EVENTWINDOW_H


#include <QCheckBox>
#include <QCloseEvent>
#include <QDebug>
#include <QDialog>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QScrollArea>
#include <QtXml>
#include <QWidget>


#include "AddEventDialog.h"
#include "RecodeEventDialog.h"
#include "ButtonID.h"
#include "CheckboxID.h"
#include "ChooseFileWindow.h"
#include "DataContainers/PlotData.h"


namespace Ui {
class EventWindow;
}


class EventWindow : public QDialog
{
    Q_OBJECT


private:

    // Methods (override)
    //--------------------
    void closeEvent(QCloseEvent *e);
    void keyPressEvent(QKeyEvent *e);
    void keyReleaseEvent(QKeyEvent *e);


    // Methods
    //--------------------
    void applyChanges();
    void checkboxMultipleStateChange();
    void setMinMaxSize(QWidget* widget, QSize min, QSize max);

    // --- Load / Save
    void xmlRead(QString filename, int file);
    void xmlWrite(QString filename, int file);


    // Members
    //--------------------
    QFont font = QFont("Arial", 10);
    QSize labelSize = QSize(70, 20);
    QSize checkboxSize = QSize(20, 20);
    QSize buttonSize = QSize(70, 20);
    QSize bigButtonSize = QSize(120, 20);

    struct event
    {
        QLabel* label;

        CheckboxID* visibleCheckbox; // Type = 0
        CheckboxID* browsableCheckbox; // Type = 1
        CheckboxID* recodeCheckbox; // Type = not needed
        CheckboxID* deleteCheckbox; // Type = 2

        bool isNewEvent = false;
    };

    struct eventFile
    {
        int fileIndex;
        QString filename;

        QVector<event> events;

        ButtonID* addEventButton;
        ButtonID* applyToAllFileButton;

        QVBoxLayout* layout;
    };

    QVector<eventFile> eventFiles;


    // --- Multiple selection
    QVector<int> lastInteraction = {-1, -1, -1}; // File | Event | Box
    QVector<int> secondToLastInteraction = lastInteraction; // Same

    bool isShiftPressed = false; // Use to make a shift + click and (de)select multiple checkboxes at once.
    bool isBoxInteract = false; // Tell if the user has interact with a checkbox or not. Use with lastBoxInteracted.


    Ui::EventWindow *ui;
    AddEventDialog *addEventDialog;
    ChooseFileWindow* chooseFile;
    RecodeEventDialog *recodeEventDialog;
    PlotData* plotData;


public:

    explicit EventWindow(PlotData* plot, QWidget* parent = nullptr);
    ~EventWindow();


signals:

    void controlPress();
    void controlRelease();


private slots:

    // --- Checkboxes
    void changeVisibleCheckBoxState(int file, int event);
    void changeBrowsableCheckBoxState(int file, int event);
    void trackLastInteraction(int fileID, int eventID, int boxType);


    // --- Add
    void requestAddNewEvent(int fileID);
    void addNewEvent(int fileID, int eventID, int newEventCode,
                     bool visibleState, bool browsableState,
                     bool isNewEvent = true);


    // --- Recode
    void recodeCheckboxChanged(int fileID, int eventID);
    void openRecodeEventWindow(int fileID, int eventID);
    void displayNewEventCode(int fileID, int eventID, int newEventCode);


    // --- Apply to All
    void applyToAllFile(int fileID);


    // --- Load / Save
    void on_loadPresetButton_clicked();
    void on_savePresetButton_clicked();

    void requestLoad(int fileIndex);
    void requestSave(int fileIndex);


    // --- Buttons
    void on_applyButton_clicked();
    void on_closeButton_clicked();
};

#endif // EVENTWINDOW_H
