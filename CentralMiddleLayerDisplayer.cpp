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
 * This class is used to display data on the central widget of the MainWindow.
 * The CentralMiddleLayerManager control this class. The data comes from the
 * ProjectData data container.
 *
 * Author: Thibaut Bertrand
 * thibaut.bertrand@inserm.fr
 ***************/


#include "CentralMiddleLayerDisplayer.h"


/***************
 * Constructor
 *-------------------------
 * QSize size: the size of the window.
 * ProjectData* projectData: the pointer to the data container of interest for
 * this class.
 ***************/
CentralMiddleLayerDisplayer::CentralMiddleLayerDisplayer(QSize size,
                                                         ProjectData* project)
{
    setGeometry(0, 0, size.width(), size.height());

    projectData = project;

    connect(projectData->presentationDataContainer, SIGNAL(notifyChange(size_t)),
            this, SLOT(onPresentationDataChanged(size_t)));

    plotInitialization();
}





/***************
 * Destructor
 ***************/
CentralMiddleLayerDisplayer::~CentralMiddleLayerDisplayer()
{

}


//      %%%%%%%%%%%%%%%%%%%%%%%%%%
//      %%%% METHODS override %%%%
//      %%%%%%%%%%%%%%%%%%%%%%%%%%


/***************
 * Called when the window is resized by the user.
 * cf Qt documentation.
 ***************/
void CentralMiddleLayerDisplayer::resizeEvent(QResizeEvent *e)
{
    plot->setFixedHeight(this->height());
    plot->replot();
    getYAxesPosition();
}


//      %%%%%%%%%%%%%%%%%%%%%%%%%
//      %%%% METHODS private %%%%
//      %%%%%%%%%%%%%%%%%%%%%%%%%


//      =========================
//      %%%% Plot and Graphs %%%%
//      =========================


/***************
 * Initialize the Plot that will receive the Graphs.
 ***************/
void CentralMiddleLayerDisplayer::plotInitialization()
{
    // Initialize plot
    //--------------------
    plot = new QCustomPlot;
    plot->setOpenGl(true); // Allows to enable OpenGL plot rendering, for increased plotting performance of graphically demanding plots


    // Set plot characteristics
    //--------------------
    setPlotCharacteristics();


    // Layout creation
    //--------------------
    QVBoxLayout *plotLayout = new QVBoxLayout;
    plotLayout->addWidget(plot);
    plotLayout->setMargin(0);
    plotLayout->setSpacing(0);
    plotLayout->addStretch();

    this->setLayout(plotLayout);
}


/***************
 * Plot characteristics have to be sets when creatin the plot or graphs. So just
 * call this methods when doing so.
 ***************/
void CentralMiddleLayerDisplayer::setPlotCharacteristics()
{
    plot->xAxis->setVisible(false);
    plot->yAxis->setVisible(false);
    plot->axisRect()->setAutoMargins(QCP::msNone);
    plot->axisRect()->setMargins(QMargins(0, 10, 0, 0));
    plot->setStyleSheet(("background:hsva(255, 255, 255, 0%);"));
    plot->setBackground(QBrush(Qt::NoBrush));
    plot->setFixedHeight(this->height());

    setGraphColors();
    plot->replot();
}


/***************
 * Set the color of the graĥs. If it's the norrmal view (no butterfly), then
 * each file will have it's own color (like first file is green, and second file
 * is blue, or whatever). That mean that all the channels from a same file will
 * be of the same color.
 * However, if the user wish to see butterfly plots, then each channel in each
 * butterfly will be of different colors. This is quite hard to have real
 * distinct colors when there's more than a dozen of channels in one plot
 * (without doing some ugly random colors), all while keeping them  colorblind-
 * proof.
 * See Presentation data and this link for more info:
 *  https://vis4.net/palettes/#/150|d|00429d,96ffea,ffffe0|ffffe0,ff005e,93003a|1|1
 ***************/
