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


#include "ButterflyOrganization.h"
#include "ui_ButterflyOrganization.h"


/***************
 * Constructor
 *-------------------------
 * PlotData* plot: the data container of interest.
 ***************/
ButterflyOrganization::ButterflyOrganization(PlotData* plot, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ButterflyOrganization)
{
    ui->setupUi(this);

    this->setWindowTitle("Butterfly plot creation");

    plotData = plot;


    // == PROTOTYPE ==
    createRegions();
    // ===============


    //==============================
    // fileReferenceLayout
    //==============================
    filenameList.clear();

    for (int i_file = 0; i_file < plotData->eegFileNb; i_file++)
    {
        QLabel* fileLabel = new QLabel;
        setMinMaxSize(fileLabel, QSize(35, 20), QSize(35, 20));
        fileLabel->setFont(font);
        fileLabel->setText("File " + QString::number(i_file + 1) + ": ");

        QLabel* fileNameLabel = new QLabel;
        fileNameLabel->setFont(font);
        fileNameLabel->setText(plotData->eegDataStorage[i_file]->eegFilename);

        filenameList.append(fileNameLabel->text());

        QHBoxLayout* layout = new QHBoxLayout;
        layout->addWidget(fileLabel);
        layout->addWidget(fileNameLabel);
        layout->addStretch();

        this->ui->fileReferenceLayout->insertLayout(0, layout);
    }


    //==============================
    // tabWidgets
    //==============================
    // n tab for file (static)
    // m tab for butterfly (! dynamic !), 1 at first

    // File tabWidgets
    //--------------------
    fileTabWidget = new QTabWidget;
    this->ui->fileFrameLayout->addWidget(fileTabWidget);

    for (int i_file = 0; i_file < plotData->eegFileNb; i_file++)
    {
        QScrollArea* scrollArea = new QScrollArea;
        fileScrollAreaList.append(scrollArea);
        fileTabWidget->addTab(scrollArea, "File " + QString::number(i_file + 1));

        QWidget* mainContainer = new QWidget;

        QVBoxLayout* layout = new QVBoxLayout;
        fileLayoutList.append(layout);
        layout->addStretch();

        mainContainer->setLayout(layout);

        scrollArea->setWidget(mainContainer);
        scrollArea->setWidgetResizable(true);
    }


    // Butterfly tabWidget
    //--------------------
    butterflyTabWidget = new QTabWidget;
    this->ui->butterflyFrameLayout->addWidget(butterflyTabWidget);

    QPushButton* addBtfButton = new QPushButton;
    addBtfButton->setText("+");
    addBtfButton->setFont(font);
    setMinMaxSize(addBtfButton, QSize(20, 20), QSize(20, 20));

    connect(addBtfButton, SIGNAL(clicked()),
            this, SLOT(addNewButterfly()));

    butterflyTabWidget->setCornerWidget(addBtfButton, Qt::TopRightCorner);


    //==============================
    // Populate frames
    //==============================

    channelList.resize(fileTabWidget->count());

    // Populate the fileFrame
    //====================

    // Channel labels
    for (int i_file = 0; i_file < plotData->eegFileNb; i_file++)
    {
        channelList[i_file].resize(plotData->eegDataStorage[i_file]->channelNb);

        for (int j_chan = 0; j_chan < plotData->eegDataStorage[i_file]->channelNb; j_chan++)
        {
            QLabel* label = new QLabel;

            label->setText(plotData->eegDataStorage[i_file]->channels[j_chan]->label);
            label->setFont(font);
            setMinMaxSize(label, labelSize, labelSize);

            channelList[i_file][j_chan].label = label;
            channelList[i_file][j_chan].fileID = i_file;
            channelList[i_file][j_chan].chanID = j_chan;
            channelList[i_file][j_chan].type = plotData->eegDataStorage[i_file]->channels[j_chan]->type;
        }
    }


    for (int i_file = 0; i_file < channelList.length(); i_file++)
    {
        QWidget* btfChannelContainer = new QWidget;
        QHBoxLayout* btfLayout = new QHBoxLayout;
        QLabel* emptyLabel = new QLabel;
        setMinMaxSize(emptyLabel, labelSize, labelSize);
        btfLayout->addWidget(emptyLabel);
        btfLayout->addStretch();
        btfChannelLayout.append(btfLayout);
        btfChannelContainer->setLayout(btfLayout);

        fileLayoutList[i_file]->addWidget(btfChannelContainer);

        for (int j_chan = 0; j_chan < channelList[i_file].length(); j_chan++)
        {
            QWidget* channelContainer = new QWidget;

            QHBoxLayout* layout = new QHBoxLayout;
            layout->addWidget(channelList[i_file][j_chan].label);

            layout->addStretch();
            channelList[i_file][j_chan].layout = layout;

            channelContainer->setLayout(layout);

            fileLayoutList[i_file]->addWidget(channelContainer);
        }
    }

    btfChannelLabel.resize(plotData->eegFileNb);

    this->ui->butterflyFrame->setMaximumWidth(labelSize.width() * 8);

    if (!plotData->butterflyPlotList.isEmpty())
    {
        generateExistingPlots();
    }
}


/***************
 * Destructor
 ***************/
ButterflyOrganization::~ButterflyOrganization()
{
    delete ui;
}


//      %%%%%%%%%%%%%%%%%%%%%%%%%
//      %%%% METHODS private %%%%
//      %%%%%%%%%%%%%%%%%%%%%%%%%


/***************
 * If the user already created some plots (or if s-he loaded a project with
 * butterfly plots in it for example), this method is called to generate them in
 * the window.
 ***************/
void ButterflyOrganization::generateExistingPlots()
{
    // First, create the empty plots
    for (int i_btf = 0; i_btf < plotData->butterflyPlotList.length(); i_btf++)
    {
        addNewButterfly();
    }


    // Then, for each of them
    for (int i_btf = 0; i_btf < butterflyNb; i_btf++)
    {
        // Set it's name
        butterflyList[i_btf].btfNameEdit->setText(plotData->butterflyPlotList[i_btf]->name);

        // And check the channel' checkboxes
        for (int j_chan = 0; j_chan < plotData->butterflyPlotList[i_btf]->filesAndChannelsID.length(); j_chan++)
        {
            int fileID = plotData->butterflyPlotList[i_btf]->filesAndChannelsID[j_chan][0];
            int chanID = plotData->butterflyPlotList[i_btf]->filesAndChannelsID[j_chan][1];

            channelList[fileID][chanID].checkboxList[i_btf]->setChecked(true);

            // Update the butterfly sub-window (on the left).
            // If you don't, that's gonna crash... Just sayin'.
            channelBtfSelected(fileID, chanID, i_btf);
        }
    }

    // And that's it! Easy peasy lemon squeezy!
}


/***************
 * Update the left side widget (on the butterfly side) to dipslay which channels
 * are set to be present in this butterfly.
 *-------------------------
 * int btfID = the ID number of the targeted btf.
 ***************/
