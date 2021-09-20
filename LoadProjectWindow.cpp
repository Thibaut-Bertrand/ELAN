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
 * Allow the user to choose which part of the project to load. Unavailable data
 * aren't available to load.
 *
 * Author: Thibaut Bertrand
 * thibaut.bertrand@inserm.fr
 ***************/


#include "LoadProjectWindow.h"
#include "ui_LoadProjectWindow.h"


/***************
 * Constructor
 *-------------------------
 * QString filename: the file path of the file to laod.
 * ProjectData* project: the data container of interest for the class.
 ***************/
LoadProjectWindow::LoadProjectWindow(QString filename, ProjectData* project,
                                     QWidget* parent) :
    QDialog(parent),
    ui(new Ui::LoadProjectWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("Load project");

    projectData = project;

    filenameToRead = filename;

    checkBoxList = this->findChildren<QCheckBox*>();

    checkAvailableData();
}


/***************
 * Desturctor
 ***************/
LoadProjectWindow::~LoadProjectWindow()
{
    delete ui;
}


//      %%%%%%%%%%%%%%%%%%%%%%%%%
//      %%%% METHODS private %%%%
//      %%%%%%%%%%%%%%%%%%%%%%%%%


/***************
 * Read the xml file to present the data to the users, which will choose what to
 * load or not.
 *-------------------------
 * Note: As the xml parser don't recognize boolean when reading files,
 * false = 0, true = 1.
 ***************/