void CentralMiddleLayerDisplayer::setGraphColors()
{
    // Butterfly plots
    //--------------------
    if (projectData->plotDataContainer->areBtfDipslayed)
    {
        int plotNb;
        int colorInterval;
        int currentColor;

        for (int i_btf = 0; i_btf < channelDisplayElementsList.length(); i_btf++)
        {
            // Get the number of plot in each btf
            plotNb = channelDisplayElementsList[i_btf].length();


            // Get the interval to select the colors (there 150 of them)
            colorInterval = (projectData->presentationDataContainer->btfColorList.length() / plotNb);
            currentColor = 0;


            // Set the color to the graphs
            for (int j_chan = 0; j_chan < plotNb; j_chan++)
            {
                QColor color;
                color.setNamedColor(projectData->presentationDataContainer->btfColorList[currentColor]);

                channelDisplayElementsList[i_btf][j_chan].graph->setPen(QPen(color, 1));

                currentColor += colorInterval;
            }
        }
    }


    // Free channels
    //--------------------
    else
    {
        if (channelDisplayElementsList.length() == 1)
        {
            for (int i_chan = 0; i_chan < channelDisplayElementsList[0].length(); i_chan++)
            {
                channelDisplayElementsList[0][i_chan].graph->setPen(QPen(projectData->presentationDataContainer->singleFileColor, 1));
            }
        }


        else if (channelDisplayElementsList.length() > 1)
        {
            for (int i_file = 0; i_file < channelDisplayElementsList.length(); i_file++)
            {
                for (int j_chan = 0; j_chan < channelDisplayElementsList[i_file].length(); j_chan++)
                {
                    channelDisplayElementsList[i_file][j_chan].graph->setPen(QPen(projectData->presentationDataContainer->plotColors[i_file], 1));
                }
            }
        }
    }
}


//      %%%%%%%%%%%%%%%%%%%%%%%%
//      %%%% METHODS public %%%%
//      %%%%%%%%%%%%%%%%%%%%%%%%


/***************
 * Create and arrange the QCPGraphs in the QCustomPlot. All this depnds on the
 * number of loaded .eeg files, the number of their channels that are set active
 * (to be displayed), which and how many channels each files have in common, and
 * if butterfly have to be displayed (and in this case, how many and which
 * channels from which file are in them).
 *
 * NOTE: QCutomPlot is a "plot", that is a container for QCPGraphs. A plot can
 * have as many graphs as needed.
 * A graphs can only contain one set of data (key-value), so there's one graph
 * for each single channel that is drawed. Each graph also have, quite
 * obviously, two main axes (x and y).
 *
 * TODO: make a schema to explain how are drawed the graphs and why we need to
 * have gigantic amplitude range.
 ***************/