void ButterflyOrganization::updateBtfPresentChannel(int btfID)
{
    // Clear/Create the layouts for the present channel
    //--------------------
    // Clear
    QLayoutItem* item;

    for (int i_layout = 0; i_layout < butterflyList[btfID].fileLayout.length(); i_layout++)
    {
        while ( (item = butterflyList[btfID].fileLayout[i_layout]->takeAt(0)) != NULL)
        {
            delete item->widget();
            delete item;
        }
    }

    butterflyList[btfID].fileLayout.clear();
    butterflyList[btfID].channels.clear();
    butterflyList[btfID].channels.resize(butterflyList[btfID].targetFiles.size());


    // Create
    for (int i_targ = 0; i_targ < butterflyList[btfID].targetFiles.length(); i_targ++)
    {
        QVBoxLayout* layout = new QVBoxLayout;
        butterflyList[btfID].fileLayout.append(layout);

        QLabel* label = new QLabel("File " + QString::number(butterflyList[btfID].targetFiles[i_targ]));
        setMinMaxSize(label, labelSize, labelSize);
        label->setStyleSheet("font-weight: bold");
        layout->addWidget(label);
        layout->addStretch();

        butterflyList[btfID].presentChannelLayout->addLayout(layout);
    }


    // Populate layouts
    for (int i_file = 0; i_file < butterflyList[btfID].targetFiles.length(); i_file++)
    {
        int insert = 1;

        for (int j_chan = 0; j_chan < butterflyList[btfID].presentChannels[i_file].length(); j_chan++)
        {
            if (plotData->eegDataStorage[butterflyList[btfID].targetFiles[i_file] - 1]->channelList.contains(butterflyList[btfID].presentChannels[i_file][j_chan]))
            {
                QLabel* label = new QLabel;
                label->setText(butterflyList[btfID].presentChannels[i_file][j_chan]);
                setMinMaxSize(label, labelSize, labelSize);

                butterflyList[btfID].channels[i_file].append(label);

                butterflyList[btfID].fileLayout[i_file]->insertWidget(insert, label);

                insert++;
            }
        }
    }
}


/***************
 * When selecting a group of channels in the butterfly subwindow (= not with the
 * "custom" option), this method will update the channels checkboxes in order to
 * know which are selected or not.
 *-------------------------
 * int btfID = the ID number of the targeted btf.
 ***************/
void ButterflyOrganization::updateChannelsCheckboxes(int btfID)
{
    int targetFile = 0;

    // For each file
    for (int i_file = 0; i_file < channelList.length(); i_file++)
    {
        // If the file is concerned
        if (butterflyList[btfID].targetFiles.contains(i_file + 1))
        {
            // For each chan in the concerned file
            for (int j_chan = 0; j_chan < channelList[i_file].length(); j_chan++)
            {
                if (butterflyList[btfID].presentChannels[targetFile].contains(channelList[i_file][j_chan].label->text()))
                {
                    channelList[i_file][j_chan].checkboxList[btfID]->setChecked(true);
                }

                else
                {
                    channelList[i_file][j_chan].checkboxList[btfID]->setChecked(false);
                }
            }

            targetFile++;
        }

        else
        {
            for (int j_chan = 0; j_chan < channelList[i_file].length(); j_chan++)
            {
                channelList[i_file][j_chan].checkboxList[btfID]->setChecked(false);
            }
        }
    }
}


/***************
 * Set the minimum and maximum size of the specified widget to the specified
 * values.
 *-------------------------
 * QWidget *widget: the widget to set the size of.
 * QSize minSize: the minimum size to set.
 * QSize maxSize: the maximum size to set.
 ***************/
void ButterflyOrganization::setMinMaxSize(QWidget *widget, QSize minSize,
                                          QSize maxSize)
{
    widget->setMinimumSize(minSize);
    widget->setMaximumSize(maxSize);
}


/***************
 * Load the selected configuration file and generate the appropriate plots.
 * Configuration files are in the .xml format.
 *-------------------------
 * QString filename = the name of the file to load.
 ***************/
void ButterflyOrganization::xmlRead(QString filename)
{

}


/***************
 * Save the currently created butterfly, with their option, data type...
 * Configuration files are in the .xml format.
 *
 * NOTE: Not sure how to handle the saving/loading of butterfly plots... Like,
 * if the data files are differents, what to do? Just take the channels that
 * exist? But then if there supplementary channels that are not in the save file
 * and that the user wants, s-he will have to go through the channels or
 * recreating new plots, hence nullifying the point of the save file.
 * Could use the regions for the save, but then if the user don't want some
 * channels of the regions?
 * Maybe it's better not to save the butterfly outside of the general project...
 *-------------------------
 * QString filename = the name of the file to save.
 ***************/
void ButterflyOrganization::xmlWrite(QString filename)
{
    qDebug() << "Saving...";

    // Save data
    //--------------------

    // Create a document to write XML
    QDomDocument saveDocument;


    // Root element (must be unique! All other nodes are its children!)
    //--------------------
    QDomElement saveFile = saveDocument.createElement("ROOT");
    saveDocument.appendChild(saveFile);


    for (int i_btf = 0; i_btf < butterflyList.length(); i_btf++)
    {
        // Main node element : BUTTERFLY
        //====================
        qDebug() << "== BUTTERFLY";
        QDomElement btfNode = saveDocument.createElement("BUTTERFLY");
        btfNode.setAttribute("Name", butterflyList[i_btf].btfNameEdit->text());
        saveDocument.appendChild(btfNode);


        // ===== FILE =====
        qDebug() << "== FILES";
        QDomElement filesNode = saveDocument.createElement("FILES");
        btfNode.appendChild(filesNode);

        for (int j_file = 0; j_file < butterflyList[i_btf].fileCheckboxes.length(); j_file++)
        {
            if (butterflyList[i_btf].fileCheckboxes[j_file]->isChecked())
            {
                QDomElement file = saveDocument.createElement("File");
//                file.setAttribute()
                // Proposition : don't save the files, and instead ask
            }
        }


        // ===== DATA TYPE =====
        qDebug() << "== DATA TYPE";
        QDomElement dataNode = saveDocument.createElement("DATA");
        btfNode.appendChild(dataNode);


        // ===== OPTION =====
        qDebug() << "== OPTION";
        QDomElement optionNode = saveDocument.createElement("OPTION");
        btfNode.appendChild(optionNode);


        // ===== CHANNEL GROUP files =====
        qDebug() << "== CHANNEL GROUP";
        QDomElement groupNode = saveDocument.createElement("GROUP");
        btfNode.appendChild(groupNode);


        // ===== SELECTED CHANNELS files =====
        qDebug() << "== SELECTED CHANNEL";
        QDomElement channelNode = saveDocument.createElement("CHANNEL");
        btfNode.appendChild(channelNode);
    }



    // Saving
    //====================
    qDebug() << "== WRITING";
}


/***************
 * PROTOTYPE, FOR TESTING PURPOSES ONLY!
 * Note: for the MEG channel, I used the CTF nomenclature.
 ***************/
