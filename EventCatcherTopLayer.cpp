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
 * This widget catch and manage input coming from the user, notably the
 * interactions withe the cursors.
 *
 * Author: Thibaut Bertrand
 * thibaut.bertrand@inserm.fr
 ***************/


#include "EventCatcherTopLayer.h"


/***************
 * Constructor
 *-------------------------
 * PresentationData presentation; the data container of interest for this class.
 ***************/
EventCatcherTopLayer::EventCatcherTopLayer(PresentationData* presentation)
{
    presentationData = presentation;

    connect(this->presentationData, SIGNAL(notifyChange(size_t)),
            this, SLOT(onPresentationDataChanged(size_t)));

    dragRubberBand = new QRubberBand(QRubberBand::Rectangle, this);

    setMouseTracking(true);
}


/***************
 * Destructor
 ***************/
EventCatcherTopLayer::~EventCatcherTopLayer()
{

}


//      %%%%%%%%%%%%%%%%%%%%%%%%%%
//      %%%% METHODS override %%%%
//      %%%%%%%%%%%%%%%%%%%%%%%%%%


/***************
 * Draw the rubberband when in drag mode.
 * cf Qt documentation.
 ***************/
void EventCatcherTopLayer::mouseMoveEvent(QMouseEvent *e)
{
    if (presentationData->dragAndDrop)
    {
        dragRubberBand->setGeometry(QRect(dragOrigin,
                                      QPoint(e->pos().x(),
                                             this->height())).normalized());
    }

    emit(sendCursorPosition(e->pos().x(), e->pos().y())); // -> CentralTopLayer -> Wrapper -> CentralMiddleLayerManager -> CentralMiddleLayerDisplayer
 }


/***************
 * Place a cursor after each click. Left-click place the first cursor, and
 * right-click place the second cursor.
 * This function is for when the cursors are une "click mode".
 * cf Qt documentation.
 ***************/
void EventCatcherTopLayer::mousePressEvent(QMouseEvent *e)
{
    if (presentationData->dragAndDrop && e->button() == Qt::LeftButton)
    {
        dragOrigin = QPoint(e->pos().x(), 0);
        dragRubberBand->setGeometry(QRect(dragOrigin, QSize()));
        dragRubberBand->show();
    }


    double tmpCursorOnePosition = presentationData->cursorOneRelativePosition;
    double tmpCursorTwoPosition = presentationData->cursorTwoRelativePosition;


    // Left button = first cursor
    //--------------------
    if(e->button() == Qt::LeftButton)
    {
        cursorOneDisplayed = true;
        presentationData->cursorOneColor.setAlpha(255);

        tmpCursorOnePosition = (double)(100.0 * (double)e->pos().x()) / (double)this->width();


        // If the first cursor is placed after the second (on its left), then
        // their position will be switched, so that the first cursor is always
        // the one on the left.

        if (presentationData->isCursorTwoDisplayed && !presentationData->dragAndDrop)
        {
            if (tmpCursorTwoPosition < tmpCursorOnePosition)
            {
                double tmpPosition = tmpCursorOnePosition;
                tmpCursorOnePosition = tmpCursorTwoPosition;
                tmpCursorTwoPosition = tmpPosition;
            }
        }
    }

    // Right button = second cursor
    //--------------------
    else if(!presentationData->dragAndDrop && e->button() == Qt::RightButton)
    {
        cursorTwoDisplayed = true;
        presentationData->cursorTwoColor.setAlpha(255);


        tmpCursorTwoPosition = (double)(100.0 * (double)e->pos().x()) / (double)this->width();

        // If the second cursor is placed before the first (on its right), then
        // they will be switched, so that the first cursor is always the one on
        // the left.

        if (cursorOneDisplayed)
        {
            if (tmpCursorOnePosition > tmpCursorTwoPosition)
            {
                double tmpPosition = tmpCursorTwoPosition;
                tmpCursorTwoPosition = tmpCursorOnePosition;
                tmpCursorOnePosition = tmpPosition;
            }
        }
    }

    presentationData->setCursorRelativePosition(tmpCursorOnePosition,
                                                         tmpCursorTwoPosition);


    // If both cursor are displayed : SIGNAL send the interval
    //--------------------
    if (cursorOneDisplayed && cursorTwoDisplayed)
    {
        intervalHighlight(presentationData->cursorOneRelativePosition, presentationData->cursorTwoRelativePosition);
    }
}


/***************
 * Place the first cursor where the right-click is pressed and place the second
 * cursor where it is released
 * This function is for when the cursors are in "click-and-drag" mode.
 * cf Qt documentation.
 ***************/
void EventCatcherTopLayer::mouseReleaseEvent(QMouseEvent *e)
{
    if (presentationData->dragAndDrop)
    {
        dragRubberBand->hide();
    }


    double tmpCursorOnePosition = presentationData->cursorOneRelativePosition;
    double tmpCursorTwoPosition = presentationData->cursorTwoRelativePosition;


    if (presentationData->dragAndDrop && e->button() == Qt::LeftButton)
    {
        tmpCursorTwoPosition = (double)(100.0 * (double)e->pos().x()) / (double)this->width();

        cursorTwoDisplayed = true;
        presentationData->cursorTwoColor.setAlpha(255);

        repaint();

        intervalHighlight(tmpCursorOnePosition, tmpCursorTwoPosition);
    }

    presentationData->setCursorRelativePosition(tmpCursorOnePosition,
                                                         tmpCursorTwoPosition);
}


