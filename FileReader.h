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
 * This class is used to read and get data from the provided data file. The
 * ELAN's libraries are used to do so.
 *
 * Author: Thibaut Bertrand
 * thibaut.bertrand@inserm.fr
 ***************/


#ifndef FILEREADER_H
#define FILEREADER_H


#include <QDebug>
#include <QFile>
#include <fstream>
#include <iostream>
#include <QString>
#include <sstream>
#include <tuple>


#include "DataContainers/ChannelData.h"


class FileReader
{
public:

    FileReader();
    ~FileReader();

    // Methods
    //--------------------
    static int getChannelNb(QString filename);
    static int getSampleNb(QString filename);
    static float getSampFreq(QString filename);
    static void getLabel(QString filename, QVector<QString>& labelList);
    static void getUnits(QString filename, QVector<QString>& unitList, int chanNb);

    static void ReadPosFile(QString filename,
                            QVector<QVector<int>>& eventDataList);
    static void SortVector(QVector<QVector<int>>& eventDataList);

    static void yAxisData(QString filename, QVector<QVector<double>>& matrix,
                          int start, int size);


    static std::tuple<int, double>  readAllFile(QString filename, QVector<ChannelData*>& channels);
};

#endif // FILEREADER_H
