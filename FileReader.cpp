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


#include "FileReader.h"


extern "C"
{
#include "elanfile.h"
}


/***************
 * Constructor
 ***************/
FileReader::FileReader()
{

}


/***************
 * Destructor
 ***************/
FileReader::~FileReader()
{

}


//      %%%%%%%%%%%%%%%%%%%%%%%%
//      %%%% METHODS public %%%%
//      %%%%%%%%%%%%%%%%%%%%%%%%


/***************
 * Return the number of channel.
 *-------------------------
 * QString filename: the path of the data file to read.
 ***************/
int FileReader::getChannelNb(QString filename)
{
    // Load header file
    //--------------------
    elan_struct_t *elanStruct = new elan_struct_t;
    ef_read_elan_header_file((char*)filename.toStdString().c_str(), elanStruct);


    // Get the number of channel
    //--------------------
    int channelNb = elanStruct->chan_nb;
    ef_free_struct(elanStruct);

    return channelNb;
}


/***************
 * Return the number of sample (per channel).
 *-------------------------
 * QString filename: the path of the data file to read.
 ***************/
int FileReader::getSampleNb(QString filename)
{
    // Load header file
    //--------------------
    elan_struct_t *elanStruct = new elan_struct_t;
    ef_read_elan_header_file((char*)filename.toStdString().c_str(), elanStruct);


    // Get the number of sample
    //--------------------
    int sampleNb = elanStruct->eeg.samp_nb;
    ef_free_struct(elanStruct);

    return sampleNb;
}


/***************
 * Get the sampling frequency of the data.
 *-------------------------
 * QString filename: the path of the data file to read.
 ***************/
float FileReader::getSampFreq(QString filename)
{
    setlocale(LC_NUMERIC, "C");

    // Load header file
    //--------------------
    elan_struct_t *elanStruct = new elan_struct_t;
    ef_read_elan_header_file((char*)filename.toStdString().c_str(), elanStruct);


    // Get the sampling frequency
    //--------------------
    float sampFreq = elanStruct->eeg.sampling_freq;
    ef_free_struct(elanStruct);

    return sampFreq;
}


/***************
 * Get the list of labels.
 *-------------------------
 * QString filename: the path of the data file to read.
 * QVector<QString>& unitList: the channel list to populate.
 ***************/
void FileReader::getLabel(QString filename, QVector<QString>& labelList)
{
    // Load file
    //--------------------
    elan_struct_t *elanStruct = new elan_struct_t;
    ef_read_elan_header_file((char*)filename.toStdString().c_str(), elanStruct);
    int channelNb = elanStruct->chan_nb;

    // Get the labels
    //--------------------
    for (int i = 0; i < channelNb; i++)
    {
        QString str(elanStruct->chan_list[i].lab);
        labelList[i] = str;
    }

    ef_free_struct(elanStruct);
}


/***************
 * When loading a .eeg file, this method will look at the associated .ent file
 * (header), in order to get the units of the channels.
 *-------------------------
 * QString filename: the path of the data file to read.
 * QVector<QString>& unitList: the unit list to populate.
 * int chanNb: the number of channel.
 ***************/
void FileReader::getUnits(QString filename, QVector<QString>& unitList,
                          int chanNb)
{
    // Open the .ent file
    //--------------------
    int lastPoint = filename.lastIndexOf(".");
    QString headerFilename = filename.left(lastPoint);
    headerFilename = headerFilename + ".eeg.ent";



//    QString headerFilename = filename + ".ent";

    QFile headerFile(headerFilename);
//    headerFile.open(QIODevice::ReadWrite);
    headerFile.open(QIODevice::ReadOnly);

    // Read the units
    //--------------------

    QTextStream in (&headerFile);
    QString line;

    int i = 0;
    int firstUnitLine = 10 + (2 * (chanNb + 2));
    int lastUnitLine = firstUnitLine + chanNb;

//    QVector<QString> testUnitList;
    do {
        line = in.readLine();

        if (i > lastUnitLine)
        {
            break;
        }

        if (i >= firstUnitLine)
        {
//            testUnitList.append(line);
            if (line.contains("µV", Qt::CaseSensitive)
                    || line.contains("uV", Qt::CaseSensitive)
                    || line.contains("microV", Qt::CaseSensitive)
                    || line.contains("unknown", Qt::CaseSensitive))
            {
                unitList.append("uV");
            }

            else if (line.contains("fT", Qt::CaseSensitive))
            {
                unitList.append("fT");
            }

            else if (line.contains("V", Qt::CaseSensitive))
            {
                unitList.append("V");
            }
        }

        i++;
    } while (!line.isNull());
//    qDebug()<<testUnitList;
}


/***************
 * Use to read the indicated .pos file, which contain the event associated with
 * an .eeg file.
 *-------------------------
 * QString filename: the path of the event file to read.
 * QVector<QVector<int>>& eventDataList: the event list to populate.
 ***************/
