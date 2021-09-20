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
 * This class is used as a wrapper that contain other, more specific data
 * containers, such as PlotData for variables concerning plots in general,
 * ScaleData for variables concerning the amplitude scales and ranges, PageData
 * for the time scale and the page information and PresentationData for colors,
 * themes and other aesthetic variables.
 * This class also contain some variable that don't match the other classes.
 *
 * Author: Thibaut Bertrand
 * thibaut.bertrand@inserm.fr
 ***************/


#ifndef RECODECURRENTEVENTDIALOG_H
#define RECODECURRENTEVENTDIALOG_H


#include "AddEventDialog.h"
#include "DataContainers/PlotData.h"

#include <QDialog>


namespace Ui {
class RecodeCurrentEventDialog;
}


class RecodeCurrentEventDialog : public QDialog
{
    Q_OBJECT


private:

    // Members
    //--------------------
    int eventFile = 0;
    int targetedEvent = 0;
    QVector<int> existingEventCodes;

    Ui::RecodeCurrentEventDialog *ui;
    AddEventDialog *addEventDialog;
    PlotData* plotData;


public:

    explicit RecodeCurrentEventDialog(PlotData* plot, int fileID,
                                      int eventID, QWidget *parent = nullptr);
    ~RecodeCurrentEventDialog();


signals:

    void sendNewCode(int newCode);
    void sendNewEventCode(int newCode, int visible, int browsable);


private slots:

    void addNewEvent(int newCode, bool isVisible, bool isBrowsable);


    // UI slots
    void on_okPushButton_clicked();
    void on_newCodePushButton_clicked();
};

#endif // RECODECURRENTEVENTDIALOG_H
