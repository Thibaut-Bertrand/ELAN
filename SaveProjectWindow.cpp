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
 * Allow the user to choose which part of the project to save. Unavailable data
 * aren't available to save.
 *
 * Author: Thibaut Bertrand
 * thibaut.bertrand@inserm.fr
 ***************/


#include "SaveProjectWindow.h"
#include "ui_SaveProjectWindow.h"


/***************
 * Constructor
 *-------------------------
 * ProjectData* project: the data container of interest for the class.
 ***************/
SaveProjectWindow::SaveProjectWindow(ProjectData* project, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SaveProjectWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("Save project");

    checkBoxList = this->findChildren<QCheckBox*>(); // Get all the possible checkbox in the window

    projectData = project;

    checkAvailableData();
}


/***************
 * Desturctor
 ***************/
SaveProjectWindow::~SaveProjectWindow()
{
    delete ui;
}


//      %%%%%%%%%%%%%%%%%%%%%%%%%
//      %%%% METHODS private %%%%
//      %%%%%%%%%%%%%%%%%%%%%%%%%


/***************
 * Check the available data in the PlotData data container. The only data that
 * can be unavaible are: data files, event files and butterfly plots. The rest
 * is here by default.
 ***************/
void SaveProjectWindow::checkAvailableData()
{
    // Check if the user has load at leat one data file
    if (projectData->plotDataContainer->eegFileNb == 0)
    {
        this->ui->dataFilesCheckbox->setEnabled(false);
        this->ui->eventFilesCheckbox->setEnabled(false);
        this->ui->eventStatesCheckbox->setEnabled(false);
        this->ui->butterflyCheckbox->setEnabled(false);
    }


    // Check if the user has load at leat one event file
    else
    {
        bool hasEventLoaded = false;

        for (int i_file = 0; i_file < projectData->plotDataContainer->eegDataStorage.length(); i_file++)
        {
            if (projectData->plotDataContainer->eegDataStorage[i_file]->eventDataStorage.length() > 0)
            {
                hasEventLoaded = true;
                break;
            }
        }

        if (!hasEventLoaded)
        {
            this->ui->eventFilesCheckbox->setEnabled(false);
            this->ui->eventStatesCheckbox->setEnabled(false);
        }
    }


    // Check if the user has made any butterfly plots
    if (projectData->plotDataContainer->butterflyPlotList.length() == 0)
    {
        this->ui->butterflyCheckbox->setEnabled(false);
    }
}


/***************
 * Write a .xml file containing all or part of the information of the data
 * structure ProjectData, depending of the choices of the user.
 ***************/
