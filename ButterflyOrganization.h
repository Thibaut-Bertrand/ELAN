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
 * Create and manage the butterfly organizer window. This window allow the user
 * to organize the channels of all the .eeg files into different butterfly.
 *
 * Author: Thibaut Bertrand
 * thibaut.bertrand@inserm.fr
 ***************/


#ifndef BUTTERFLYORGANIZATION_H
#define BUTTERFLYORGANIZATION_H


#include <QDialog>
#include <QtXml>


#include "ButtonID.h"
#include "CheckboxID.h"
#include "ComboboxID.h"
#include "DataContainers/PlotData.h"
#include "GenerateMultipleButterfly.h"
#include "LineEditID.h"


namespace Ui {
class ButterflyOrganization;
}


class ButterflyOrganization : public QDialog
{

    Q_OBJECT


private:

    // Methods
    //--------------------
    void generateExistingPlots();
    void updateBtfPresentChannel(int ID);
    void updateChannelsCheckboxes(int btfID);

    void setMinMaxSize(QWidget* widget, QSize minSize, QSize maxSize);

    void xmlRead(QString filename);
    void xmlWrite(QString filename);

    // == PROTOTYPE ==
    void createRegions();


    // Members
    //--------------------
    QFont font = QFont("Arial", 10);
    QFont smallFont = QFont("Arial", 7);
    QSize labelSize = QSize(70, 20);
    QSize comboboxSize = QSize(100, 20);
    QSize bigComboboxSize = QSize(200, 20);
    QSize smallButtonSize = QSize(20, 20);

    QVector<QString> filenameList;


    // --- UI File
    QTabWidget* fileTabWidget;
    QVector<QScrollArea*> fileScrollAreaList;


    // --- UI Butterfly
    QTabWidget* butterflyTabWidget;
    QVector<QWidget*> butterWidgetList;


    // --- Channels
    QVector<QHBoxLayout*> btfChannelLayout;
    QVector<QVector<QLabel*>> btfChannelLabel;
    QVector<QVBoxLayout*> fileLayoutList; // 1 per channel and 1 (the first) for the btf label

    struct channel
    {
        int fileID;
        int chanID;

        QString type;

        QLabel* label;
        QVector<CheckboxID*> checkboxList; // Boxes have 3 IDs : file, channel and butterfly
        QHBoxLayout* layout;
    };

    QVector<QVector<channel>> channelList;


    // --- Butterfly
    struct butterflyPlot
    {
        int ID;
        QString name;

        bool allFile = true;
        QVector<int> targetFiles; // The selected files for the btf


        // Present channels
        QVector<QVector<QString>>presentChannels;
        QVector<QVector<QLabel*>> channels; // The selected channels
        QHBoxLayout* presentChannelLayout; // Display the selected channels
        QVector<QVBoxLayout*> fileLayout;


        // UI
        LineEditID* btfNameEdit;
        ComboboxID* typeCombobox;
        QVector<CheckboxID*> fileCheckboxes; // Select the file(s) associated with the btf
        QVector<CheckboxID*> optionCheckboxes; // Lateral, region
        ButtonID* addAllChanButton;
        QVector<CheckboxID*> channelCheckboxes; // The possible channel group to select (if custom, it's empty)
        QVBoxLayout* layoutChannelSelection; // Possible channel group
        ButtonID* deleteButton;
    };

    int butterflyNb = 0;
    QVector<butterflyPlot> butterflyList;


    // --- Regions
    struct channelRegion
    {
        QString region; // frontal, parietal...
        QString type; // EEG, MEG...
        QString side; // Left or Right (centered are both)
        QVector<QString> channelNames;
    };

    QVector<channelRegion> regions;


    // --- Others

    QVector<QLabel*> butterflyLabelList;
    QVector<ComboboxID*> butterflyFilesComboBoxList;
    QVector<ComboboxID*> butterflyChannelsComboBoxList;
    QVector<ButtonID*> butterflyRemoveButtonList;
    QVector<QHBoxLayout*> butterflyLayoutList;

    PlotData* plotData;
    GenerateMultipleButterfly* multipleBtfWindow;
    Ui::ButterflyOrganization *ui;


public:

    explicit ButterflyOrganization(PlotData* plot, QWidget *parent = 0);
    ~ButterflyOrganization();


signals:

    void applyButterfly();


private slots:

    void addNewButterfly();
    void removeButterfly(int btfID);

    void btfFileCheckboxChange(int btfID);
    void changeProposedChannel(int btfID);

    void btfChannelSelected(int btfID);
    void channelBtfSelected(int fileID, int chanID, int btfID);

    void addAllChanToBtf(int btfID);

    void generatePlot(int targetFile, QString type, QString channels);
    void updateBtfName(int, QString);


    // UI
    void on_applyButton_clicked();
    void on_cancelButton_clicked();
    void on_loadButton_clicked();
    void on_multiplePlotButton_clicked();
    void on_saveButton_clicked();
};

#endif // BUTTERFLYORGANIZATION_H