void FileReader::ReadPosFile(QString filename,
                             QVector<QVector<int>>& eventDataList)
{
    // Load .pos File
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;


    // Read while it is not the end of the file
    int i = 0;
    while (!file.atEnd())
    {
         // Read the first line. All line have three columns:
        // | Event Sample | Event Code | Reject Code |
        QString line = file.readLine();

        /* For each line, split the string by checking all type of white and
         * empty space. The two value in the line.section method indicate the
         * first and last word to split from the string. As we only want one at
         * a time, both value are the same */
        // First value: Event Sample
        QString eventSample = line.section(QRegExp("\\s+"), 0, 0,
                                         QString::SectionSkipEmpty);
        // Second value: Event Code
        QString eventCode = line.section(QRegExp("\\s+"), 1, 1,
                                          QString::SectionSkipEmpty);
        // Third value: Reject Code
        QString rejectCode = line.section(QRegExp("\\s+"), 2, 2,
                                         QString::SectionSkipEmpty);


        // We then transform the QString into int and put them into the 2D QVector:
        eventDataList.append({eventSample.toInt(),
                             eventCode.toInt(),
                             rejectCode.toInt()});

        i++;
    }


    // As it is possible for the event not to be sorted, we do it there:
    SortVector(eventDataList);
}


/***************
 * It just sort the specified vector.
 *-------------------------
 * QVector<QVector<int>>& eventDataList: the vector to sort.
 ***************/
void FileReader::SortVector(QVector<QVector<int>>& eventDataList)
{
    std::sort(eventDataList.begin(),
              eventDataList.end(),
              [](const QVector<int>& left,
              const QVector<int>& right)->bool{
                if(left.empty() && right.empty())
                    return false;
                if(left.empty())
                    return true;
                if(right.empty())
                    return false;
                return left.first()<right.first();
            }
        );
}


/***************
 * For each channel, put the data that are inside the specified range inside the
 * matrix.
 *-------------------------
 * QString filename: The name of the file to read.
 * QVector<QVector<double>>& matrix: The data container for the data to be
 * displayed on the horizontal axis.
 * int start: The first sample to  read for each channel
 * int interval: The number of sample to read for each channel (= the number of
 * points to display on the horizontal axis)
 ***************/
void FileReader::yAxisData(QString filename, QVector<QVector<double>>& matrix, int start, int interval)
{
    // Read header file
    //--------------------
    elan_struct_t *elanStruct = new elan_struct_t;
    ef_read_elan_header_file((char*)filename.toStdString().c_str(), elanStruct);

    int chanNb = elanStruct->chan_nb;
    int sampNb = chanNb * interval;

    if (elanStruct->eeg.data_type == 0) // float
    {
        float **tmpData = (float**)ef_alloc_array_2d(chanNb, interval, sizeof(float));

        int sampRead = ef_read_EEG_data_sampblock_all_chan((char*)filename.toStdString().c_str(), elanStruct, 0, start, interval, (void***)&tmpData);


        for (int i = 0; i < chanNb; i++)
        {
            for (int j = 0; j < interval; j++)
            {
                matrix[i][j] = (double)tmpData[i][j];
            }
        }
    }


    else if (elanStruct->eeg.data_type == 1) // double
    {
        double **tmpData = (double**)ef_alloc_array_2d(chanNb, interval, sizeof(double));

        int sampRead = ef_read_EEG_data_sampblock_all_chan((char*)filename.toStdString().c_str(), elanStruct, 0, start, interval, (void***)&tmpData);

        if (sampRead != interval)
        {
            qDebug() << "ERROR: Reading data";
            return;
        }


        for (int i = 0; i < chanNb; i++)
        {
            for (int j = 0; j < interval; j++)
            {
                matrix[i][j] = tmpData[i][j];
            }
        }
    }


    ef_free_struct(elanStruct);
}



/***************
 * This method will read the specified header file (.ent) in order to get info
 * for the EegDataStorage.
 *-------------------------
 * QString filename: the name of the .eeg file to read.
 * QVector<ChannelData*>& channels: the list of hte channel list from the
 * EegDataStorage that is loading.
 *
 * return std::tuple<int, double>: return the sample number and the sampling
 * frequency.
 ***************/
std::tuple<int, double>  FileReader::readAllFile(QString filename, QVector<ChannelData*>& channels)
{
    setlocale(LC_NUMERIC, "C");

    elan_struct_t *elanStruct = new elan_struct_t;
    int err = ef_read_elan_header_file((char*)filename.toStdString().c_str(), elanStruct);

    if (err == -1)
    {
        qDebug() << "ERROR: unsupported version";

        ef_free_struct(elanStruct);

        return std::make_tuple(0, 0.);
    }

    else
    {
        // Get sample number
        int sampleNb = elanStruct->eeg.samp_nb;

        // Get sampling frequency
        float sampFreq = elanStruct->eeg.sampling_freq;

        // Channels
        QVector<QString> unitList;
        getUnits(filename, unitList, elanStruct->chan_nb);
        for (int i_chan = 0; i_chan < elanStruct->chan_nb; i_chan++)
        {
            ChannelData* channel = new ChannelData();
            // Get channel labels
            channel->label = QString(elanStruct->chan_list[i_chan].lab);

            // Get channel units
            channel->setUnit(unitList[i_chan]);

            channels.append(channel);
        }

        ef_free_struct(elanStruct);

        return std::make_tuple(sampleNb, sampFreq);
    }
}