/***************
 * cf Qt documentation.
 ***************/
void EventCatcherTopLayer::paintEvent(QPaintEvent *e)
{
    QWidget::paintEvent(e);
    QPainter p1(this);
    QPainter p2(this);

    int cursorOnePositionX = (int)( (this->width() * presentationData->cursorOneRelativePosition) / 100 );
    int cursorTwoPositionX = (int)( (this->width() * presentationData->cursorTwoRelativePosition) / 100 );

    p1.fillRect(cursorOnePositionX, 0, 1, this->height(), QBrush(QColor(presentationData->cursorOneColor)) );
    p2.fillRect(cursorTwoPositionX, 0, 1, this->height(), QBrush(QColor(presentationData->cursorTwoColor)) );


    if (cursorOnePositionX > 0 && cursorOnePositionX < this->width()
            && cursorTwoPositionX > 0 && cursorTwoPositionX < this->width()
            && presentationData->isCursorIntervalShown)
    {
        QPainter intervalPainter(this);
        intervalPainter.setBrush(QBrush(presentationData->cursorIntervalColor));
        intervalPainter.setPen(QColor(0, 0, 0, 100));

        int interval = cursorTwoPositionX - cursorOnePositionX;

        QRectF intervalRectangle(cursorOnePositionX, 0,
                                 interval, this->height());

        intervalPainter.drawRect(intervalRectangle);
    }
}


/***************
 * Keep the cursor at the correct relative position when the window is resized.
 * cf Qt documentation.
 ***************/
void EventCatcherTopLayer::resizeEvent(QResizeEvent *e)
{
    repaint();
}


//      %%%%%%%%%%%%%%%%%%%%%%%%
//      %%%% METHODS public %%%%
//      %%%%%%%%%%%%%%%%%%%%%%%%


/***************
 * When in "drag-and-drop" mode for the cursors, this methods highlight the
 * space between the two cursors.
 *-------------------------
 * double cursorOne:
 * double cursorTwo:
 ***************/
void EventCatcherTopLayer::intervalHighlight(double cursorOne, double cursorTwo)
{
    double start, end;

    if (cursorOne < cursorTwo)
    {
        start = cursorOne;
        end = cursorTwo;
    }

    else
    {
        start = cursorTwo;
        end = cursorOne;
    }
}


/***************
 * When loading a project, set the new color for the cursor brush.
 ***************/
void EventCatcherTopLayer::setCursorPositon()
{
    cursorOneDisplayed = true;
    cursorTwoDisplayed = true;

    repaint();
}


//      %%%%%%%%%%%%%%%%%%%%%%
//      %%%% SLOTS public %%%%
//      %%%%%%%%%%%%%%%%%%%%%%


/***************
 * Place the second cursor after the first, depending of the indicated interval
 * from the user.
 *
 * NOTE:
 * Not working anymore. Just add a button in the ui an make a small window to
 * get the input for the interval. And use a pointer to the ProjectData instead
 * of the PresentationData in this class, so the timeScale can be taken from the
 * PageData. Also check if the first cursor + interval don't go over the time
 * scale.
 *-------------------------
 * int interval: the interval to which place the second cursor, in milliseconds.
 * int timeScale: the time scale in milliseconds.
 ***************/
void EventCatcherTopLayer::CursorIntervalInput(int interval, int timeScale)
{
    // if there's not the first cursor, we can't place the second
    if (cursorOneDisplayed)
    {
        // Time of the first cursor
        int timeFirstCursor = (presentationData->cursorOneRelativePosition * timeScale) / 100;

        // Time of the second cursor
        int timeSecondCursor = timeFirstCursor + interval;

        // Relative position of the second cursor
        presentationData->cursorTwoRelativePosition = (timeSecondCursor * 100) / timeScale;

        // Placement of the second cursor
        cursorTwoDisplayed = true;

        repaint();
    }
}


/***************
 * Switch between click and click-and-drag state to position the cursors
 ***************/
void EventCatcherTopLayer::CursorStateChange()
{
    if (!presentationData->dragAndDrop)
    {
        presentationData->dragAndDrop = true;
    }

    else
    {
        presentationData->dragAndDrop = false;
    }
}


/***************
 * Receive info from the data container and filter the hints to process only the
 * one of interest
 *-------------------------
 * size_t hint: flag that indicate specific changes in the data container. Allow
 * the method to know if it should take the signal into account or not.
 ***************/
void EventCatcherTopLayer::onPresentationDataChanged(size_t hint)
{
    if (hint & PresentationData::eHintCursorsChanged)
    {
        repaint();
    }

    if (hint & PresentationData::eHintIntervalChanged)
    {
        repaint();
    }
}
