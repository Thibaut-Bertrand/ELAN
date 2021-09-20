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
 * Create and manage the window used to select the event that will serve as a
 * reference to realign multiple .eeg files on it.
 * All files must have event files loaded and active for this to be possible.
 *
 * Author: Thibaut Bertrand
 * thibaut.bertrand@inserm.fr
 ***************/


#include "SelectAlignmentReferenceWindow.h"
#include "ui_SelectAlignmentReferenceWindow.h"


/***************
 * Constructor
 *-------------------------
 * PlotData* plot: the data container of interest for this class.
 ***************/
SelectAlignmentReferenceWindow::SelectAlignmentReferenceWindow(PlotData* plot,
                                                               QWidget* parent) :
    QDialog(parent), ui(new Ui::SelectAlignmentReferenceWindow)
{
    // eventDataList:
    // Event Sample | Event Code | Accept/Reject
    // For all the event of the .pos file.


    ui->setupUi(this);

    this->setWindowTitle("File alignment");

    plotData = plot;


    // File references
    //--------------------
    for (int i_file = 0; i_file < plotData->eegFileNb; i_file++)
    {
        QLabel* fileReference = new QLabel;
        QString filename = plotData->eegDataStorage[i_file]->eegFilename;

        fileReference->setText("File " + QString::number(i_file + 1)
                               + ": " + filename);
        fileReference->setFont(font);

        this->ui->fileReferenceLayout->addWidget(fileReference);
    }

    this->ui->fileReferenceLayout->addStretch();


    // Event selector
    //--------------------
    for (int i_file = 0; i_file < plotData->eegFileNb; i_file++)
    {
        file newFile;

        QVBoxLayout* fileLayout = new QVBoxLayout;

        QLabel* fileLabel = new QLabel;
        fileLabel->setText("File " + QString::number(i_file + 1));
        fileLabel->setFont(font);
        fileLayout->addWidget(fileLabel);


        if (plotData->eegDataStorage[i_file]->hasEventLoaded)
        {
            newFile.eventList = new QListWidget;
            newFile.eventList->setMaximumWidth(100);

            int eventFileIndex = plotData->eegDataStorage[i_file]->activeEventFileIndex;

            for (int j_event = 0; j_event < plotData->eegDataStorage[i_file]->eventDataStorage[eventFileIndex]->eventDataList.length(); j_event++)
            {
                QString eventCode = QString::number(plotData->eegDataStorage[i_file]->eventDataStorage[eventFileIndex]->eventDataList[j_event][1]);
                QListWidgetItem* eventItem = new QListWidgetItem(eventCode, newFile.eventList);
            }

            fileLayout->addWidget(newFile.eventList);

            newFile.hasEvent = true;
        }

        else
        {
            QLabel* emptyLabel = new QLabel;
            emptyLabel->setText("Events no availbles");
            fileLayout->addWidget(emptyLabel);
        }


        fileLayout->addStretch();

        fileList.append(newFile);

        this->ui->wrapperLayout->addLayout(fileLayout);
    }

    this->ui->wrapperLayout->addStretch();
}


/***************
 * Destructor
 ***************/
SelectAlignmentReferenceWindow::~SelectAlignmentReferenceWindow()
{
    delete ui;
}


//      %%%%%%%%%%%%%%%%%%%%%%%
//      %%%% SLOTS private %%%%
//      %%%%%%%%%%%%%%%%%%%%%%%


/***************
 * Send the event reference that will served as "0" for each file..
 ***************/
void SelectAlignmentReferenceWindow::on_applyButton_clicked()
{
    QVector<int> selectedEvents;

    for (int i_file = 0; i_file < fileList.length(); i_file++)
    {
        if (fileList[i_file].hasEvent)
        {
            selectedEvents.append(fileList[i_file].eventList->currentRow());

            if (selectedEvents[i_file] < 0)
            {
                selectedEvents[i_file] = 0;
            }
        }
    }

    emit sendEventReference(selectedEvents);

    this->~SelectAlignmentReferenceWindow();
}

/***************
 * Call the destructor to close the window
 ***************/
void SelectAlignmentReferenceWindow::on_cancelButton_clicked()
{
    this->~SelectAlignmentReferenceWindow();
}
