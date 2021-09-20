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


/* ########################################################################## */
/* Create and manage the Scale Window, which is used to modify the scale of   */
/* graphs' x- and y-axis, displayed in the Central Widget.                    */
/*                                                                            */
/* Author: Thibaut Bertrand                                                   */
/* thibaut.bertrand@inserm.fr                                                 */
/* ########################################################################## */


#ifndef SCALEWINDOW_H
#define SCALEWINDOW_H


#include <QDebug>
#include <QDialog>
#include <QLabel>
#include <QtXml>


#include "ChangeMultipleAmplitudeScales.h"
#include "ChooseFileWindow.h"
#include "DataContainers/PageData.h"
#include "DataContainers/PlotData.h"
#include "LineEditID.h"


namespace Ui {
class ScaleWindow;
}


class ScaleWindow : public QDialog
{
    Q_OBJECT


private:

    // Methods
    //--------------------
    void setMinMaxSize(QWidget* widget, QSize minSize, QSize maxSize);


    // --- Load / Save
    void xmlRead(QString filename, int fileIndex);
    void xmlWrite(QString saveFilename, int targetFileIndex);


    // Members
    //--------------------
    QFont font = QFont("Arial", 10);
    QSize labelSize = QSize(85, 20);
    int layoutSpacing = 5;

    QIntValidator* intValidator = new QIntValidator(0, 100000000, this);
    QDoubleValidator* doubleValidator = new QDoubleValidator(0., 1000000000., 3, this);


    QVector<QString> channelLabels;

    struct file
    {
        QVector<QString> channelNames;
        QVector<LineEditID*> channelLineEdit;
    };

    QVector<file> files;

    QVector<QString> filenameList;


    PageData* pageData;
    PlotData* plotData;

    ChangeMultipleAmplitudeScales* changeMultipleScalesWindow;
    ChooseFileWindow* chooseFile;

    Ui::ScaleWindow *ui;


public:

    explicit ScaleWindow(PageData* page, PlotData* plot, QWidget *parent = 0);
    ~ScaleWindow();


private slots:


    void applyMultipleNewScales(QVector<int> filesID, QString channelType,
                                QString newAmplitudeScale);


    // --- Load / Save configuration
    void requestLoad(int fileIndex);
    void requestSave(int fileIndex);


    // UI
    //--------------------

    // --- Time Scale
    void on_applyTimeButton_clicked();
    void on_defaultTimeButton_clicked();


    // --- Amplitude Scales

    // Load / Save configuration
    void on_loadAmplitudeButton_clicked();
    void on_saveAmplitudeButton_clicked();


    // Change scales
    void on_applyAmplitudeButton_clicked();
    void on_setDefaultsButton_clicked();
    void on_setScalesButton_clicked();


    // --- Others
    void on_closeButton_clicked();
};

#endif // SCALEWINDOW_H