void CentralMiddleLayerDisplayer::setGraphArrangement()
{
    // Clean slate
    //--------------------
    plot->clearGraphs();


    totalGraphPosition = 2;


    // Get the number of butterfly position
    if (projectData->plotDataContainer->areBtfDipslayed
            && !projectData->plotDataContainer->butterflyPlotList.isEmpty())
    {
        totalGraphPosition += projectData->plotDataContainer->butterflyPlotList.length();
    }

    else
    {
        totalGraphPosition += projectData->plotDataContainer->channelToDisplayNb;
    }

    int actualPosition = 0; // The position that's actually worked on.


    // Top graph (for excess data)
    //--------------------
    topAxis = plot->axisRect()->addAxis(QCPAxis::atLeft);

    topAxis->setRange(-excessTopLimit - ((totalGraphPosition) - 1 - actualPosition) * (excessTopLimit * 2),
                      excessTopLimit + actualPosition * (excessTopLimit * 2));
    topAxis->setVisible(false);

    topGraph = new QCPGraph(plot->xAxis, topAxis);

    actualPosition++;


    // Graphs
    //--------------------

    channelDisplayElementsList.clear();

    int currentGraph = 1;

    // --- Butterfly plot graphs ---
    if (projectData->plotDataContainer->areBtfDipslayed)
    {
        channelDisplayElementsList.resize(projectData->plotDataContainer->butterflyPlotList.length());

        // For each possible butterfly
        for (int i_btf = 0; i_btf < projectData->plotDataContainer->butterflyPlotList.length(); i_btf++)
        {
            // For each of its channels
            for (int j_chan = 0; j_chan < projectData->plotDataContainer->butterflyPlotList[i_btf]->filesAndChannelsID.length(); j_chan++)
            {
                channelDisplayElements channelElement;


                // Indexes
                channelElement.fileIndex = projectData->plotDataContainer->butterflyPlotList[i_btf]->filesAndChannelsID[j_chan][0];
                channelElement.channelIndex = projectData->plotDataContainer->butterflyPlotList[i_btf]->filesAndChannelsID[j_chan][1];
                channelElement.ID = currentGraph;
                currentGraph++;


                // Aplitude Axis
                channelElement.amplitudeAxis = plot->axisRect()->addAxis(QCPAxis::atLeft);

                QCPRange amplitudeRange = projectData->plotDataContainer->eegDataStorage[channelElement.fileIndex]->channels[channelElement.channelIndex]->amplitudeRange;

                channelElement.amplitudeAxis->setRange(amplitudeRange.lower - ((totalGraphPosition) - 1 - actualPosition) * amplitudeRange.size(),
                                                       amplitudeRange.upper + actualPosition * amplitudeRange.size());
                channelElement.amplitudeAxis->setVisible(false);


                // Time Axis
                channelElement.timeAxis = plot->axisRect()->addAxis(QCPAxis::atBottom);

                channelElement.timeAxis->setRange(0, projectData->plotDataContainer->eegDataStorage[channelElement.fileIndex]->samplePerPage);
                channelElement.timeAxis->setVisible(false);


                // Graph
                channelElement.graph = new QCPGraph(channelElement.timeAxis, channelElement.amplitudeAxis);

                channelDisplayElementsList[i_btf].append(channelElement);
            }

            actualPosition++;
        }
    }


    // --- Free channels graphs ---
    else
    {
        channelDisplayElementsList.resize(projectData->plotDataContainer->eegFileNb);

        // For each possible channel
        for (int i_unique = 0; i_unique < projectData->plotDataContainer->uniqueChannelDisplayedList.length(); i_unique++)
        {
            // If at least one file have this channel displayed
            if (projectData->plotDataContainer->uniqueChannelDisplayedList[i_unique])
            {
                // For each file
                for (int j_file = 0; j_file < projectData->plotDataContainer->eegFileNb; j_file++)
                {
                    // If this file contain the channel
                    if (projectData->plotDataContainer->eegDataStorage[j_file]->labelList.contains(projectData->plotDataContainer->uniqueChannelLabelList[i_unique]))
                    {
                        // Get its index
                        int channelIndex = projectData->plotDataContainer->eegDataStorage[j_file]->labelList.indexOf(projectData->plotDataContainer->uniqueChannelLabelList[i_unique]);

                        // Then, if the channel is set to be displayed
                        if (projectData->plotDataContainer->eegDataStorage[j_file]->channels[channelIndex]->isDisplayed)
                        {
                            channelDisplayElements channelElement;


                            // Indexes
                            channelElement.fileIndex = j_file;
                            channelElement.channelIndex = channelIndex;
                            channelElement.ID = currentGraph;
                            currentGraph++;


                            // Aplitude Axis
                            channelElement.amplitudeAxis = plot->axisRect()->addAxis(QCPAxis::atLeft);

                            QCPRange amplitudeRange = projectData->plotDataContainer->eegDataStorage[j_file]->channels[channelIndex]->amplitudeRange;

                            channelElement.amplitudeAxis->setRange(amplitudeRange.lower - ((totalGraphPosition) - 1 - actualPosition) * amplitudeRange.size(),
                                                                   amplitudeRange.upper + actualPosition * amplitudeRange.size());
                            channelElement.amplitudeAxis->setVisible(false);


                            // Time Axis
                            channelElement.timeAxis = plot->axisRect()->addAxis(QCPAxis::atBottom);

                            channelElement.timeAxis->setRange(0, projectData->plotDataContainer->eegDataStorage[j_file]->samplePerPage);
                            channelElement.timeAxis->setVisible(false);


                            // Graph
                            channelElement.graph = new QCPGraph(channelElement.timeAxis, channelElement.amplitudeAxis);

                            channelDisplayElementsList[j_file].append(channelElement);
                        }
                    }
                }

                actualPosition++;
            }
        }
    }


    // Bottom graph (for excess data)
    //--------------------
    bottomAxis = plot->axisRect()->addAxis(QCPAxis::atLeft);

    bottomAxis->setRange(-excessBottomLimit - ((totalGraphPosition) - 1 - actualPosition) * (excessBottomLimit * 2),
                      excessBottomLimit + actualPosition * (excessBottomLimit * 2));
    bottomAxis->setVisible(false);

    bottomGraph = new QCPGraph(plot->xAxis, bottomAxis);


    // Finalization
    //--------------------
    setPlotCharacteristics();
    initializePlotInfo();
    getYAxesPosition();
}


