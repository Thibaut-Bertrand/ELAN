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
 * Create and manage the window used to select the event that will serve as a
 * reference to realign multiple .eeg files on it.
 * All files must have event files loaded and active for this to be possible.
 *
 * Author: Thibaut Bertrand
 * thibaut.bertrand@inserm.fr
 ***************/


#ifndef SELECTALIGNMENTREFERENCEWINDOW_H
#define SELECTALIGNMENTREFERENCEWINDOW_H


#include <QDialog>


#include "ButtonID.h"
#include "CheckboxID.h"
#include "DataContainers/PlotData.h"


namespace Ui {
class SelectAlignmentReferenceWindow;
}


class SelectAlignmentReferenceWindow : public QDialog
{

    Q_OBJECT


private:

    // Members
    //--------------------
    QFont font = QFont("Arial", 10);
    QSize labelSize = QSize(40, 20);
    QSize buttonSize = QSize(70, 20);

    int selectedFileReference = -1;

    struct file
    {
        bool hasEvent = false;
        QListWidget* eventList;
    };

    QVector<file> fileList;


    PlotData* plotData;

    Ui::SelectAlignmentReferenceWindow* ui;


public:

    explicit SelectAlignmentReferenceWindow(PlotData* plot,
                                            QWidget* parent = nullptr);
    ~SelectAlignmentReferenceWindow();


signals:

    void sendEventReference(QVector<int> selectedEvents);


private slots:

    void on_applyButton_clicked();
    void on_cancelButton_clicked();
};

#endif // SELECTALIGNMENTREFERENCEWINDOW_H