void LoadProjectWindow::xmlRead()
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


    // Create a document to read XML
    //--------------------
    QDomDocument document;


    // Open a file for reading
    //--------------------
    QFile file(filenameToRead);
    document.setContent(&file);

    qDebug() << "Loading...";


    // Getting root element
    //--------------------
    QDomElement root = document.firstChildElement("ROOT");

    QDomNode mainNode, secondNode, thirdNode;
    QDomNodeList firstListNode, secondListNode, thirdListNode;


    //====================
    // First node element : PROJECT
    //====================
    qDebug() << "=== PROJECT";
    mainNode = root.firstChild(); // -> PROJECT

    if (this->ui->currentDirectoryCheckbox->isChecked())
    {
        secondNode = mainNode.firstChild(); // -> ProjectCurrentDirectory

        projectData->projectCurrentDirectory = QString(secondNode.attributes().namedItem("Folder").nodeValue());
    }


    //====================
    // Second node element : EEG
    //====================
    qDebug() << "=== EEG";
    mainNode = mainNode.nextSibling(); // -> EEG

    // Those two list are for changing the channels states accross all data files
    QVector<QVector<QString>> presentChannels;
    QVector<QVector<bool>> newChannelStates;

    // This one's for the channels' amplitude scales
    QVector<QVector<int>> newScales;


    // ===== EEG files =====
    qDebug() << "== data files";
    if (this->ui->dataFilesCheckbox->isChecked())
    {
        firstListNode = mainNode.childNodes(); // -> EegFile

        presentChannels.resize(firstListNode.count());
        newChannelStates.resize(firstListNode.count());
        newScales.resize(firstListNode.count());


        for (int i_dataFile = 0; i_dataFile < firstListNode.count(); i_dataFile++)
        {
            qDebug() << "file"<<i_dataFile;
            QString filename = firstListNode.at(i_dataFile).attributes().namedItem("DataFilename").nodeValue();
            projectData->eegFileLoading(filename);


            // ===== Channel states =====
            qDebug() << "= Channel states";
            secondNode = firstListNode.at(i_dataFile).firstChild(); // -> CHANNEL

            if (this->ui->channelStatesCheckbox->isChecked())
            {
                secondListNode = secondNode.childNodes(); // -> Channel

                for (int j_chan = 0; j_chan < secondListNode.count(); j_chan++)
                {
//                    qDebug() << "chan"<<j_chan;
                    presentChannels[i_dataFile].append(secondListNode.at(j_chan).attributes().namedItem("Label").nodeValue());

                    secondListNode.at(j_chan).attributes().namedItem("IsDisplayed").nodeValue().toInt() == 1 ?
                                newChannelStates[i_dataFile].append(true) :
                                newChannelStates[i_dataFile].append(false);

                    newScales[i_dataFile].append(secondListNode.at(j_chan).attributes().namedItem("Scale").nodeValue().toInt());
                }
            }


            // ===== Event files =====
            qDebug() << "= Event files";
            secondNode = secondNode.nextSibling(); // -> EVENT

            if (this->ui->eventFilesCheckbox->isChecked())
            {
                secondListNode = secondNode.childNodes(); // -> EventFile

                for (int j_eventFile = 0; j_eventFile < secondListNode.count(); j_eventFile++)
                {
                    qDebug() << "file"<<j_eventFile;
                    QString eventFilename = secondListNode.at(j_eventFile).attributes().namedItem("EventFilename").nodeValue();
                    projectData->eventFileLoading(eventFilename, i_dataFile);

                    // ===== Event States =====
                    qDebug() << "Event states";
                    if (this->ui->eventStatesCheckbox->isChecked())
                    {
                        projectData->plotDataContainer->eegDataStorage[i_dataFile]->activeEventFileIndex = j_eventFile;

                        thirdNode = secondListNode.at(j_eventFile).firstChild(); // -> EventStates
                        thirdListNode = thirdNode.childNodes(); // -> Event

                        QVector<QVector<bool>> eventStates;
                        eventStates.resize(thirdListNode.count());

                        for (int k_event = 0; k_event < thirdListNode.count(); k_event++)
                        {
//                            qDebug() << "event"<<k_event;
                            thirdListNode.at(k_event).attributes().namedItem("Visible").nodeValue().toInt() == 1 ?
                                        eventStates[k_event].append(true) :
                                        eventStates[k_event].append(false);

                            thirdListNode.at(k_event).attributes().namedItem("Browsable").nodeValue().toInt() == 1 ?
                                        eventStates[k_event].append(true) :
                                        eventStates[k_event].append(false);
                        }

                        projectData->plotDataContainer->changeEventStates(eventStates,
                                                                          i_dataFile);
                    }
                }

                int activeFile = firstListNode.at(i_dataFile).attributes().namedItem("ActiveEventFile").nodeValue().toInt();
                projectData->plotDataContainer->eegDataStorage[i_dataFile]->activeEventFileIndex = activeFile;

                int dataOffset = firstListNode.at(i_dataFile).attributes().namedItem("DataOffset").nodeValue().toInt();
                projectData->plotDataContainer->eegDataStorage[i_dataFile]->dataOffset = dataOffset;
            }
        }


        if (this->ui->channelStatesCheckbox->isChecked())
        {
            // Update the channel states
            projectData->plotDataContainer->changeAllChannelStates(presentChannels,
                                                                   newChannelStates);
            projectData->plotDataContainer->setChannelsAmplitudeScale(newScales);
        }
    }


    //====================
    // Third node element : BUTTERFLY
    //====================
    qDebug() << "=== BUTTERFLY";

    // ===== Butterfly plots =====

    mainNode = mainNode.nextSibling(); // -> BUTTERFLY

    if (this->ui->butterflyCheckbox->isChecked())
    {
        firstListNode = mainNode.childNodes(); // -> ButterflyPlot

        QVector<QString> btfNames;
        QVector<QVector<QVector<int>>> allFilesAndChannelsID;
        int fileID, chanID;

        allFilesAndChannelsID.resize(firstListNode.count());

        for (int i_btf = 0; i_btf < firstListNode.count(); i_btf++)
        {
            qDebug() << "btf"<<i_btf;
            secondListNode = firstListNode.at(i_btf).childNodes(); // -> Channel

            btfNames.append(firstListNode.at(i_btf).attributes().namedItem("Name").nodeValue());

            for (int j_chan = 0; j_chan < secondListNode.count(); j_chan++)
            {
//                qDebug() << "chan"<<j_chan;
                fileID = secondListNode.at(j_chan).attributes().namedItem("File").nodeValue().toInt();
                chanID = secondListNode.at(j_chan).attributes().namedItem("Channel").nodeValue().toInt();
                allFilesAndChannelsID[i_btf].append({fileID, chanID});
            }
        }

        projectData->plotDataContainer->initializeButterflyList(btfNames,
                                                                allFilesAndChannelsID);
    }



    //====================
    // Fourth node element : PAGE
    //====================
    qDebug() << "=== PAGE";
    mainNode = mainNode.nextSibling(); // -> PAGE

    if (this->ui->pageCheckbox->isChecked())
    {
        // ===== Time scale =====
        qDebug() << "== Time scale";
        secondNode = mainNode.firstChild(); // -> TimeScale

        if (this->ui->timeScaleCheckbox->isChecked())
        {
            int newTimeScale = secondNode.attributes().namedItem("Scale").nodeValue().toInt();
            projectData->pageDataContainer->setTimeScale(newTimeScale);
        }


        // ===== View =====
        qDebug() << "== View mode";
        secondNode = secondNode.nextSibling(); // -> ViewMode

        if (this->ui->viewModeCheckbox->isChecked())
        {
            // Hmm... Maybe have to check if files are loaded before that...?
        }
    }


    //====================
    // Fifth node element : PREFERENCES
    //====================
    qDebug() << "=== PREFERENCES";
    mainNode = mainNode.nextSibling(); // -> PREFERENCES

    if (this->ui->preferencesCheckbox->isChecked())
    {
        secondNode = mainNode.firstChild();

        // ===== Background =====
        qDebug() << "== Background";
        QColor newBackgroundColor;
        newBackgroundColor.setRed(secondNode.attributes().namedItem("Red").nodeValue().toInt());
        newBackgroundColor.setGreen(secondNode.attributes().namedItem("Green").nodeValue().toInt());
        newBackgroundColor.setBlue(secondNode.attributes().namedItem("Blue").nodeValue().toInt());
        projectData->presentationDataContainer->setBackgroundColor(newBackgroundColor);
    }


    // Close file
    //--------------------
    file.close();
    qDebug() << "File reading finished";
}