void SaveProjectWindow::xmlWrite()
{
    // For info, here's the final structure of the save file in .xml format:

    /*
     * <ROOT> main node
     *
     *
     *    <PROJECT Empty = "bool"> main node
     *
     *       <ProjectCurrentDirectory Folder = "path"/>
     *
     *    </PROJECT>
     *
     *
     *    <EEG Empty = "bool"> main node
     *
     *       <EegFile DataFilename = "path" ActiveEventFile = "int" DataOffset = "int" HeaderFilename = "path"> first list node
     *
     *
     *           <CHANNEL Empty = "bool"> second node
     *
     *               <Channel IsDisplayed = "bool" Scale = "int" Label = "string"/> second list node   -> For each channels in the file
     *               ...
     *               ...
     *               ...
     *
     *           </CHANNEL>
     *
     *
     *           <EVENT Empty = "bool"> second node
     *
     *               <EventFile EventFileName = "path"> second list node
     *
     *                   <EventStates Empty = 0> third node
     *
     *                      <Event Visible = "int" Browsable = "int" Code = "int"/> third list node    -> For each event of the event file
     *                      ...
     *                      ...
     *                      ...
     *
     *                   </EventStates>
     *
     *               </EventFile>    -> For each event file associated with the data file
     *               ...
     *               ...
     *               ...
     *
     *           </EVENT>
     *
     *
     *       </EegFile>    -> For each .eeg or .h5 file loaded
     *       ...
     *       ...
     *       ...
     *
     *
     *   <BUTTERFLY Empty = "bool"> main node
     *
     *      <ButterFlyPlot Name = "string"> first list node
     *
     *          <Channel Channel = "int" File = "int"/> second list node   -> For each channel in the butterfly plot
     *          ...
     *          ...
     *          ...
     *
     *      <!ButterflyPlot>    -> For each created butterfly plot
     *      ...
     *      ...
     *      ...
     *
     *   </BUTTERFLY>
     *
     *
     *    </EEG>
     *
     *
     *    <PAGE Empty = "bool"> main node
     *
     *        <TimeScale Empty = "bool" Scale "int"/> second node
     *
     *        <ViewMode Empty = "bool" Mode = "bool" Page = "int" Event = "int"/> second node
     *
     *    </PAGE>
     *
     *
     *    <PREFERENCES Empty = "bool"> main node
     *
     *        <Background blue = "int" red = "int" green = "int"/> second node
     *
     *    </PREFERENCES>
     *
     *
     * </ROOT>
     */


    // Ask file name
    //--------------------
    QString filename;

    QFileDialog dialog;
    filename = dialog.getSaveFileName(this, tr("Choose save location"), QDir::currentPath() + "/untitled.xml", tr(".xml"));

    qDebug() << "Saving...";

    if (filename != "")
    {
        // Save data
        //--------------------

        // Create a document to write XML
        QDomDocument saveDocument;


        // Root element (must be unique! All other nodes are its children!)
        //--------------------
        QDomElement saveFile = saveDocument.createElement("ROOT");
        saveDocument.appendChild(saveFile);



        //====================
        // First node element : PROJECT
        //====================

        qDebug() << "=== PROJECT";
        QDomElement projectNode = saveDocument.createElement("PROJECT");
        saveFile.appendChild(projectNode);


        // ===== Current directory =====
        qDebug() << "CurrentDirectory";
        QDomElement projectCurrentDirectory = saveDocument.createElement("ProjectCurrentDirectory");

        if (!this->ui->currentDirectoryCheckbox->isChecked())
        {
            projectNode.setAttribute("Empty", true);
        }

        else
        {
            projectNode.setAttribute("Empty", false);
            projectCurrentDirectory.setAttribute("Folder", projectData->projectCurrentDirectory);
        }

        projectNode.appendChild(projectCurrentDirectory);



        //====================
        // Second node element : EEG
        //====================

        // ===== EEG files =====
        // .eeg/.h5 filename, .ent filename, active event file index (default 0), data offset (default at 0)
        qDebug() << "=== EEG";
        QDomElement eegNode = saveDocument.createElement("EEG");
        saveFile.appendChild(eegNode);

        if (!this->ui->dataFilesCheckbox->isChecked())
        {
            eegNode.setAttribute("Empty", true);
        }

        else
        {
            eegNode.setAttribute("Empty", false);


            for (int i_dataFile = 0; i_dataFile < projectData->plotDataContainer->eegFileNb; i_dataFile++)
            {
                qDebug() << "File"<<i_dataFile;
                QDomElement eegFile = saveDocument.createElement("EegFile");
                eegFile.setAttribute("DataFilename", projectData->plotDataContainer->eegDataStorage[i_dataFile]->eegFilename);
                eegFile.setAttribute("HeaderFilename", projectData->plotDataContainer->eegDataStorage[i_dataFile]->entFilename);
                eegFile.setAttribute("ActiveEventFile", projectData->plotDataContainer->eegDataStorage[i_dataFile]->activeEventFileIndex);
                eegFile.setAttribute("DataOffset", projectData->plotDataContainer->eegDataStorage[i_dataFile]->dataOffset);
                eegNode.appendChild(eegFile);


                // ===== Channel states =====
                // label, isDisplayed, range
                qDebug() << "== Channels";
                QDomElement channelNode = saveDocument.createElement("CHANNEL");
                eegFile.appendChild(channelNode);

                if (!this->ui->channelStateCheckbox->isChecked())
                {
                    channelNode.setAttribute("Empty", true);
                }

                else
                {
                    channelNode.setAttribute("Empty", false);

                    for (int j_chan = 0; j_chan < projectData->plotDataContainer->eegDataStorage[i_dataFile]->channelNb; j_chan++)
                    {
                        qDebug() << "Channel"<<j_chan;
                        QDomElement channel = saveDocument.createElement("Channel");
                        channel.setAttribute("Label", projectData->plotDataContainer->eegDataStorage[i_dataFile]->channels[j_chan]->label);
                        channel.setAttribute("IsDisplayed", projectData->plotDataContainer->eegDataStorage[i_dataFile]->channels[j_chan]->isDisplayed);
                        channel.setAttribute("Scale", projectData->plotDataContainer->eegDataStorage[i_dataFile]->channels[j_chan]->amplitudeRange.upper);
                        channelNode.appendChild(channel);
                    }
                }



                // ===== Event files =====
                // .pos filename
                qDebug() << "== EventFile(s)";
                QDomElement eventNode = saveDocument.createElement("EVENT");
                eegFile.appendChild(eventNode);

                if (!this->ui->eventFilesCheckbox->isChecked())
                {
                    eventNode.setAttribute("Empty", true);
                }

                else
                {
                    eventNode.setAttribute("Empty", false);

                    for (int j_eventFile = 0; j_eventFile < projectData->plotDataContainer->eegDataStorage[i_dataFile]->eventDataStorage.length(); j_eventFile++)
                    {
                        qDebug() << "event file"<<j_eventFile;
                        QDomElement eventFile = saveDocument.createElement("EventFile");
                        eventFile.setAttribute("EventFilename", projectData->plotDataContainer->eegDataStorage[i_dataFile]->eventDataStorage[j_eventFile]->eventFilename);
                        eventNode.appendChild(eventFile);


                        // ===== Event States =====
                        // Event (code, visible, browsable)
                        qDebug() << "= Event states";
                        QDomElement eventStates = saveDocument.createElement("EventStates");
                        eventFile.appendChild(eventStates);

                        if (!this->ui->eventStatesCheckbox->isChecked())
                        {
                            eventStates.setAttribute("Empty", true);
                        }

                        else
                        {
                            eventStates.setAttribute("Empty", false);

                            for (int k_event = 0; k_event < projectData->plotDataContainer->eegDataStorage[i_dataFile]->eventDataStorage[j_eventFile]->eventStates.length(); k_event++)
                            {
                                qDebug() << "Event"<<k_event;
                                QDomElement event = saveDocument.createElement("Event");
                                event.setAttribute("Code", projectData->plotDataContainer->eegDataStorage[i_dataFile]->eventDataStorage[j_eventFile]->eventStates[k_event][0]);
                                event.setAttribute("Visible", projectData->plotDataContainer->eegDataStorage[i_dataFile]->eventDataStorage[j_eventFile]->eventStates[k_event][1]);
                                event.setAttribute("Browsable", projectData->plotDataContainer->eegDataStorage[i_dataFile]->eventDataStorage[j_eventFile]->eventStates[k_event][2]);
                                eventStates.appendChild(event);
                            }
                        }
                    }
                }
            }



            //====================
            // Third node element : BUTTERFLY
            //====================
            // ===== Butterfly plots =====
            // Name, Channels (file, chan)
            qDebug() << "=== Butterfly plot(s)";
            QDomElement btfNode = saveDocument.createElement("BUTTERFLY");
            saveFile.appendChild(btfNode);

            if (!this->ui->butterflyCheckbox->isChecked())
            {
                btfNode.setAttribute("Empty", true);
            }

            else
            {
                btfNode.setAttribute("Empty", false);

                for (int j_btf = 0; j_btf < projectData->plotDataContainer->butterflyPlotList.length(); j_btf++)
                {
                    qDebug() << "btf"<<j_btf;
                    QDomElement btf = saveDocument.createElement("ButterflyPlot");
                    btf.setAttribute("Name", projectData->plotDataContainer->butterflyPlotList[j_btf]->name);
                    btfNode.appendChild(btf);

                    for (int k_chan = 0; k_chan < projectData->plotDataContainer->butterflyPlotList[j_btf]->filesAndChannelsID.length(); k_chan++)
                    {
                        qDebug() << "btf chan"<<k_chan;
                        QDomElement btfChannel = saveDocument.createElement("Channel");
                        btfChannel.setAttribute("File", projectData->plotDataContainer->butterflyPlotList[j_btf]->filesAndChannelsID[k_chan][0]);
                        btfChannel.setAttribute("Channel", projectData->plotDataContainer->butterflyPlotList[j_btf]->filesAndChannelsID[k_chan][1]);
                        btf.appendChild(btfChannel);
                    }
                }
            }
        }



        //====================
        // Fourth node element : PAGE
        //====================

        qDebug() << "=== PAGE";
        QDomElement pageNode = saveDocument.createElement("PAGE");
        saveFile.appendChild(pageNode);

        if (!this->ui->pageCheckbox->isChecked())
        {
            pageNode.setAttribute("Empty", true);
        }

        else
        {
            pageNode.setAttribute("Empty", false);


            // ===== Time scale =====
            qDebug() << "== Time scale";
            QDomElement timeScale = saveDocument.createElement("TimeScale");
            pageNode.appendChild(timeScale);

            if (!this->ui->timeScaleCheckbox->isChecked())
            {
                timeScale.setAttribute("Empty", true);
            }

            else
            {
                timeScale.setAttribute("Empty", false);
                timeScale.setAttribute("Scale", projectData->pageDataContainer->timeScale);
            }


            // ===== View =====
            // View mode, time at first sample, event index
            qDebug() << "== View";
            QDomElement viewMode = saveDocument.createElement("ViewMode");
            pageNode.appendChild(viewMode);

            if (!this->ui->viewCheckbox->isChecked())
            {
                viewMode.setAttribute("Empty", true);
            }

            else
            {
                viewMode.setAttribute("Empty", false);
                viewMode.setAttribute("Mode", projectData->pageDataContainer->isViewFocusedOnEvent);
                viewMode.setAttribute("Page", projectData->pageDataContainer->actualPage);
                viewMode.setAttribute("Event", projectData->pageDataContainer->currentEventFocusIndex);
            }
        }



        //====================
        // Fifth node element : PREFERENCES
        //====================

        qDebug() << "=== PREFERENCES";
        QDomElement preferenceNode = saveDocument.createElement("PREFERENCES");
        saveFile.appendChild(preferenceNode);

        if (!this->ui->preferenceCheckbox->isChecked())
        {
            preferenceNode.setAttribute("Empty", true);
        }

        else
        {
            preferenceNode.setAttribute("Empty", false);

            // ===== Background =====
            QDomElement background = saveDocument.createElement("Background");
            background.setAttribute("Red", projectData->presentationDataContainer->backgroundColor.red());
            background.setAttribute("Green", projectData->presentationDataContainer->backgroundColor.green());
            background.setAttribute("Blue", projectData->presentationDataContainer->backgroundColor.blue());
            preferenceNode.appendChild(background);
        }


        // Saving
        //====================
        qDebug() << "=== WRITING";
        QFile file(filename);

        if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            qDebug() << "Open the file for writing failed";
        }

        else
        {
            QTextStream stream(&file);
            stream << saveDocument.toString();
            file.close();
            qDebug() << "Save file writing done.";
        }
    }
}