void ButterflyOrganization::createRegions()
{
    //====================
    // EEG regions
    //====================

    // EEG occipital: O, PO, Iz
    //--------------------

    // Right (even)
    //----------
    channelRegion occRightRegionEeg;
    occRightRegionEeg.region = "Occipital";
    occRightRegionEeg.type = "EEG";
    occRightRegionEeg.side = "Right";

    for (int i = 2; i <= 10; i = i + 2)
    {
        QString OString = QString("O%1").arg(i);
        occRightRegionEeg.channelNames.append(OString);

        QString POString = QString("PO%1").arg(i);
        occRightRegionEeg.channelNames.append(POString);
    }

    occRightRegionEeg.channelNames.append("Oz");
    occRightRegionEeg.channelNames.append("POz");
    occRightRegionEeg.channelNames.append("Iz");


    // Left (odd)
    //----------
    channelRegion occLeftRegionEeg;
    occLeftRegionEeg.region = "Occipital";
    occLeftRegionEeg.type = "EEG";
    occLeftRegionEeg.side = "Left";

    for (int i = 1; i <= 10; i = i + 2)
    {
        QString OString = QString("O%1").arg(i);
        occLeftRegionEeg.channelNames.append(OString);

        QString POString = QString("PO%1").arg(i);
        occLeftRegionEeg.channelNames.append(POString);
    }

    occLeftRegionEeg.channelNames.append("Oz");
    occLeftRegionEeg.channelNames.append("POz");
    occLeftRegionEeg.channelNames.append("Iz");


    // EEG parietal: P, CP, C
    //--------------------

    // Right (even)
    //----------
    channelRegion parRightRegionEeg;
    parRightRegionEeg.region = "Parietal";
    parRightRegionEeg.type = "EEG";
    parRightRegionEeg.side = "Right";

    for (int i = 2; i <= 10; i = i + 2)
    {
        QString PString = QString("P%1").arg(i);
        parRightRegionEeg.channelNames.append(PString);

        QString CPString = QString("CP%1").arg(i);
        parRightRegionEeg.channelNames.append(CPString);

        QString CString = QString("C%1").arg(i);
        parRightRegionEeg.channelNames.append(CString);
    }

    parRightRegionEeg.channelNames.append("Pz");
    parRightRegionEeg.channelNames.append("CPz");
    parRightRegionEeg.channelNames.append("Cz");


    // Left (odd)
    //----------
    channelRegion parLeftRegionEeg;
    parLeftRegionEeg.region = "Parietal";
    parLeftRegionEeg.type = "EEG";
    parLeftRegionEeg.side = "Left";

    for (int i = 1; i <= 10; i = i + 2)
    {
        QString PString = QString("P%1").arg(i);
        parLeftRegionEeg.channelNames.append(PString);

        QString CPString = QString("CP%1").arg(i);
        parLeftRegionEeg.channelNames.append(CPString);

        QString CString = QString("C%1").arg(i);
        parLeftRegionEeg.channelNames.append(CString);
    }

    parLeftRegionEeg.channelNames.append("Pz");
    parLeftRegionEeg.channelNames.append("CPz");
    parLeftRegionEeg.channelNames.append("Cz");


    // EEG temporal: T, TP, FT, A
    //--------------------

    // Right (even)
    //----------
    channelRegion temRightRegionEeg;
    temRightRegionEeg.region = "Temporal";
    temRightRegionEeg.type = "EEG";
    temRightRegionEeg.side = "Right";

    for (int i = 2; i <= 10; i = i + 2)
    {
        QString TString = QString("T%1").arg(i);
        temRightRegionEeg.channelNames.append(TString);

        QString FTString = QString("FT%1").arg(i);
        temRightRegionEeg.channelNames.append(FTString);

        QString TPtring = QString("TP%1").arg(i);
        temRightRegionEeg.channelNames.append(TPtring);
    }

    temRightRegionEeg.channelNames.append("RPA");
    temRightRegionEeg.channelNames.append("LPA");


    // Left (odd)
    //----------
    channelRegion temLeftRegionEeg;
    temLeftRegionEeg.region = "Temporal";
    temLeftRegionEeg.type = "EEG";
    temLeftRegionEeg.side = "Left";

    for (int i = 1; i <= 10; i = i + 2)
    {
        QString TString = QString("T%1").arg(i);
        temLeftRegionEeg.channelNames.append(TString);

        QString FTString = QString("FT%1").arg(i);
        temLeftRegionEeg.channelNames.append(FTString);

        QString TPtring = QString("TP%1").arg(i);
        temLeftRegionEeg.channelNames.append(TPtring);
    }

    temLeftRegionEeg.channelNames.append("RPA");
    temLeftRegionEeg.channelNames.append("LPA");


    // EEG frontal: Fp, F, FC, AF, Nz
    //--------------------

    // Right (even)
    //----------
    channelRegion froRightRegionEeg;
    froRightRegionEeg.region = "Frontal";
    froRightRegionEeg.type = "EEG";
    froRightRegionEeg.side = "Right";

    for (int i = 2; i <= 10; i = i + 2)
    {
        QString FpString = QString("Fp%1").arg(i);
        froRightRegionEeg.channelNames.append(FpString);

        QString AFString = QString("AF%1").arg(i);
        froRightRegionEeg.channelNames.append(AFString);

        QString Ftring = QString("F%1").arg(i);
        froRightRegionEeg.channelNames.append(Ftring);

        QString FCtring = QString("FC%1").arg(i);
        froRightRegionEeg.channelNames.append(FCtring);
    }

    froRightRegionEeg.channelNames.append("Nz");
    froRightRegionEeg.channelNames.append("Fpz");
    froRightRegionEeg.channelNames.append("Fz");
    froRightRegionEeg.channelNames.append("FCz");


    // Left (odd)
    //----------
    channelRegion froLeftRegionEeg;
    froLeftRegionEeg.region = "Frontal";
    froLeftRegionEeg.type = "EEG";
    froLeftRegionEeg.side = "Left";

    for (int i = 1; i <= 10; i = i + 2)
    {
        QString FpString = QString("Fp%1").arg(i);
        froLeftRegionEeg.channelNames.append(FpString);

        QString AFString = QString("AF%1").arg(i);
        froLeftRegionEeg.channelNames.append(AFString);

        QString Ftring = QString("F%1").arg(i);
        froLeftRegionEeg.channelNames.append(Ftring);

        QString FCtring = QString("FC%1").arg(i);
        froLeftRegionEeg.channelNames.append(FCtring);
    }

    froLeftRegionEeg.channelNames.append("Nz");
    froLeftRegionEeg.channelNames.append("Fpz");
    froLeftRegionEeg.channelNames.append("Fz");
    froLeftRegionEeg.channelNames.append("FCz");


    //====================
    // MEG regions
    //====================

    // MEG occipital: MRO, MLO, MZO
    //--------------------

    // Right
    //----------
    channelRegion occRightRegionMeg;
    occRightRegionMeg.region = "Occipital";
    occRightRegionMeg.type = "MEG";
    occRightRegionMeg.side = "Right";

    for (int i = 1; i <= 9; i++)
    {
        for (int j = 1; j <= 9; j++)
        {
            QString MROString = QString("MRO%1%2").arg(i).arg(j);
            occRightRegionMeg.channelNames.append(MROString);
        }
    }

    occRightRegionMeg.channelNames.append("MZO01");
    occRightRegionMeg.channelNames.append("MZO02");


    // Left
    //----------
    channelRegion occLeftRegionMeg;
    occLeftRegionMeg.region = "Occipital";
    occLeftRegionMeg.type = "MEG";
    occLeftRegionMeg.side = "Left";

    for (int i = 1; i <= 9; i++)
    {
        for (int j = 1; j <= 9; j++)
        {
            QString MLOString = QString("MLO%1%2").arg(i).arg(j);
            occLeftRegionMeg.channelNames.append(MLOString);
        }
    }

    occLeftRegionMeg.channelNames.append("MZO01");
    occLeftRegionMeg.channelNames.append("MZO02");


    // MEG parietal: MRP, MLP, MZP
    //--------------------

    // Right
    //----------
    channelRegion parRightRegionMeg;
    parRightRegionMeg.region = "Parietal";
    parRightRegionMeg.type = "MEG";
    parRightRegionMeg.side = "Right";

    for (int i = 1; i <= 9; i++)
    {
        for (int j = 1; j <= 9; j++)
        {
            QString MRPString = QString("MRP%1%2").arg(i).arg(j);
            parRightRegionMeg.channelNames.append(MRPString);
        }
    }

    parRightRegionMeg.channelNames.append("MZP01");
    parRightRegionMeg.channelNames.append("MZP02");


    // Left
    //----------
    channelRegion parLeftRegionMeg;
    parLeftRegionMeg.region = "Parietal";
    parLeftRegionMeg.type = "MEG";
    parLeftRegionMeg.side = "Left";

    for (int i = 1; i <= 9; i++)
    {
        for (int j = 1; j <= 9; j++)
        {
            QString MLPString = QString("MLP%1%2").arg(i).arg(j);
            parLeftRegionMeg.channelNames.append(MLPString);
        }
    }

    parLeftRegionMeg.channelNames.append("MZP01");
    parLeftRegionMeg.channelNames.append("MZP02");


    // MEG temporal: MRT, MLT
    //--------------------

    // Right
    //----------
    channelRegion temRightRegionMeg;
    temRightRegionMeg.region = "Temporal";
    temRightRegionMeg.type = "MEG";
    temRightRegionMeg.side = "Right";

    for (int i = 1; i <= 9; i++)
    {
        for (int j = 1; j <= 9; j++)
        {
            QString MRTString = QString("MRT%1%2").arg(i).arg(j);
            temRightRegionMeg.channelNames.append(MRTString);
        }
    }


    // Left
    //----------
    channelRegion temLeftRegionMeg;
    temLeftRegionMeg.region = "Temporal";
    temLeftRegionMeg.type = "MEG";
    temLeftRegionMeg.side = "Left";

    for (int i = 1; i <= 9; i++)
    {
        for (int j = 1; j <= 9; j++)
        {
            QString MLPString = QString("MLT%1%2").arg(i).arg(j);
            temLeftRegionMeg.channelNames.append(MLPString);
        }
    }


    // MEG frontal: MRF, MLF, MZF
    //--------------------

    // Right
    //----------
    channelRegion froRightRegionMeg;
    froRightRegionMeg.region = "Frontal";
    froRightRegionMeg.type = "MEG";
    froRightRegionMeg.side = "Right";

    for (int i = 1; i <= 9; i++)
    {
        for (int j = 1; j <= 9; j++)
        {
            QString MRFString = QString("MRF%1%2").arg(i).arg(j);
            froRightRegionMeg.channelNames.append(MRFString);
        }
    }

    froRightRegionMeg.channelNames.append("MZF01");
    froRightRegionMeg.channelNames.append("MZF02");
    froRightRegionMeg.channelNames.append("MZF03");


    // Left
    //----------
    channelRegion froLeftRegionMeg;
    froLeftRegionMeg.region = "Frontal";
    froLeftRegionMeg.type = "MEG";
    froLeftRegionMeg.side = "Left";

    for (int i = 1; i <= 9; i++)
    {
        for (int j = 1; j <= 9; j++)
        {
            QString MLFString = QString("MLF%1%2").arg(i).arg(j);
            froLeftRegionMeg.channelNames.append(MLFString);
        }
    }

    froLeftRegionMeg.channelNames.append("MZF01");
    froLeftRegionMeg.channelNames.append("MZF02");
    froLeftRegionMeg.channelNames.append("MZF03");


    // MEG central: MRC, MLC, MZC
    //--------------------

    // Right
    //----------
    channelRegion cenRightRegionMeg;
    cenRightRegionMeg.region = "Central";
    cenRightRegionMeg.type = "MEG";
    cenRightRegionMeg.side = "Right";

    for (int i = 1; i <= 9; i++)
    {
        for (int j = 1; j <= 9; j++)
        {
            QString MRCString = QString("MRC%1%2").arg(i).arg(j);
            cenRightRegionMeg.channelNames.append(MRCString );
        }
    }

    cenRightRegionMeg.channelNames.append("MZC01");
    cenRightRegionMeg.channelNames.append("MZC02");


    // Left
    //----------
    channelRegion cenLeftRegionMeg;
    cenLeftRegionMeg.region = "Central";
    cenLeftRegionMeg.type = "MEG";
    cenLeftRegionMeg.side = "Left";

    for (int i = 1; i <= 9; i++)
    {
        for (int j = 1; j <= 9; j++)
        {
            QString MLCString = QString("MLC%1%2").arg(i).arg(j);
            cenLeftRegionMeg.channelNames.append(MLCString);
        }
    }

    cenLeftRegionMeg.channelNames.append("MZC01");
    cenLeftRegionMeg.channelNames.append("MZC02");



    //====================
    // Saving
    //====================

    // EEG
    //--------------------
    regions.append(occRightRegionEeg);
    regions.append(occLeftRegionEeg);

    regions.append(parRightRegionEeg);
    regions.append(parLeftRegionEeg);

    regions.append(temRightRegionEeg);
    regions.append(temLeftRegionEeg);

    regions.append(froRightRegionEeg);
    regions.append(froLeftRegionEeg);


    // MEG
    //--------------------
    regions.append(occRightRegionMeg);
    regions.append(occLeftRegionMeg);

    regions.append(parRightRegionMeg);
    regions.append(parLeftRegionMeg);

    regions.append(temRightRegionMeg);
    regions.append(temLeftRegionMeg);

    regions.append(froRightRegionMeg);
    regions.append(froLeftRegionMeg);

    regions.append(cenRightRegionMeg);
    regions.append(cenLeftRegionMeg);

}


