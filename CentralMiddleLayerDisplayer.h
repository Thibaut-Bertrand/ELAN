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
 *
 * Author: Thibaut Bertrand
 * thibaut.bertrand@inserm.fr
 ***************/


#ifndef CENTRALMIDDLELAYERDISPLAYER_H
#define CENTRALMIDDLELAYERDISPLAYER_H


#include <QVector>
#include <QString>
#include <QPainter>


#include "DataContainers/ProjectData.h"
#include "EegDataStorage.h"
#include "qcustomplot.h"


class CentralMiddleLayerDisplayer : public QWidget
{

    Q_OBJECT


private:

    // Methods
    //--------------------
    void plotInitialization();
    void setPlotCharacteristics();
    void setGraphColors();


    // Members
    //--------------------
    QCPItemText* plotInfo;
    ProjectData* projectData;


public:

    CentralMiddleLayerDisplayer(QSize size, ProjectData* project);
    ~CentralMiddleLayerDisplayer();


    // Methods (override)
    //--------------------
    void resizeEvent(QResizeEvent *e);


    // Methods
    //--------------------
    void setGraphArrangement();
    void setGraphData();
    void setXAxis();
    void getYAxesPosition();


    // --- Plot information
    void initializePlotInfo();
    void updatePlotInfo(QPointF point, int targetedGraph);


    // Members
    //--------------------

    // Plot
    int graphNb;
    int eegFileNumber = 0;
    double excessRange = 200.;

    QCustomPlot *plot;


    /* int totalGraphPosition: The number of position for the graph.
     * Two different channels (like Fp1 and Fp2) each takes one position.
     * Two similar channel (from different files, like Fp1 from file 1 and Fp1
     * from file 2), takes the same position.
     * A butterfly, no matter the number of channel inside it, take only one
     * position.
     * There two more position (the default 2), which are located at the very
     * top and bottom of the plot. Those are the position of the "overflow"
     * graphs, that are used to draw data that exceed the range of the other
     * graphs (generally happen when the firsts or lats channels have huge
     * spikes or whatnot.
     */
    int totalGraphPosition = 2;
    int excessTopLimit = 100;
    int excessBottomLimit = 100;

    QCPAxis* topAxis;
    QCPAxis* bottomAxis;
    QCPGraph* topGraph;
    QCPGraph* bottomGraph;


    struct channelDisplayElements
    {
        int fileIndex = 0;
        int channelIndex = 0;
        int ID = 0;
        QCPAxis* amplitudeAxis;
        QCPAxis* timeAxis;
        QCPGraph* graph;
    };

    QVector<QVector<channelDisplayElements>> channelDisplayElementsList;


    QVector<QVector<double>> xAxisList; // As each file can have different Sample Frequency, so different Time Scale (= Time of the page * Sample Frequency), each should have its own xAxis.


    // Data
    bool multipleEegFile = false; // Become true if 2 or more .eeg files are loaded


    // | .eeg files | channels | data points |
    QVector<QVector<double>> plotMatrix; // Each .eeg file (first vector) has multiple channels (second vector). Each channel has a data set (third vector)


    // Events
    int eventNb;

    QVector<QVector<QVector<int>>> eventInRange; // List the sample (position) of the event in range to be displayed

    QPainter painter;


public slots:

    // Plot info
    //--------------------
    void displayPlotInfo(int x, int y);


    // Settings
    //--------------------
    void onPresentationDataChanged(size_t hint);
};

#endif // CENTRALMIDDLELAYERDISPLAYER_H