/***************
 ***************/
void CentralMiddleLayerDisplayer::setGraphData()
{
    // Set data
    //--------------------
    for (int i_file = 0; i_file < channelDisplayElementsList.length(); i_file++)
    {
        for (int j_chan = 0; j_chan < channelDisplayElementsList[i_file].length(); j_chan++)
        {
            int fileIndex = channelDisplayElementsList[i_file][j_chan].fileIndex;
            int channelIndex = channelDisplayElementsList[i_file][j_chan].channelIndex;

            if (projectData->plotDataContainer->isSignalCentered)
            {
                channelDisplayElementsList[i_file][j_chan].graph->setData(xAxisList[fileIndex],
                                                                          projectData->plotDataContainer->eegDataStorage[fileIndex]->dataToDisplayCentered[channelIndex]);
            }

            else
            {
                channelDisplayElementsList[i_file][j_chan].graph->setData(xAxisList[fileIndex],
                                                                          projectData->plotDataContainer->eegDataStorage[fileIndex]->dataToDisplay[channelIndex]);
            }
        }
    }


    // Finalization
    //--------------------
    plot->replot();
}


/***************
 * Initialize the x-axis (time) vector(s).
 ***************/
void CentralMiddleLayerDisplayer::setXAxis()
{
    xAxisList.clear();
    xAxisList.resize(projectData->plotDataContainer->eegFileNb);

    for (int i_file = 0; i_file < xAxisList.length(); i_file++)
    {
        for (int j_point = 0; j_point < projectData->plotDataContainer->eegDataStorage[i_file]->samplePerPage; j_point++)
        {
            xAxisList[i_file].append(j_point);
        }
    }
}


/***************
 * Get the positions, in pixels, of each of the y-axis, that is of each graph.
 * Those values are then used by the ChannelLabelsDisplayers to correctly
 * position the labels in front of the correct graph.
 ***************/
void CentralMiddleLayerDisplayer::getYAxesPosition()
{
    QVector<double> positions;

    for (int i_file = 0; i_file < channelDisplayElementsList.length(); i_file++)
    {
        for (int j_chan = 0; j_chan < channelDisplayElementsList[i_file].length(); j_chan++)
        {
            double position = (channelDisplayElementsList[i_file][j_chan].amplitudeAxis->range().upper * this->height()) / channelDisplayElementsList[i_file][j_chan].amplitudeAxis->range().size();

            if (!positions.contains(position))
            {
                positions.append(position);
            }
        }
    }

    std::sort(positions.begin(), positions.end());

    projectData->horizontalAxesChanged(positions);
}


//      ==========================
//      %%%% Plot information %%%%
//      ==========================


/***************
 * Initialize the widget that will display channel info when holding the Ctrl
 * key.
 ***************/