//      %%%%%%%%%%%%%%%%%%%%%%%
//      %%%% SLOTS private %%%%
//      %%%%%%%%%%%%%%%%%%%%%%%


/***************
 * On the right side of the screen. Allow the user to organize the butterfly
 * plots, that is, select for each of them what channel they receive, depending
 * of the selected option (by type, laterality and/or region).
 ***************/
void ButterflyOrganization::addNewButterfly()
{
    butterflyNb++;


    // Update channels (right window' side)
    //==============================

    for (int i_file = 0; i_file < fileLayoutList.length(); i_file++)
    {
        QLabel* btfLabel = new QLabel("Btf " + QString::number(butterflyNb));
        setMinMaxSize(btfLabel, labelSize, labelSize);
        btfLabel->setFont(smallFont);
        btfChannelLabel[i_file].append(btfLabel);
        btfChannelLayout[i_file]->insertWidget(btfChannelLayout[i_file]->count() - 1, btfLabel);

        for (int j_chan = 0; j_chan < channelList[i_file].length(); j_chan++)
        {
            CheckboxID* checkbox = new CheckboxID(i_file, j_chan, butterflyNb - 1);
            setMinMaxSize(checkbox, labelSize, labelSize);

            connect(checkbox, SIGNAL(sendThreeID(int, int, int)),
                    this, SLOT(channelBtfSelected(int, int, int)));

            channelList[i_file][j_chan].checkboxList.append(checkbox);

            channelList[i_file][j_chan].layout->insertWidget(channelList[i_file][j_chan].layout->count() - 1, checkbox);
        }
    }


    // Create butterfly tab (left window' side)
    //==============================
    butterflyPlot newButterfly;
    newButterfly.ID = butterflyNb;


    // Tab
    //--------------------
    QWidget* mainContainer = new QWidget;
    butterWidgetList.append(mainContainer);
    butterflyTabWidget->addTab(mainContainer, "Btf " + QString::number(butterflyNb));


    // Create layouts
    //--------------------
    QHBoxLayout* mainLayout = new QHBoxLayout;

    QVBoxLayout* leftLayout = new QVBoxLayout;

    QHBoxLayout* nameLayout = new QHBoxLayout;
    QHBoxLayout* fileLayout = new QHBoxLayout;
    QHBoxLayout* typeLayout = new QHBoxLayout;
    QHBoxLayout* optionLayout = new QHBoxLayout;
    QHBoxLayout* addAllLayout = new QHBoxLayout;
    QHBoxLayout* channelLayout = new QHBoxLayout;

    QVBoxLayout* rightLayout = new QVBoxLayout;


    // Layout caracteristics
    //--------------------
    int margin = 15;
    nameLayout->setContentsMargins(0, margin, 0, margin);
    fileLayout->setContentsMargins(0, margin, 0, margin);
    typeLayout->setContentsMargins(0, margin, 0, margin);
    optionLayout->setContentsMargins(0, margin, 0, margin);
    addAllLayout->setContentsMargins(0, margin, 0, margin);
    channelLayout->setContentsMargins(0, margin, 0, margin);


    // Place layouts
    //--------------------
    mainContainer->setLayout(mainLayout);
    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(rightLayout);

    leftLayout->addLayout(nameLayout);
    leftLayout->addLayout(fileLayout);
    leftLayout->addLayout(typeLayout);
    leftLayout->addLayout(optionLayout);
    leftLayout->addLayout(addAllLayout);
    leftLayout->addLayout(channelLayout);
    leftLayout->addStretch();


    // Poputlate nameLayout
    //--------------------
    newButterfly.btfNameEdit = new LineEditID(butterflyNb);
    newButterfly.btfNameEdit->setText("Btf " + QString::number(butterflyNb));
    setMinMaxSize(newButterfly.btfNameEdit, QSize(250, 20), QSize(250, 20));

    connect(newButterfly.btfNameEdit, SIGNAL(sendTextChanged(int, QString)),
            this, SLOT(updateBtfName(int, QString)));

    nameLayout->addWidget(newButterfly.btfNameEdit);
    nameLayout->addStretch();


    // Poputlate fileLayout
    //--------------------
    QLabel* labelFile = new QLabel;
    labelFile->setText("File: ");
    setMinMaxSize(labelFile, labelSize, labelSize);
    fileLayout->addWidget(labelFile);


    // --- Scrollarea
    QScrollArea* scrollFile = new QScrollArea;
    scrollFile->setMaximumHeight(100);
    QWidget* mainContainerFile = new QWidget;
    QVBoxLayout* layoutFileSelection = new QVBoxLayout;

    mainContainerFile->setLayout(layoutFileSelection);

    scrollFile->setWidget(mainContainerFile);
    scrollFile->setWidgetResizable(true);

    layoutFileSelection->addStretch();
    fileLayout->addWidget(scrollFile);
    fileLayout->addStretch();


    // --- Checkboxes
    CheckboxID* allFileCheckbox = new CheckboxID(butterflyNb, 0, "File");
    allFileCheckbox->setText("All Files");
    setMinMaxSize(allFileCheckbox, QSize(140, 20), QSize(140, 20));
    allFileCheckbox->setChecked(true);

    connect(allFileCheckbox, SIGNAL(sendInfoShort(int, int)),
            this, SLOT(btfFileCheckboxChange(int)));

    newButterfly.fileCheckboxes.append(allFileCheckbox);

    layoutFileSelection->insertWidget(0, allFileCheckbox);

    for (int i_file = 0; i_file < fileTabWidget->count(); i_file++)
    {
        CheckboxID* fileCheckbox = new CheckboxID(butterflyNb, i_file + 1, "File");
        fileCheckbox->setText("File " + QString::number(i_file + 1));
        setMinMaxSize(fileCheckbox, QSize(140, 20), QSize(140, 20));

        connect(fileCheckbox, SIGNAL(sendInfoShort(int, int)),
                this, SLOT(btfFileCheckboxChange(int)));

        newButterfly.fileCheckboxes.append(fileCheckbox);

        layoutFileSelection->insertWidget(i_file + 1, fileCheckbox);
    }


    // Poputlate typeLayout
    //--------------------
    typeLayout->addWidget(new QLabel("Type: "));

    newButterfly.typeCombobox = new ComboboxID(butterflyNb);
    newButterfly.typeCombobox->addItem("EEG");
    newButterfly.typeCombobox->addItem("MEG");
    setMinMaxSize(newButterfly.typeCombobox, comboboxSize, comboboxSize);

    connect(newButterfly.typeCombobox, SIGNAL(currentIndexChanged(int, int)),
            this, SLOT(changeProposedChannel(int)));

    typeLayout->addWidget(newButterfly.typeCombobox);
    typeLayout->addStretch();


    // Poputlate optionLayout
    //--------------------
    CheckboxID* lateralBox = new CheckboxID(butterflyNb, "Lateral");
    lateralBox->setText("Lateral");
    setMinMaxSize(lateralBox, comboboxSize, comboboxSize);
    lateralBox->setChecked(true);

    CheckboxID* regionBox = new CheckboxID(butterflyNb, "Region");
    regionBox->setText("Region");
    setMinMaxSize(regionBox, comboboxSize, comboboxSize);
    regionBox->setChecked(true);

    connect(lateralBox, SIGNAL(sendInfoShort(int, int)),
            this, SLOT(changeProposedChannel(int)));

    connect(regionBox, SIGNAL(sendInfoShort(int, int)),
            this, SLOT(changeProposedChannel(int)));

    newButterfly.optionCheckboxes.append(lateralBox);
    newButterfly.optionCheckboxes.append(regionBox);

    optionLayout->addWidget(lateralBox);
    optionLayout->addWidget(regionBox);
    optionLayout->addStretch();


    // Poputlate addAllLayout
    //--------------------
    newButterfly.addAllChanButton = new ButtonID(butterflyNb);
    newButterfly.addAllChanButton->setText("Add all channel");
    setMinMaxSize(newButterfly.addAllChanButton, QSize(125, 20), QSize(125, 20));

    connect(newButterfly.addAllChanButton, SIGNAL(sendID(int, int)),
            this, SLOT(addAllChanToBtf(int)));

    addAllLayout->addWidget(newButterfly.addAllChanButton);
    addAllLayout->addStretch();


    // Poputlate channelLayout
    //--------------------
    QLabel* labelChannel = new QLabel;
    labelChannel->setText("Channel: ");
    setMinMaxSize(labelChannel, labelSize, labelSize);
    channelLayout->addWidget(labelChannel);


    // --- Scrollarea
    QScrollArea* scrollChannel= new QScrollArea;
    scrollChannel->setMaximumHeight(200);
    QWidget* mainContainerChannel = new QWidget;
    newButterfly.layoutChannelSelection = new QVBoxLayout;

    mainContainerChannel->setLayout(newButterfly.layoutChannelSelection);

    scrollChannel->setWidget(mainContainerChannel);
    scrollChannel->setWidgetResizable(true);

    newButterfly.layoutChannelSelection->addStretch();
    channelLayout->addWidget(scrollChannel);
    channelLayout->addStretch();


    // QScrollArea for present channel and delete button
    //--------------------

    // --- Delete button
    QHBoxLayout* deleteLayout = new QHBoxLayout;
    newButterfly.deleteButton = new ButtonID(butterflyNb);
    newButterfly.deleteButton->setFirstID(butterflyNb);
    newButterfly.deleteButton->setText("X");
    newButterfly.deleteButton->setFont(font);
    setMinMaxSize(newButterfly.deleteButton, QSize(20, 20), QSize(20, 20));

    connect(newButterfly.deleteButton, SIGNAL(sendID(int, int)),
            this, SLOT(removeButterfly(int)));

    deleteLayout->addStretch();
    deleteLayout->addWidget(newButterfly.deleteButton);

    rightLayout->addLayout(deleteLayout);


    // --- Scrollarea
    QScrollArea* scrollPresentChannel = new QScrollArea;
    QWidget* mainContainerPresentChannel = new QWidget;
    newButterfly.presentChannelLayout = new QHBoxLayout;

    scrollPresentChannel->setMinimumWidth(150);
    scrollPresentChannel->setMaximumWidth(150);
    mainContainerPresentChannel->setLayout(newButterfly.presentChannelLayout);

    scrollPresentChannel->setWidget(mainContainerPresentChannel);
    scrollPresentChannel->setWidgetResizable(true);
    rightLayout->addWidget(scrollPresentChannel);


    butterflyList.append(newButterfly);

    btfFileCheckboxChange(newButterfly.ID);
    changeProposedChannel(butterflyNb);
}


