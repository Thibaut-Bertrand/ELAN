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
 * This class is contains data about the time scale, the page number and the
 * actually displayed page.
 *
 * Author: Thibaut Bertrand
 * thibaut.bertrand@inserm.fr
 ***************/


#include "PageData.h"


/***************
 * Constructor
 ***************/
PageData::PageData()
{
    setTimeFirstSample();
}

//      %%%%%%%%%%%%%%%%%%%%%%%%%
//      %%%% METHODS private %%%%
//      %%%%%%%%%%%%%%%%%%%%%%%%%


/***************
 * Set the time that correspond to the first sample of the window. With this
 * value, when requesting data to display, the eegDataStorage will know which
 * sample is the first to get in the list, which is important when multiple
 * files have different sampling frequency.
 ***************/
void PageData::setTimeFirstSample()
{
    // Classic view mode
    //--------------------
    if (!isViewFocusedOnEvent)
    {
        timeAtFirstSample = (actualPage * timeScale) - timeScale;
    }

    // Event focused view mode
    //--------------------
    else
    {
        // (* 1000) because it's in milliseconds
        int timeAtEventSample = 1000 * (browsableEvents[currentEventFocusIndex][0] / eegFileSamplingFrequency[browsableEvents[currentEventFocusIndex][2]]);

        timeAtFirstSample = timeAtEventSample - (timeScale / 2);
    }
}


//      %%%%%%%%%%%%%%%%%%%%%%%%
//      %%%% METHODS public %%%%
//      %%%%%%%%%%%%%%%%%%%%%%%%


//      ===============
//      %%%% Pages %%%%
//      ===============


/***************
 * Change the current page. Also check if the wanted page exist to begin with.
 * the term page might be a little misleading. It's more like a specific chunk
 * of data.
 *
 * In the classic view mode, a page correspond at the data points
 * contained in the timeScale. For example, if the timeScale is 10 seconds, then
 * the first page will show all the data points that are present in the first 10
 * seconds of each loaded .eeg files. The tenth page will show the seconds 90 to
 * 100 of each files.
 *
 * In the event focused view mode, a page is the chunk of data centered around
 * the currently focused event. For example, if the timeScale is 10 seconds, and
 * the view focus on an event that is positioned at 12 seconds, then the page
 * will show the seconds 7 to 17 (as the vent is centered, the left half is what
 * is before the event, and the second half, what is after).
 *
 * Notes: Do keep in mind that the sampling frequency is differnet for each .eeg
 * file, so the number of data point might be different for each of them.
 *-------------------------
 * int newPage: in the classic view mode, it's simply the wanted page number. In
 * event focused view, it's the index of the event in the vector
 * browsableEvents.
 ***************/
void PageData::pageChange(int newPage)
{
    // Classic view mode
    //--------------------
    if (!isViewFocusedOnEvent)
    {
        if (newPage >= 1 && newPage <= totalPageNb)
        {
            actualPage = newPage;

            setTimeFirstSample();

            emit notifyChange(eHintPageChanged); // -> ProjectData && -> MainWindow
        }
    }

    // Event focused view mode
    //--------------------
    else
    {
        if (newPage >= 1 && newPage < browsableEvents.length())
        {
            currentEventFocusIndex = newPage;

            setTimeFirstSample();

            emit notifyChange(eHintPageChanged); // -> ProjectData && -> MainWindow
        }
    }
}


/***************
 * Change the displayed page (when the user input a key), to the next or
 * previous one. The page existence is checked in the pageChange(int) method.
 *-------------------------
 * QKeyEvent* e: either "Right" (next page) or "Left" (previous page).
 ***************/
void PageData::pageChange(QKeyEvent *e)
{
    // Classic view mode
    //--------------------
    if (!isViewFocusedOnEvent)
    {
        if (e->key() == Qt::Key_Q
                || e->key() == Qt::Key_Left)
        {
            pageChange(actualPage - 1);
        }

        else if (e->key() == Qt::Key_D
                 || e->key() == Qt::Key_Right)
        {
            pageChange(actualPage + 1);
        }
    }

    // Event focused view mode
    //--------------------
    else
    {
        if (e->key() == Qt::Key_Q
                || e->key() == Qt::Key_Left)
        {
            pageChange(currentEventFocusIndex - 1);
        }

        else if (e->key() == Qt::Key_D
                 || e->key() == Qt::Key_Right)
        {
            pageChange(currentEventFocusIndex + 1);
        }
    }
}


/***************
 * Set the total page number depending of the sampling frequency and sample
 * number of all the loaded files and the actual time scale.
 *-------------------------
 * QVector<int> freqList: the list of the sampling frequency of each loaded
 * file.
 * QVector<int> sampList: the list of the sample number of each loaded file.
 ***************/
void PageData::setTotalPageNb(QVector<int> freqList, QVector<int> sampList)
{
    // Compute the total page number for each file
    int tmpPageNb = 0;

    for (int i_file = 0; i_file < freqList.length(); i_file++)
    {
        if ( (sampList[i_file] / (timeScale * ((double)freqList[i_file] / 1000.) ) )  > tmpPageNb)
        {
            tmpPageNb = sampList[i_file] / (timeScale * ((double)freqList[i_file] / 1000) );
        }
    }

    totalPageNb = tmpPageNb;

    emit notifyChange(eHintPageNumberChanged); // -> MainWindow
}


//      ====================
//      %%%% Time scale %%%%
//      ====================


/***************
 * Increase the time scale by the given increment value.
 *-------------------------
 * int increment: value to add/remove to the actual time scale.
 ***************/
void PageData::incrementTimeScale(int increment)
{
    setTimeScale(timeScale + increment);
}


/***************
 * Just reset the time scale to its default value.
 ***************/
void PageData::setDefaultTimeScale()
{
    setTimeScale(defaultTimeScale);
}


/***************
 * Set the time scale to the new value. Also check if the scale is not inferior
 * to the minimum value.
 *-------------------------
 * int newTimeScale: the new value of the time scale.
 ***************/
void PageData::setTimeScale(int newTimeScale)
{
    if (newTimeScale < minimumTimeScale)
    {
        newTimeScale = minimumTimeScale;
    }

    timeScale = newTimeScale;

    setTimeFirstSample();

    emit notifyChange(eHintTimeScaleChanged); // -> ProjectManager && -> MainWindow
}