//      %%%%%%%%%%%%%%%%%%%%%%%
//      %%%% SLOTS private %%%%
//      %%%%%%%%%%%%%%%%%%%%%%%


//      =================
//      %%%% Buttons %%%%
//      =================


/***************
 * Save only the selected data of the window (can be all of them).
 ***************/
void SaveProjectWindow::on_saveButton_clicked()
{
    xmlWrite();

    this->~SaveProjectWindow();
}


/***************
 * Just close the window.
 ***************/
void SaveProjectWindow::on_cancelButton_clicked()
{
    this->~SaveProjectWindow();
}


/***************
 * Select all available data.
 ***************/
void SaveProjectWindow::on_selectAllButton_clicked()
{
    for (int i_box = 0; i_box < checkBoxList.length(); i_box++)
    {
        if (checkBoxList[i_box]->isEnabled())
        {
            checkBoxList[i_box]->setChecked(true);
        }
    }
}


//      ====================
//      %%%% Checkboxes %%%%
//      ====================


/***************
 * Update the checkboxes. See the UI.
 *-------------------------
 * int arg1: the checkbox state.
 ***************/
void SaveProjectWindow::on_dataFilesCheckbox_stateChanged(int arg1)
{
    if (!this->ui->dataFilesCheckbox->isChecked())
    {
        this->ui->channelStateCheckbox->setChecked(false);
        this->ui->eventFilesCheckbox->setChecked(false);
        this->ui->eventStatesCheckbox->setChecked(false);
        this->ui->butterflyCheckbox->setChecked(false);
    }
}


/***************
 * Update the checkboxes. See the UI.
 *-------------------------
 * int arg1: the checkbox state.
 ***************/
void SaveProjectWindow::on_eventFilesCheckbox_stateChanged(int arg1)
{
    if (!this->ui->eventFilesCheckbox->isChecked())
    {
        this->ui->eventStatesCheckbox->setChecked(false);
    }
}


/***************
 * Update the checkboxes. See the UI.
 *-------------------------
 * int arg1: the checkbox state.
 ***************/
void SaveProjectWindow::on_pageCheckbox_stateChanged(int arg1)
{
    if (!this->ui->pageCheckbox->isChecked())
    {
        this->ui->timeScaleCheckbox->setChecked(false);
        this->ui->viewCheckbox->setChecked(false);
    }
}