/***************
 * The useer can choose to delete one of the butterfly. This will update the
 * window when she does so.
 *-------------------------
 * int btfID = the ID number of the targeted btf.
 ***************/
void ButterflyOrganization::removeButterfly(int btfID)
{
    butterflyNb--;


    // Update channels (right window' side)
    //==============================

    // --- Remove checkboxes
    for (int i_file = 0; i_file < channelList.length(); i_file++)
    {
        delete btfChannelLabel[i_file][btfID - 1];
        btfChannelLabel[i_file].remove(btfID - 1);

        for (int j_chan = 0; j_chan < channelList[i_file].length(); j_chan++)
        {
            channelList[i_file][j_chan].layout->removeWidget(channelList[i_file][j_chan].checkboxList[btfID - 1]);

            delete channelList[i_file][j_chan].checkboxList[btfID - 1];
            channelList[i_file][j_chan].checkboxList.remove(btfID - 1);
        }
    }


    // --- Update btfLabel
    for (int i_file = 0; i_file < btfChannelLabel.length(); i_file++)
    {
        for (int j_btf = 0; j_btf < btfChannelLabel[i_file].length(); j_btf++)
        {
            if (j_btf >= btfID - 1)
            {
                QString label = "Btf " + QString::number(j_btf + 1);
                btfChannelLabel[i_file][j_btf]->setText(label);
            }
        }
    }


    // --- Update checkboxes
    for (int i_file = 0; i_file < channelList.length(); i_file++)
    {
        for (int j_chan = 0; j_chan < channelList[i_file].length(); j_chan++)
        {
            for (int k_check = 0; k_check < channelList[i_file][j_chan].checkboxList.length(); k_check++)
            {
                if (k_check >= btfID)
                {
                    channelList[i_file][j_chan].checkboxList[k_check]->setID(channelList[i_file][j_chan].checkboxList[k_check]->ID - 1);
                }
            }
        }
    }

    // Create butterfly tab (left window' side)
    //==============================

    // --- Remove things
    butterflyTabWidget->removeTab(btfID - 1);
    butterflyList.remove(btfID - 1);
    butterWidgetList.remove(btfID - 1);


    // --- Update things
    for (int i_btf = 0; i_btf < butterflyList.length(); i_btf++)
    {
        if (butterflyList[i_btf].ID > btfID)
        {
            butterflyList[i_btf].ID = butterflyList[i_btf].ID - 1;
            butterflyList[i_btf].deleteButton->setFirstID(butterflyList[i_btf].deleteButton->firstID - 1);
        }
    }

    for (int i_tab = 0; i_tab < butterflyTabWidget->count(); i_tab++)
    {
        if (i_tab >= btfID - 1)
        {
            updateBtfName(i_tab + 1, butterflyList[i_tab].name);
            butterflyList[i_tab].btfNameEdit->setText(butterflyList[i_tab].name);
        }
    }
}


