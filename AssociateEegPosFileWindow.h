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
 * Open a window to let the user choose to which .eeg file to associate the
 * event file.
 *
 * Author: Thibaut Bertrand
 * thibaut.bertrand@inserm.fr
 ***************/


#ifndef ASSOCIATEEEGPOSFILEWINDOW_H
#define ASSOCIATEEEGPOSFILEWINDOW_H


#include <QDebug>
#include <QDialog>


namespace Ui {
class AssociateEegPosFileWindow;
}


class AssociateEegPosFileWindow : public QDialog
{
    Q_OBJECT


private:

    Ui::AssociateEegPosFileWindow *ui;


public:

    explicit AssociateEegPosFileWindow(QVector<QString> eegFilenameList,
                                       QString eventFilename,
                                       QWidget *parent = nullptr);
    ~AssociateEegPosFileWindow();


    // Members
    //--------------------
    int fileIndex = 0;
    QString eventFile = "";


signals:

    void sendFileIndex(QString eventFilename, int fileIndex);


private slots:

    void on_cancelButton_clicked();
    void on_fileSelectComboBox_currentIndexChanged(int index);
    void on_okButton_clicked();
};

#endif // ASSOCIATEEEGPOSFILEWINDOW_H