/***************
 * This methods will first quicly read the save file to look which data are
 * avaialble. Depending if a specific chunk of data is present or not, the
 * checkboxes of the window will be enable or not.
 ***************/
void LoadProjectWindow::checkAvailableData()
{
    qDebug() << "Checking data...";
    // Create a document to read XML
    //--------------------
    QDomDocument document;

    QDomNode mainNode, secondNode, thirdNode, fourthNode, fifthNode, sixthNode;


    // Open a file for reading
    //--------------------
    QFile file(filenameToRead);
    document.setContent(&file);


    // Getting root element
    //--------------------
    QDomElement root = document.firstChildElement("ROOT");


    // First node element : PROJECT
    //====================
    qDebug() << "== PROJECT";
    mainNode = root.firstChild(); // -> PROJECT


    // ===== Current directory =====
    secondNode = mainNode.firstChild(); // -> ProjectCurrentDirectory

    if (secondNode.attributes().namedItem("Empty").nodeValue().toInt() == 1)
    {
        this->ui->currentDirectoryCheckbox->setEnabled(false);
    }


    // Second node element : EEG
    //====================
    qDebug() << "== EEG";
    mainNode = mainNode.nextSibling(); // -> EEG

    if (mainNode.attributes().namedItem("Empty").nodeValue().toInt() == 1)
    {
        this->ui->dataFilesCheckbox->setEnabled(false);
        this->ui->channelStatesCheckbox->setEnabled(false);
        this->ui->eventFilesCheckbox->setEnabled(false);
        this->ui->eventStatesCheckbox->setEnabled(false);
        this->ui->butterflyCheckbox->setEnabled(false);
    }

    else
    {
        // ===== EEG files =====
        secondNode = mainNode.firstChild(); // -> EegFile
        thirdNode = secondNode.firstChild(); // -> CHANNEL

        if (thirdNode.attributes().namedItem("Empty").nodeValue().toInt() == 1)
        {
            this->ui->channelStatesCheckbox->setEnabled(false);
        }


        // ===== Event files =====
        thirdNode = thirdNode.firstChild(); // -> EVENT

        if (thirdNode.attributes().namedItem("Empty").nodeValue().toInt() == 1)
        {
            this->ui->eventFilesCheckbox->setEnabled(false);
            this->ui->eventStatesCheckbox->setEnabled(false);
        }

        else
        {
            fourthNode = thirdNode.firstChild(); // -> EventFile
            fifthNode = fourthNode.firstChild(); // -> EventStates

            if (fifthNode.attributes().namedItem("Empty").nodeValue().toInt() == 1)
            {
                this->ui->eventStatesCheckbox->setEnabled(false);
            }
        }


        // ===== Butterfly =====
        secondNode = secondNode.nextSibling(); // -> BUTTERFLY

        if (thirdNode.attributes().namedItem("Empty").nodeValue().toInt() == 1)
        {
            this->ui->butterflyCheckbox->setEnabled(false);
        }
    }



    // Third node element : PAGE
    //====================
    qDebug() << "== PAGE";
    mainNode = mainNode.nextSibling(); // -> PAGE

    if (mainNode.attributes().namedItem("Empty").nodeValue().toInt() == 1)
    {
        this->ui->timeScaleCheckbox->setEnabled(false);
        this->ui->viewModeCheckbox->setEnabled(false);
    }

    else
    {
        secondNode = mainNode.firstChild(); // -> Time scale

        if (secondNode.attributes().namedItem("Empty").nodeValue().toInt() == 1)
        {
            this->ui->timeScaleCheckbox->setEnabled(false);
        }


        secondNode = mainNode.firstChild(); // -> Time scale

        if (secondNode.attributes().namedItem("Empty").nodeValue().toInt() == 1)
        {
            this->ui->viewModeCheckbox->setEnabled(false);
        }
    }


    // Fourth node element : PREFERENCES
    //====================
    qDebug() << "== PREFERENCES";
    mainNode = mainNode.nextSibling(); // -> PREFERENCES

    if (mainNode.attributes().namedItem("Empty").nodeValue().toInt() == 1)
    {
        this->ui->preferencesCheckbox->setEnabled(false);
    }


    // Close file
    //--------------------
    file.close();

    qDebug() << "Data checked!\n\n";
}


