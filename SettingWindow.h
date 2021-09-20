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
/* Create and manage the Setting Window, which is used to modify the themes   */
/* and colors of the backgrounds, the cursors, and the highlighted cursors'   */
/* interval.                                                                  */
/*                                                                            */
/* Author: Thibaut Bertrand                                                   */
/* thibaut.bertrand@inserm.fr                                                 */
/* ########################################################################## */


#ifndef SETTINGWINDOW_H
#define SETTINGWINDOW_H


#include <QColorDialog>
#include <QDialog>
#include <QDebug>
#include <QFileDialog>
#include <QtXml>


#include "ButtonID.h"
#include "DataContainers/PresentationData.h"


namespace Ui {
class SettingWindow;
}


class SettingWindow : public QDialog
{
    Q_OBJECT


private:

    // Methods
    //--------------------
    void changeColorOfCursor(int target, int red, int green, int blue);
    void changeColorOfCursor(int target, QColor newColor);
    void changeColorOfPlot(int fileID, int red, int green, int blue);
    void changeColorOfPlot(int fileID, QColor newColor);
    void colorSquareUpdate();
    void colorSampleButtons(int red, int green, int blue);


    // Members
    //--------------------
    QColor background;
    QColor squareColor;


    // --- Plots
    QVector<QWidget*> plotSquares;


    // --- Others
    PresentationData* presentationData;
    Ui::SettingWindow *ui;


public:

    explicit SettingWindow(PresentationData* presentation, QWidget* parent = 0);
    ~SettingWindow();


signals:

    void requestColorChange(QColor);


private slots:

    // Background
    //--------------------

    // --- Buttons

    // Colors
    void on_pushButtonBackground_clicked();
    void on_pushButton_2Background_clicked();
    void on_pushButton_3Background_clicked();
    void on_pushButton_4Background_clicked();
    void on_pushButton_5Background_clicked();
    void on_pushButton_6Background_clicked();
    void on_pushButton_7Background_clicked();
    void on_pushButton_8Background_clicked();
    void on_pushButton_9Background_clicked();
    void on_pushButton_10Background_clicked();
    void on_pushButton_11Background_clicked();
    void on_pushButton_12Background_clicked();
    void on_pushButton_13Background_clicked();
    void on_pushButton_14Background_clicked();
    void on_pushButton_15Background_clicked();
    void on_pushButton_16Background_clicked();

    // Actions
    void on_applyBackgroundButton_clicked();
    void on_defaultBackgroundButton_clicked();
    void on_customColorBackgroundButton_clicked();


    // --- Edits
    void on_editRedBackground_textEdited(const QString &arg1);
    void on_editGreenBackground_textEdited(const QString &arg1);
    void on_editBlueBackground_textEdited(const QString &arg1);


    // --- Sliders
    void on_sliderRedBackground_sliderMoved(int position);
    void on_sliderGreenBackground_sliderMoved(int position);
    void on_sliderBlueBackground_sliderMoved(int position);


    // Cursors
    //--------------------

    // --- Buttons
    void on_applyCursorsButton_clicked();
    void on_cursorOneButton_clicked();
    void on_cursorTwoButton_clicked();
    void on_defaultCursorsButton_clicked();
    void on_intervalButton_clicked();


    // Plots
    //--------------------

    // Colors
    void on_changeSinglePlotButton_clicked();
    void selectNewColor(int fileID);

    // Actions
    void on_applyPlotButton_clicked();
    void on_defaultPlotButton_clicked();


    // General
    //--------------------
    void on_closeButton_clicked();


    // --- Load / Save configuration
    void on_loadThemeButton_clicked();
    void on_saveThemeButton_clicked();
    void xmlRead(QString filename);
    void xmlWrite(QString filename);
};

#endif // SETTINGWINDOW_H