/***************
 * Update the btf file checkboxes (either all files are selected, or each file
 * are selected individualy).
 *-------------------------
 * int btfID = the ID number of the targeted btf.
 ***************/
void ButterflyOrganization::btfFileCheckboxChange(int btfID)
{
    btfID--;

    butterflyList[btfID].targetFiles.clear();

    // Check whick files are selected
    //--------------------
    // All of them
    if (butterflyList[btfID].fileCheckboxes[0]->isChecked())
    {
        butterflyList[btfID].allFile = true;

        for (int i_box = 1; i_box < butterflyList[btfID].fileCheckboxes.length(); i_box++)
        {
            butterflyList[btfID].fileCheckboxes[i_box]->setCheckState(Qt::Checked);
            butterflyList[btfID].fileCheckboxes[i_box]->setEnabled(false);

            butterflyList[btfID].targetFiles.append(i_box);
        }
    }


    // Part of them
    else if (!butterflyList[btfID].fileCheckboxes[0]->isChecked())
    {
        butterflyList[btfID].allFile = false;

        for (int i_box = 1; i_box < butterflyList[btfID].fileCheckboxes.length(); i_box++)
        {
            butterflyList[btfID].fileCheckboxes[i_box]->setEnabled(true);

            if (butterflyList[btfID].fileCheckboxes[i_box]->isChecked())
            {
                butterflyList[btfID].targetFiles.append(i_box);
            }
        }
    }


    btfChannelSelected(btfID);
}


/***************
 * Change the proposed channel regions whnen the type (EEG/MEG) or the option
 * (lateral/region...) are changed.
 *-------------------------
 * int btfID = the ID number of the targeted btf.
 ***************/
void ButterflyOrganization::changeProposedChannel(int btfID)
{
    btfID--;

    // Clear
    //--------------------
    QLayoutItem* item;
    if (!butterflyList[btfID].channelCheckboxes.isEmpty())
    {
        while ( (item = butterflyList[btfID].layoutChannelSelection->takeAt(0)) != NULL)
        {
            delete item->widget();
            delete item;
        }
    }

    butterflyList[btfID].channelCheckboxes.clear();


    // Laterality only
    //--------------------
    if (butterflyList[btfID].optionCheckboxes[0]->isChecked()
            && !butterflyList[btfID].optionCheckboxes[1]->isChecked())
    {
        CheckboxID* leftCheckbox = new CheckboxID(btfID, 0, "Channel");
        leftCheckbox->setText("Left");
        setMinMaxSize(leftCheckbox, QSize(140, 20), QSize(140, 20));

        butterflyList[btfID].channelCheckboxes.append(leftCheckbox);

        butterflyList[btfID].layoutChannelSelection->addWidget(leftCheckbox);


        CheckboxID* rightCheckbox = new CheckboxID(btfID, 1, "Channel");
        rightCheckbox->setText("Right");
        setMinMaxSize(leftCheckbox, QSize(140, 20), QSize(140, 20));

        butterflyList[btfID].channelCheckboxes.append(rightCheckbox);

        butterflyList[btfID].layoutChannelSelection->addWidget(rightCheckbox);


        butterflyList[btfID].layoutChannelSelection->addStretch();

        connect(leftCheckbox, SIGNAL(sendInfoShort(int , int)),
                this, SLOT(btfChannelSelected(int)));

        connect(rightCheckbox, SIGNAL(sendInfoShort(int , int)),
                this, SLOT(btfChannelSelected(int)));
    }


    // Region only
    //--------------------
    else if (!butterflyList[btfID].optionCheckboxes[0]->isChecked()
            && butterflyList[btfID].optionCheckboxes[1]->isChecked())
    {
        QVector<QString> uniqueRegion;

        for (int i_reg = 0; i_reg < regions.length(); i_reg++)
        {
            if (!uniqueRegion.contains(regions[i_reg].region)
                    && regions[i_reg].type == butterflyList[btfID].typeCombobox->currentText())
            {
                uniqueRegion.append(regions[i_reg].region);
            }
        }

        for (int i_reg = 0; i_reg < uniqueRegion.length(); i_reg++)
        {
            CheckboxID* checkbox = new CheckboxID(btfID, i_reg, "Channel");
            checkbox->setText(uniqueRegion[i_reg]);
            setMinMaxSize(checkbox, QSize(140, 20), QSize(140, 20));

            butterflyList[btfID].channelCheckboxes.append(checkbox);

            butterflyList[btfID].layoutChannelSelection->addWidget(checkbox);

            connect(checkbox, SIGNAL(sendInfoShort(int , int)),
                    this, SLOT(btfChannelSelected(int)));
        }

        butterflyList[btfID].layoutChannelSelection->addStretch();
    }

    // Laterality + Region
    //--------------------
    else if (butterflyList[btfID].optionCheckboxes[0]->isChecked()
            && butterflyList[btfID].optionCheckboxes[1]->isChecked())
    {
        for (int i_reg = 0; i_reg < regions.length(); i_reg++)
        {
            if (regions[i_reg].type == butterflyList[btfID].typeCombobox->currentText())
            {
                CheckboxID* checkbox = new CheckboxID(btfID, i_reg, "Channel");
                checkbox->setText(regions[i_reg].region + " " + regions[i_reg].side);
                setMinMaxSize(checkbox, QSize(140, 20), QSize(140, 20));

                butterflyList[btfID].channelCheckboxes.append(checkbox);

                butterflyList[btfID].layoutChannelSelection->addWidget(checkbox);

                connect(checkbox, SIGNAL(sendInfoShort(int , int)),
                        this, SLOT(btfChannelSelected(int)));
            }
        }

        butterflyList[btfID].layoutChannelSelection->addStretch();
    }

    btfChannelSelected(btfID);
}