void CentralMiddleLayerDisplayer::initializePlotInfo()
{
    QCPItemText* tmpInfo = new QCPItemText(plot);

    tmpInfo->position->setType(QCPItemPosition::ptAbsolute);
    tmpInfo->setText("");
    tmpInfo->setPositionAlignment(Qt::AlignLeft);
    tmpInfo->setBrush(QBrush(QColor(240, 222, 173)));
    tmpInfo->setPadding(QMargins(2, 2, 2, 2));

    plotInfo = tmpInfo;
}


/***************
 * Update the info widget showing the channel name when moving the mouse.
 *-------------------------
 * QPointF point: the point in space of the mouse cursor position.
 * int targetedGraph: the graph targeted by the mouse.
 ***************/
void CentralMiddleLayerDisplayer::updatePlotInfo(QPointF point,
                                                 int targetedGraph)
{
    int fileIndex = 0;
    int chanIndex = 0;


    for (int i_file = 0; i_file < channelDisplayElementsList.length(); i_file++)
    {
        for (int j_chan = 0; j_chan < channelDisplayElementsList[i_file].length(); j_chan++)
        {
            if (channelDisplayElementsList[i_file][j_chan].ID == targetedGraph)
            {
                fileIndex = channelDisplayElementsList[i_file][j_chan].fileIndex;
                chanIndex = channelDisplayElementsList[i_file][j_chan].channelIndex;
                break;
            }
        }
    }

    QString infoText = projectData->plotDataContainer->eegDataStorage[fileIndex]->channels[chanIndex]->label;

    plotInfo->setText(infoText);

    point.setX(point.x() + 10);
    plotInfo->position->setCoords(point);

    plot->replot();
}


//      %%%%%%%%%%%%%%%
//      %%%% SLOTS %%%%
//      %%%%%%%%%%%%%%%


//      ==============
//      %%%% Data %%%%
//      ==============


/***************
 * This method is used to display information about the closest graph to the
 * mouse. For that, the Ctrl key have to be pressed, else, nothing is shown.
 * The mouse position (x and y) are sent by the EventCatcherTopLayer.
 *-------------------------
 * int x: the x position of the mouse cursor.
 * int y: the Y position of the mouse cursor.
 ***************/
void CentralMiddleLayerDisplayer::displayPlotInfo(int x, int y)
{
    if (projectData->presentationDataContainer->isCtrlPressed
            && !projectData->plotDataContainer->eegDataStorage.isEmpty())
    {
        plotInfo->setVisible(true);
        QPointF point; // This is for the selectTest method of the QCPGraph
        point.setX(x);
        point.setY(y);

        double actualPosition = this->height(); // The height is use to get the highest value actualPosition can possibly have.
        int targetedGraph = 0;

        for (int i_graph = 0; i_graph < plot->graphCount(); i_graph++)
        {
            if (plot->graph(i_graph)->selectTest(point, false) < actualPosition
                    && plot->graph(i_graph)->selectTest(point, false) >= 0)
            {
                actualPosition = plot->graph(i_graph)->selectTest(point, false);
                targetedGraph = i_graph;
            }
        }

        if (targetedGraph > 0)
        {
            updatePlotInfo(point, targetedGraph);
        }
    }

    else if (!projectData->presentationDataContainer->isCtrlPressed
             && !projectData->plotDataContainer->eegDataStorage.isEmpty())
    {
        plotInfo->setVisible(false);
        plot->replot();
    }
}


//      ==================
//      %%%% Settings %%%%
//      ==================


/***************
 * Receive info from the data container and filter the hints to process only the
 * one of interest
 *-------------------------
 * size_t hint: flag that indicate specific changes in the data container. Allow
 * the method to know if it should take the signal into account or not.
 ***************/
void CentralMiddleLayerDisplayer::onPresentationDataChanged(size_t hint)
{
    if (hint & PresentationData::eHintPlotsChanged)
    {
        setGraphColors();

        /*
         * Note: This slot is called when applying colors in the preferences
         * window (when this note is being written). The plot colors update
         * only when closing this window due to how QCustomPlot works. I don't
         * know if there's a way to update plots when the window is still open.
         */
    }
}
