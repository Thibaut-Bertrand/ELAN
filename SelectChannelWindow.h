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
 * A simple dialog box to select which chanels to display.
 *
 * Author: Thibaut Bertrand
 * thibaut.bertrand@inserm.fr
 ***************/


#ifndef SELECTCHANNELWINDOW_H
#define SELECTCHANNELWINDOW_H


#include <QCloseEvent>
#include <QDebug>
#include <QDialog>
#include <QDialog>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QtXml>
#include <QWidget>


#include "ButtonID.h"
#include "CheckboxID.h"
#include "ChooseFileWindow.h"
#include "DataContainers/PlotData.h"


namespace Ui {
class SelectChannelWindow;
}

class SelectChannelWindow : public QDialog
{
    Q_OBJECT


private:

    // Methods
    //--------------------

    // --- Load / Save
    void xmlRead(QString filename, int fileIndex);
    void xmlWrite(QString saveFilename, int targetFileIndex);


    // --- Checkboxes
    void checkboxMultipleStateChange();


    // --- Others
    void setMinMaxSize(QWidget* widget, QSize minSize, QSize maxSize);


    // Members
    //--------------------
    bool isChangeApplied = true; // Indicate if the user applied or not the changes. By default, the user didn't touch anything , so the "changes" are applied.
    bool isShiftPressed = false; // Use to make a shift + click and (de)select multiple checkboxes at once.
    bool selectAll = false;

    QSize smallLabelSize = QSize(50, 20);
    QFont font = QFont("Arial", 10);


    struct Channel{
        QLabel* label;
        ButtonID* selectAllFileButton;
        ButtonID* unselectAllFileButton;
        QVector<CheckboxID*> fileCheckboxes;
    };

    QVector<Channel*> channelList;

    QVector<QString> filenameList;


    QVector<int> lastInteraction = {-1, -1}; // File ID | Channel ID
    QVector<int> secondToLastInteraction = lastInteraction; // Same


    PlotData* plotData;

    ChooseFileWindow* chooseFile;
    Ui::SelectChannelWindow *ui;


public:

    explicit SelectChannelWindow(PlotData* plot, QWidget *parent = nullptr);
    ~SelectChannelWindow();


    // Methods (override)
    //--------------------
    void closeEvent(QCloseEvent *e);
    void keyPressEvent(QKeyEvent *e);
    void keyReleaseEvent(QKeyEvent *e);


signals:

    void updateChannelsToDisplay(); // Index of the channels to display
    void controlPress();
    void controlRelease();


private slots:

    // --- Checkboxes
    void selectAllFileForChan(int chanIndex);
    void trackLastInteraction(int fileID, int chanID);
    void unselectAllFileForChan(int chanIndex);

    // --- Load / Save
    void requestLoad(int fileIndex);
    void requestSave(int fileIndex);


    // UI
    //--------------------

    // --- Checkboxes
    void on_selectAllButton_clicked();


    // --- Load / Save
    void on_loadConfigurationButton_clicked();
    void on_saveConfigurationButton_clicked();


    // --- Others
    void on_applyButton_clicked();
    void on_cancelButton_clicked();
    void on_closeButton_clicked();

};

#endif // SELECTCHANNELWINDOW_H
