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
 * A simple dialog box to add a new event to the list.
 *
 * Author: Thibaut Bertrand
 * thibaut.bertrand@inserm.fr
 ***************/


#ifndef ADDEVENTDIALOG_H
#define ADDEVENTDIALOG_H


#include <QDebug>
#include <QDialog>
#include <QIntValidator>
#include <QMessageBox>


namespace Ui {
class AddEventDialog;
}

class AddEventDialog : public QDialog
{

    Q_OBJECT


private:

    // Methods
    //--------------------
    bool doCodeExist(int newEventCode);


    // Members
    //--------------------
    int fileID;
    int eventID;
    QVector<int> existingEventCodes;

    Ui::AddEventDialog *ui;


public:

    explicit AddEventDialog(int _fileID, int _eventID,
                            QVector<int> existingEvent,
                            QWidget *parent = nullptr);
    explicit AddEventDialog(QVector<int> existingEvent,
                            QWidget *parent = nullptr);
    ~AddEventDialog();



signals:

    void newEvent(int fileID, int eventID, int newEventCode, bool visibleState,
                  bool browsableState);
    void newEvent(int newEventCode, bool visibleState, bool browsableState);


private slots:

    void visibleCheckBoxStateChanged();
    void browsableCheckBoxStateChanged();


    void on_doneButton_clicked();
    void on_cancelButton_clicked();
};

#endif // ADDEVENTDIALOG_H
