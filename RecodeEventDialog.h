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
 * A simple dialog box to change the code of an existing event.
 *
 * Author: Thibaut Bertrand
 * thibaut.bertrand@inserm.fr
 ***************/

#ifndef RECODEEVENTDIALOG_H
#define RECODEEVENTDIALOG_H


#include <QDialog>
#include <QIntValidator>
#include <QMessageBox>


namespace Ui {
class RecodeEventDialog;
}


class RecodeEventDialog : public QDialog
{
    Q_OBJECT


private:

    // Methods
    //--------------------
    bool doCodeExist(int newEventCode);


    // Members
    //--------------------
    int originalEventCode = 0;
    int originalEventID = 0;
    QVector<int> existingEventCodes;
    int fileID = 0;


    Ui::RecodeEventDialog *ui;


public:

    explicit RecodeEventDialog(int originalCode, int originalID,
                               QVector<int> eventCodes, int file,
                               QWidget *parent = nullptr);
    ~RecodeEventDialog();


signals:

    void sendNewCode(int fileID, int originalEventID, int newEventCode);


private slots:

    void on_applyButton_clicked();
    void on_cancelButton_clicked();
};

#endif // RECODEEVENTDIALOG_H