/***************
 * Called when a group of channel is selected in the butterfly subwindow (left).
 *-------------------------
 * int btfID = the ID number of the targeted btf.
 ***************/
void ButterflyOrganization::btfChannelSelected(int btfID)
{
    // First, we clear the presentChannels vectors, and give it the correct size
    butterflyList[btfID].presentChannels.clear();
    butterflyList[btfID].presentChannels.resize(butterflyList[btfID].targetFiles.length());


    // Lateral only
    //--------------------
    if (butterflyList[btfID].optionCheckboxes[0]->isChecked()
            && !butterflyList[btfID].optionCheckboxes[1]->isChecked())
    {
        // EEG
        if (butterflyList[btfID].typeCombobox->currentText() == "EEG")
        {
            // For each existing channels region
            for (int i_reg = 0; i_reg < regions.length(); i_reg++)
            {
                // For each checkboxes of the btf
                for (int j_check = 0; j_check < butterflyList[btfID].channelCheckboxes.length(); j_check++)
                {
                    // If both correspond and the box is checked
                    if (regions[i_reg].side == butterflyList[btfID].channelCheckboxes[j_check]->text()
                            && butterflyList[btfID].channelCheckboxes[j_check]->isChecked())
                    {
                        // Add the channel names to all vector (if the user use those option, all targeted file have the same channels, and if s.he don't want that, s.he should use the custom option)
                        for (int k_file = 0; k_file < butterflyList[btfID].presentChannels.length(); k_file++)
                        {
                            butterflyList[btfID].presentChannels[k_file].append(regions[i_reg].channelNames);
                        }
                    }
                }
            }
        }


        // MEG
        else if (butterflyList[btfID].typeCombobox->currentText() == "MEG")
        {
            for (int i_reg = 0; i_reg < regions.length(); i_reg++)
            {
                for (int j_check = 0; j_check < butterflyList[btfID].channelCheckboxes.length(); j_check++)
                {
                    if (regions[i_reg].side == butterflyList[btfID].channelCheckboxes[j_check]->text()
                            && butterflyList[btfID].channelCheckboxes[j_check]->isChecked())
                    {
                        for (int k_file = 0; k_file < butterflyList[btfID].presentChannels.length(); k_file++)
                        {
                            butterflyList[btfID].presentChannels[k_file].append(regions[i_reg].channelNames);
                        }
                    }
                }
            }
        }
    }


    // Region only
    //--------------------
    else if (!butterflyList[btfID].optionCheckboxes[0]->isChecked()
            && butterflyList[btfID].optionCheckboxes[1]->isChecked())
    {
        // EEG
        if (butterflyList[btfID].typeCombobox->currentText() == "EEG")
        {
            for (int i_reg = 0; i_reg < regions.length(); i_reg++)
            {
                for (int j_check = 0; j_check < butterflyList[btfID].channelCheckboxes.length(); j_check++)
                {
                    if (regions[i_reg].region == butterflyList[btfID].channelCheckboxes[j_check]->text()
                            && butterflyList[btfID].channelCheckboxes[j_check]->isChecked())
                    {
                        for (int k_file = 0; k_file < butterflyList[btfID].presentChannels.length(); k_file++)
                        {
                            butterflyList[btfID].presentChannels[k_file].append(regions[i_reg].channelNames);
                        }
                    }
                }
            }
        }

        // MEG
        else if (butterflyList[btfID].typeCombobox->currentText() == "MEG")
        {
            // Not possible?
        }
    }


    // Lateral + Region
    //--------------------
    else if (butterflyList[btfID].optionCheckboxes[0]->isChecked()
            && butterflyList[btfID].optionCheckboxes[1]->isChecked())
    {
        // EEG
        if (butterflyList[btfID].typeCombobox->currentText() == "EEG")
        {
            for (int i_reg = 0; i_reg < regions.length(); i_reg++)
            {
                for (int j_check = 0; j_check < butterflyList[btfID].channelCheckboxes.length(); j_check++)
                {
                    if (butterflyList[btfID].channelCheckboxes[j_check]->text().contains(regions[i_reg].side)
                            && butterflyList[btfID].channelCheckboxes[j_check]->text().contains(regions[i_reg].region)
                            && butterflyList[btfID].channelCheckboxes[j_check]->isChecked())
                    {
                        for (int k_file = 0; k_file < butterflyList[btfID].presentChannels.length(); k_file++)
                        {
                            butterflyList[btfID].presentChannels[k_file].append(regions[i_reg].channelNames);
                        }
                    }
                }
            }
        }

        // MEG
        else if (butterflyList[btfID].typeCombobox->currentText() == "MEG")
        {
            // Not possible?
        }
    }


    updateBtfPresentChannel(btfID);
    updateChannelsCheckboxes(btfID);
}


/***************
 * Called when a channel is selected in the channel subwindow (right).
 *-------------------------
 * int fileID = the ID number of the targeted file.
 * int chanID = the ID number of the targeted channel.
 * int btfID = the ID number of the targeted btf.
 ***************/
void ButterflyOrganization::channelBtfSelected(int fileID, int chanID, int btfID)
{
    // Get file index
    int fileIndex = butterflyList[btfID].targetFiles.indexOf(fileID + 1);

    // If absent, add it
    if (channelList[fileID][chanID].checkboxList[btfID]->isChecked())
    {
        butterflyList[btfID].presentChannels[fileIndex].append(channelList[fileID][chanID].label->text());
    }

    // If present, remove it
    else
    {
        int labelIndex = butterflyList[btfID].presentChannels[fileIndex].indexOf(channelList[fileID][chanID].label->text());
        butterflyList[btfID].presentChannels[fileIndex].remove(labelIndex);
    }


    updateBtfPresentChannel(btfID);
}


/***************
 * Add all possible channel to the butterfly.
 *-------------------------
 * int btfID = the ID number of the targetd butterfly.
 ***************/