//      %%%%%%%%%%%%%%%%%%%%%%%
//      %%%% SLOTS private %%%%
//      %%%%%%%%%%%%%%%%%%%%%%%


//      =================
//      %%%% Buttons %%%%
//      =================


/***************
 * Request the file reading and project laoding.
 ***************/
void LoadProjectWindow::on_loadProjectButton_clicked()
{
    xmlRead();

    this->~LoadProjectWindow();
}


/***************
 * Call the destructor to close the window
 ***************/
void LoadProjectWindow::on_cancelButton_clicked()
{
    this->~LoadProjectWindow();
}


/***************
 * Check all enabled checkboxes of the window.
 ***************/
void LoadProjectWindow::on_selectAllButton_clicked()
{
    for (int i_box = 0; i_box < checkBoxList.length(); i_box++)
    {
        checkBoxList[i_box]->setChecked(true);
    }
}


//      ====================
//      %%%% Checkboxes %%%%
//      ====================


/***************
 * Uncheck the "child" checkboxes. See the UI.
 *-------------------------
 * int arg1: the checkbox state.
 ***************/
void LoadProjectWindow::on_dataFilesCheckbox_stateChanged(int arg1)
{
    if (!this->ui->dataFilesCheckbox->isChecked())
    {
        this->ui->channelStatesCheckbox->setChecked(false);
        this->ui->eventFilesCheckbox->setChecked(false);
        this->ui->eventStatesCheckbox->setChecked(false);
        this->ui->butterflyCheckbox->setChecked(false);
    }
}


/***************
 * Check the "parent" checkbox. See the UI.
 *-------------------------
 * int arg1: the checkbox state.
 ***************/
void LoadProjectWindow::on_channelStatesCheckbox_stateChanged(int arg1)
{
    if (this->ui->channelStatesCheckbox->isChecked())
    {
        this->ui->dataFilesCheckbox->setChecked(true);
    }
}


/***************
 * Uncheck the "child" checkboxes and check the "parent" checkbox. See the UI.
 *-------------------------
 * int arg1: the checkbox state.
 ***************/
void LoadProjectWindow::on_eventFilesCheckbox_stateChanged(int arg1)
{
    if (this->ui->eventFilesCheckbox->isChecked())
    {
        this->ui->dataFilesCheckbox->setChecked(true);
    }

    else
    {
        this->ui->eventStatesCheckbox->setChecked(false);
    }
}


/***************
 * Check the "parent" checkboxes. See the UI.
 *-------------------------
 * int arg1: the checkbox state.
 ***************/
void LoadProjectWindow::on_eventStatesCheckbox_stateChanged(int arg1)
{
    if (this->ui->eventStatesCheckbox->isChecked())
    {
        this->ui->eventFilesCheckbox->setChecked(true);
        this->ui->dataFilesCheckbox->setChecked(true);
    }
}


/***************
 * Check the "parent" checkbox. See the UI.
 *-------------------------
 * int arg1: the checkbox state.
 ***************/
void LoadProjectWindow::on_butterflyCheckbox_stateChanged(int arg1)
{
    if (this->ui->butterflyCheckbox->isChecked())
    {
        this->ui->dataFilesCheckbox->setChecked(true);
    }
}


/***************
 * Uncheck the "child" checkboxes. See the UI.
 *-------------------------
 * int arg1: the checkbox state.
 ***************/
void LoadProjectWindow::on_pageCheckbox_stateChanged(int arg1)
{
    if (!this->ui->pageCheckbox->isChecked())
    {
        this->ui->timeScaleCheckbox->setChecked(false);
        this->ui->viewModeCheckbox->setChecked(false);
    }
}


/***************
 * Check the "parent" checkbox. See the UI.
 *-------------------------
 * int arg1: the checkbox state.
 ***************/
void LoadProjectWindow::on_timeScaleCheckbox_stateChanged(int arg1)
{
    if (this->ui->timeScaleCheckbox->isChecked())
    {
        this->ui->pageCheckbox->setChecked(true);
    }
}


/***************
 * Check the "parent" checkbox. See the UI.
 *-------------------------
 * int arg1: the checkbox state.
 ***************/
void LoadProjectWindow::on_viewModeCheckbox_stateChanged(int arg1)
{
    if (this->ui->viewModeCheckbox->isChecked())
    {
        this->ui->pageCheckbox->setChecked(true);
    }
}
