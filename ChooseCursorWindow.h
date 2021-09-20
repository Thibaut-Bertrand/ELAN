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
 * Ask the user on which cursor to add the event. This window should only be
 * displayed when the two cursors are present. Otherwise, it's useless to ask in
 * the first place.
 *
 * Author: Thibaut Bertrand
 * thibaut.bertrand@inserm.fr
 ***************/


#ifndef CHOOSECURSORWINDOW_H
#define CHOOSECURSORWINDOW_H


#include <QDialog>


#include "DataContainers/PlotData.h"
#include "DataContainers/PresentationData.h"


namespace Ui {
class ChooseCursorWindow;
}


class ChooseCursorWindow : public QDialog
{
    Q_OBJECT


private:

    // Members
    //--------------------
    QVector<int> fileIndexes;

    PlotData* plotData;
    PresentationData* presentationData;

    Ui::ChooseCursorWindow *ui;


public:

    explicit ChooseCursorWindow(PlotData* plot, PresentationData* presentation,
                                QWidget *parent = nullptr);
    ~ChooseCursorWindow();


signals:

    void sendNewEventInfo(double cursorPosition, int fileID, int eventCode);


private slots:

    // --- Others
    void changeProposedEventCode(int fileID);


    // --- UI
    void on_OkPushButton_clicked();
    void on_CancelButton_clicked();
};

#endif // CHOOSECURSORWINDOW_H