void ButterflyOrganization::addAllChanToBtf(int btfID)
{
    btfID--;

    if (!butterflyList[btfID].channelCheckboxes.isEmpty())
    {
        for (int i_box = 0; i_box < butterflyList[btfID].channelCheckboxes.length(); i_box++)
        {
            butterflyList[btfID].channelCheckboxes[i_box]->setChecked(true);
        }


        for (int i_file = 0; i_file < butterflyList[btfID].targetFiles.length(); i_file++)
        {
            int targetFile = butterflyList[btfID].targetFiles[i_file] - 1;

            for (int j_chan = 0; j_chan < channelList[targetFile].length(); j_chan++)
            {
                channelList[targetFile][j_chan].checkboxList[btfID]->setChecked(true);

                channelBtfSelected(targetFile, j_chan, btfID);
            }
        }
    }
}


/***************
 * Receive informations about what set of butterfly plot to generate.
 *-------------------------
 * QString channels = which channels to use (EEG or MEG).
 * QString type = which organization the channels have (Laterality, Regions or
 * both).
***************/
void ButterflyOrganization::generatePlot(int targetFile, QString type, QString channels)
{
    // Count number of btf to generate
    //--------------------

    int newBtfNb = 0;

    if (channels == "Laterality")
    {
        newBtfNb = 2;
    }

    else
    {
        if (type == "EEG")
        {
            if (channels == "Regions")
            {
                newBtfNb = 4;
            }

            else
            {
                newBtfNb = 8;
            }
        }


        else
        {
            if (channels == "Regions")
            {
                newBtfNb = 5;
            }

            else
            {
                newBtfNb = 10;
            }
        }
    }


    // Add btf
    //--------------------
    for (int i_btf = 0; i_btf < newBtfNb; i_btf++)
    {
        addNewButterfly();
    }


    // Modify the new btf to correspond to the selected options
    //--------------------

    // Get the first new btf
    int firstNewBtfIndex = butterflyNb - newBtfNb;

    int channelCheckbox = 0;

    for (int i_btf = firstNewBtfIndex; i_btf < butterflyNb; i_btf++)
    {
        // Change the targeted file
        for (int j_file = 0; j_file < butterflyList[i_btf].fileCheckboxes.length(); j_file++)
        {
            butterflyList[i_btf].fileCheckboxes[j_file]->setChecked(false);
        }

        butterflyList[i_btf].fileCheckboxes[targetFile + 1]->setChecked(true);
        btfFileCheckboxChange(butterflyList[i_btf].ID);

        // Change the data type
        butterflyList[i_btf].typeCombobox->setCurrentText(type);

        // Change the options
        if (channels == "Laterality")
        {
            butterflyList[i_btf].optionCheckboxes[0]->setChecked(true);
            butterflyList[i_btf].optionCheckboxes[1]->setChecked(false);
        }

        else if (channels == "Regions")
        {
            butterflyList[i_btf].optionCheckboxes[0]->setChecked(false);
            butterflyList[i_btf].optionCheckboxes[1]->setChecked(true);
        }

        else
        {
            butterflyList[i_btf].optionCheckboxes[0]->setChecked(true);
            butterflyList[i_btf].optionCheckboxes[1]->setChecked(true);
        }

        changeProposedChannel(butterflyList[i_btf].ID);

        // Change the selected channels
        butterflyList[i_btf].channelCheckboxes[channelCheckbox]->setChecked(true);
        btfChannelSelected(butterflyList[i_btf].ID - 1);

        // Rename the btf
        QString name = type + " " + butterflyList[i_btf].channelCheckboxes[channelCheckbox]->text();
        butterflyList[i_btf].btfNameEdit->setText(name);

        channelCheckbox++;
    }
}


/***************
 * Update the name of the selected butterfly depending of the user input.
 *-------------------------
 * int btfID = the ID number of the btf.
 * QString text = the new text for the btf name.
 ***************/
void ButterflyOrganization::updateBtfName(int btfID, QString text)
{
    btfID--;


    // Change btf name
    butterflyTabWidget->setTabText(btfID, text);

    butterflyList[btfID].name = text;

    // Change btf labels in the file subwindow
    for (int i_file = 0; i_file < btfChannelLabel.length(); i_file++)
    {
        btfChannelLabel[i_file][btfID]->setText(text);
    }
}


/***************
 * Apply the changes made, and allow the construction of the butterfly plots in
 * the central displayer.
 ***************/
void ButterflyOrganization::on_applyButton_clicked()
{
    QVector<QString> plotNames;
    QVector<QVector<QVector<int>>> allPlotsFileAndChannelID;
    /* Just to explain allPlotsFileAndChannelID:
     *
     * - Each butterfly can be composed of any channels from any files, so the
     * first vector<int> is the ID of a single channel with the file ID to which
     * it is associated. Like this: channel ID | file ID.
     *
     * - Each butterfly is composed, by definition, by multiple channels. The
     * vector<vector<int>> is the list of channels from all the channels
     * composing a butterfly plot.
     *
     * - Then, we can have multiple butterfly, and so the
     * vector<vector<vector<int>>> is the list of butterfly plots to create.*/


    // For each buttefly to create
    for (int i_btf = 0; i_btf < butterflyNb; i_btf++)
    {
        // First, add it's name
        plotNames.append(butterflyList[i_btf].btfNameEdit->text());

        QVector<QVector<int>> plotChannelList;

        // Then, look each file
        for (int j_file = 0; j_file < channelList.length(); j_file++)
        {
            // Look each of their channels
            for (int k_chan = 0; k_chan < channelList[j_file].length(); k_chan++)
            {
                // If there are in the targeted butterfly
                if (channelList[j_file][k_chan].checkboxList[i_btf]->isChecked())
                {
                    // Add the chann and file IDs to the list
                    QVector<int> channel{j_file, k_chan};

                    plotChannelList.append(channel);
                }
            }
        }

        allPlotsFileAndChannelID.append(plotChannelList);
    }

    plotData->initializeButterflyList(plotNames, allPlotsFileAndChannelID);
}


/***************
 * Display a warning for the user, preventing to cancel any modification made
 * previously. The user can choose to cancel anyway, of come back.
 ***************/
void ButterflyOrganization::on_cancelButton_clicked()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Discard changes?");
    msgBox.setText("If you click on 'Close', you will lose all the chages"
                   "you've made. To keep your changes, click on 'Cancel', then"
                   "on 'Apply'.\n");
    msgBox.setStandardButtons(QMessageBox::Close | QMessageBox::Cancel);
    int ret = msgBox.exec();

    if (ret == QMessageBox::Close)
    {
        this->~ButterflyOrganization();
    }

    else if (ret == QMessageBox::Cancel)
    {
        // Return to Event Window
        msgBox.close();
    }
}



void ButterflyOrganization::on_loadButton_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, "Choose a file", QDir::currentPath());

    if (filename != "")
    {
        xmlRead(filename);
    }
}


/***************
 * Generate and open a window that will ask the user what set of butterfly plots
 * s-he wants to generate.
 ***************/
void ButterflyOrganization::on_multiplePlotButton_clicked()
{
    multipleBtfWindow = new GenerateMultipleButterfly(filenameList, this);

    connect(this->multipleBtfWindow, SIGNAL(plotToGenerate(int, QString, QString)),
            this, SLOT(generatePlot(int, QString, QString)));

    multipleBtfWindow->show();
}


/***************
 * Ask the user where to save the configuration save file, then call the method
 * to create this file.
 ***************/
void ButterflyOrganization::on_saveButton_clicked()
{
    QString filename;

    QFileDialog dialog;
    filename = dialog.getSaveFileName(this, tr("Choose save location"), QDir::currentPath() + "/untitled.xml", tr(".xml"));

    if (filename != "")
    {
        xmlWrite(filename);
    }
}
