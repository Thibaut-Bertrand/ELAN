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
 * Present the user with a list of files, asking her-him to choose one of them,
 * then return which file was selected.
 *
 * Author: Thibaut Bertrand
 * thibaut.bertrand@inserm.fr
 ***************/


#ifndef CHOOSEFILEWINDOW_H
#define CHOOSEFILEWINDOW_H


#include <QDialog>


namespace Ui {
class ChooseFileWindow;
}


class ChooseFileWindow : public QDialog
{
    Q_OBJECT


private:

    Ui::ChooseFileWindow *ui;


public:

    explicit ChooseFileWindow(QVector<QString> fileList, QString userMessage,
                              QWidget *parent = nullptr);
    ~ChooseFileWindow();


signals:

    void sendFileIndex(int fileIndex);
    void sendFileName(QString filename);


private slots:

    void on_cancelButton_clicked();
    void on_selectButton_clicked();
};

#endif // CHOOSEFILEWINDOW_H
