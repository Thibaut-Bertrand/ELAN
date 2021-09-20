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
 * Ask to the user where to pu the first and second cursor. S-he can also set an
 * interval value. All the values are automatically checked and adjusted
 * depending of the actual time scale
 *
 * Author: Thibaut Bertrand
 * thibaut.bertrand@inserm.fr
 ***************/


#ifndef SETCURSORPOSITIONWINDOW_H
#define SETCURSORPOSITIONWINDOW_H


#include <QDialog>
#include <QIntValidator>
#include <QtMath>


#include "DataContainers/PresentationData.h"


namespace Ui {
class SetCursorPositionWindow;
}


class SetCursorPositionWindow : public QDialog
{
    Q_OBJECT


private:

    // Members
    //--------------------
    int actualTimeScale = 0;
    int interval = 0;

    PresentationData* presentationData;

    Ui::SetCursorPositionWindow *ui;


public:

    explicit SetCursorPositionWindow(PresentationData* presentation,
                                     int timeScale, QWidget *parent = nullptr);
    ~SetCursorPositionWindow();




private slots:

    void on_applyButton_clicked();
    void on_closeButton_clicked();
    void on_firstCursorLineEdit_textChanged(const QString &arg1);
    void on_secondCursorLineEdit_textChanged(const QString &arg1);
};

#endif // SETCURSORPOSITIONWINDOW_H
