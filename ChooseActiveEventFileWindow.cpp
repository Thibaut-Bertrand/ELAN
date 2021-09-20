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
 * Allow the user to choose which event file are active for each data file.
 *
 * Author: Thibaut Bertrand
 * thibaut.bertrand@inserm.fr
 ***************/


#include "ChooseActiveEventFileWindow.h"
#include "ui_ChooseActiveEventFileWindow.h"


/***************
 * Constructor
 *-------------------------
 * PlotData* plot: the data container of interest for the class.
 ***************/
ChooseActiveEventFileWindow::ChooseActiveEventFileWindow(PlotData* plot,
                                                         QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChooseActiveEventFileWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("Choose active event file");

    plotData = plot;

    int currentEegFile = 0;

    // File reference
    for (int i_dataFile = 0; i_dataFile < plotData->eegFileNb; i_dataFile++)
    {
        if (plotData->eegDataStorage[i_dataFile]->hasEventLoaded)
        {
            QLabel* fileLabel = new QLabel;
            QString filename = plotData->eegDataStorage[i_dataFile]->eegFilename;
            fileLabel->setText("File " + QString::number(currentEegFile + 1) + ": " + filename);

            this->ui->fileReferenceLayout->addWidget(fileLabel);

            currentEegFile++;
        }
    }

    this->ui->fileReferenceLayout->addStretch();


    QTabWidget* tabWidget = new QTabWidget;
    this->ui->wrapperLayout->addWidget(tabWidget);

    fileCheckboxesList.resize(plotData->eegFileNb);
    currentEegFile = 0;

    for (int i_dataFile = 0; i_dataFile < plotData->eegFileNb; i_dataFile++)
    {
        if (plotData->eegDataStorage[i_dataFile]->hasEventLoaded)
        {
            QScrollArea* scrollArea = new QScrollArea;
            tabWidget->addTab(scrollArea, "File " + QString::number(currentEegFile + 1));

            QWidget* fileContainer = new QWidget;
            QVBoxLayout* fileLayout = new QVBoxLayout;
            fileContainer->setLayout(fileLayout);

            scrollArea->setWidget(fileContainer);
            scrollArea->setWidgetResizable(true);

            for (int j_eventFile = 0; j_eventFile < plotData->eegDataStorage[i_dataFile]->eventDataStorage.length(); j_eventFile++)
            {
                CheckboxID* fileCheckbox = new CheckboxID(currentEegFile, j_eventFile);
                fileCheckbox->setText(plotData->eegDataStorage[i_dataFile]->eventDataStorage[j_eventFile]->eventFilename);

                connect(fileCheckbox, SIGNAL(sendInfoShort(int, int)),
                        this, SLOT(onCheckboxClicked(int, int)));

                fileLayout->addWidget(fileCheckbox);

                if (plotData->eegDataStorage[i_dataFile]->activeEventFileIndex == j_eventFile)
                {
                    fileCheckbox->setChecked(true);
                }

                fileCheckboxesList[i_dataFile].append(fileCheckbox);
            }

            fileLayout->addStretch();

            currentEegFile++;
        }
    }
}


/***************
 * Destructor
 ***************/
ChooseActiveEventFileWindow::~ChooseActiveEventFileWindow()
{
    delete ui;
}


//      %%%%%%%%%%%%%%%%%%%%%%%
//      %%%% SLOTS private %%%%
//      %%%%%%%%%%%%%%%%%%%%%%%


/***************
 * Uncheck the box that aren't the selected one.
 *-------------------------
 * int dataFileID: the data file to which the event file is associated with.
 * int eventFileID: the selected event file to be active.
 ***************/
void ChooseActiveEventFileWindow::onCheckboxClicked(int dataFileID, int eventFileID)
{
    if (fileCheckboxesList[dataFileID][eventFileID]->isChecked())
    {
        for (int i_box = 0; i_box < fileCheckboxesList[dataFileID].length(); i_box++)
        {
            if (i_box != eventFileID)
            {
                fileCheckboxesList[dataFileID][i_box]->setChecked(false);
            }
        }
    }
}


/***************
 * Send data and close the window.
 ***************/
void ChooseActiveEventFileWindow::on_applyButton_clicked()
{
    QVector<int> activeEventFileList;

    for (int i_dataFile = 0; i_dataFile < fileCheckboxesList.length(); i_dataFile++)
    {
        activeEventFileList.append(0); // If no file are selected, we put the first one by default. If the user wants to hide a file, it's better to unload it.

        for (int j_eventFile = 0; j_eventFile < fileCheckboxesList[i_dataFile].length(); j_eventFile++)
        {
            if (fileCheckboxesList[i_dataFile][j_eventFile]->isChecked())
            {
                activeEventFileList[i_dataFile] = j_eventFile;
            }
        }
    }

    emit activeEventFiles(activeEventFileList);

    this->~ChooseActiveEventFileWindow();
}


/***************
 * Just close the window.
 ***************/
void ChooseActiveEventFileWindow::on_cancelButton_clicked()
{
    this->~ChooseActiveEventFileWindow();
}
