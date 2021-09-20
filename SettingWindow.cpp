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
 * Create and manage the Setting Window, which is used to modify the themes and
 * colors of the backgrounds, the cursors, the highlighted cursors' interval,
 * and the plots.
 *
 * Author: Thibaut Bertrand
 * thibaut.bertrand@inserm.fr
 ***************/


#include <cmath>


#include "SettingWindow.h"
#include "ui_SettingWindow.h"

/***************
 * Constructor
 *-------------------------
 * QSize size: the size of the window.
 * PresentationData* presentation: the pointer to the data container of interest
 * for this class.
 ***************/
SettingWindow::SettingWindow(PresentationData* presentation,
                             QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingWindow)
{
    ui->setupUi(this);

    presentationData = presentation;


    // Background
    //--------------------
    squareColor = presentationData->backgroundColor;

    this->ui->editRedBackground->setText(QString::number(squareColor.red()));
    this->ui->editGreenBackground->setText(QString::number(squareColor.green()));
    this->ui->editBlueBackground->setText(QString::number(squareColor.blue()));


    int red = 0;
    int green = 0;
    int blue = 0;

    // Plots
    //--------------------
    this->ui->singleColorSquare->setAutoFillBackground(true);
    QPalette pal;
    pal.setColor(QPalette::Background, presentationData->singleFileColor);
    this->ui->singleColorSquare->setPalette(pal);


    for (int i_file = 0; i_file < presentationData->plotColors.length(); i_file++)
    {
        // Color Square
        QWidget* square = new QWidget(this);
        square->setMinimumSize(25, 25);
        square->setMaximumSize(25, 25);
        square->setAutoFillBackground(true);

        plotSquares.append(square);

        red = presentationData->plotColors[i_file].red();
        green = presentationData->plotColors[i_file].green();
        blue = presentationData->plotColors[i_file].blue();
        changeColorOfPlot(i_file, red, green, blue);


        // Button
        ButtonID* button = new ButtonID(i_file);
        button->setText("Change color");
        button->setMinimumSize(100, 25);
        button->setMaximumSize(100, 25);

        connect(button, SIGNAL(sendID(int, int)),
                this, SLOT(selectNewColor(int)));


        // Layout
        QHBoxLayout* layout = new QHBoxLayout;
        layout->addWidget(square);
        layout->addWidget(button);
        layout->addStretch();
        layout->setSpacing(15);

        this->ui->colorLayout->addLayout(layout);
    }


    // Cursors
    this->ui->cursorOneSquare->setAutoFillBackground(true);
    this->ui->cursorTwoSquare->setAutoFillBackground(true);
    this->ui->intervalSquare->setAutoFillBackground(true);
    changeColorOfCursor(0, presentationData->cursorOneColor);
    changeColorOfCursor(1, presentationData->cursorTwoColor);
    changeColorOfCursor(2, presentationData->cursorIntervalColor);


    this->ui->colorLayout->addStretch();
}


/***************
 * Destructor
 ***************/
SettingWindow::~SettingWindow()
{
    delete ui;
}


//      %%%%%%%%%%%%%%%%%%%%%%%%%
//      %%%% METHODS private %%%%
//      %%%%%%%%%%%%%%%%%%%%%%%%%


/***************
 * Change the color of the specified cursor square.
 *-------------------------
 * int fileID: the plot nmber in the list.
 * int red: the red component.
 * int green: the green component.
 * int blue: the blue component.
 ***************/
void SettingWindow::changeColorOfCursor(int target, int red, int green,
                                        int blue)
{
    QColor color;
    color.setRgb(red, green, blue);

    changeColorOfCursor(target, color);
}


/***************
 * Change the color of the specified cursor square.
 *-------------------------
 * int fileID: the plot nmber in the list.
 * QColor newColor: the new color to apply.
 ***************/
void SettingWindow::changeColorOfCursor(int target, QColor newColor)
{
    QPalette pal;
    pal.setColor(QPalette::Background, newColor);

    if (target == 0)
    {
        this->ui->cursorOneSquare->setPalette(pal);
    }

    else if (target == 1)
    {
        this->ui->cursorTwoSquare->setPalette(pal);
    }

    else if (target == 2)
    {
        this->ui->intervalSquare->setPalette(pal);
    }
}


/***************
 * Change the color of the specified file number square.
 *-------------------------
 * int fileID: the plot number in the list.
 * int red: the red component.
 * int green: the green component.
 * int blue: the blue component.
 ***************/
void SettingWindow::changeColorOfPlot(int fileID, int red, int green, int blue)
{
    QColor color;
    color.setRgb(red, green, blue);

    changeColorOfPlot(fileID, color);
}


/***************
 * Change the color of the specified file number square.
 *-------------------------
 * int fileID: the plot nmber in the list.
 * QColor newColor: the new color to apply.
 ***************/
void SettingWindow::changeColorOfPlot(int fileID, QColor newColor)
{
    QPalette pal;
    pal.setColor(QPalette::Background, newColor);
    plotSquares[fileID]->setPalette(pal);
}


/***************
 * Udpdate the exemple square in real time.
 ***************/
void SettingWindow::colorSquareUpdate()
{
    QString style = "background-color: rgb("
            + QString::number(squareColor.red()) + ", "
            + QString::number(squareColor.green()) + ", "
            + QString::number(squareColor.blue()) +
            ");\nborder: 1px solid black;";

    ui->colorSquare->setStyleSheet(style);
}


/***************
 * Udpdate the sliders and line edit.
 *-------------------------
 * int red: RGB format.
 * int green: RGB format.
 * int blue: RGB format.
 ***************/
void SettingWindow::colorSampleButtons(int red, int green, int blue)
{
    squareColor.setRgb(red, green, blue);

    this->ui->editRedBackground->setText(QString::number(red));
    this->ui->editGreenBackground->setText(QString::number(green));
    this->ui->editBlueBackground->setText(QString::number(blue));

    this->ui->sliderRedBackground->setValue(ceil(red / 2.57));
    this->ui->sliderGreenBackground->setValue(ceil(green / 2.57));
    this->ui->sliderBlueBackground->setValue(ceil(blue / 2.57));

    colorSquareUpdate();
}


//      %%%%%%%%%%%%%%%%%%%%%%%
//      %%%% SLOTS private %%%%
//      %%%%%%%%%%%%%%%%%%%%%%%


//      ====================
//      %%%% Background %%%%
//      ====================


//      %%%% Buttons %%%%


/***************
 * Change the main square color with one of the premade one. See the .ui for
 * more infos.
 ***************/
void SettingWindow::on_pushButtonBackground_clicked() {
    colorSampleButtons(240, 222, 173); }
void SettingWindow::on_pushButton_2Background_clicked() {
    colorSampleButtons(255, 255, 255); }
void SettingWindow::on_pushButton_3Background_clicked() {
    colorSampleButtons(224, 224, 224); }
void SettingWindow::on_pushButton_4Background_clicked() {
    colorSampleButtons(0, 0, 0); }
void SettingWindow::on_pushButton_5Background_clicked() {
    colorSampleButtons(255, 153, 153); }
void SettingWindow::on_pushButton_6Background_clicked() {
    colorSampleButtons(255, 204, 153); }
void SettingWindow::on_pushButton_7Background_clicked() {
    colorSampleButtons(255, 255, 153); }
void SettingWindow::on_pushButton_8Background_clicked() {
    colorSampleButtons(204, 255, 153); }
void SettingWindow::on_pushButton_9Background_clicked() {
    colorSampleButtons(153, 255, 153); }
void SettingWindow::on_pushButton_10Background_clicked() {
    colorSampleButtons(153, 255, 204); }
void SettingWindow::on_pushButton_11Background_clicked() {
    colorSampleButtons(153, 255, 255); }
void SettingWindow::on_pushButton_12Background_clicked() {
    colorSampleButtons(153, 204, 255); }
void SettingWindow::on_pushButton_13Background_clicked() {
    colorSampleButtons(153, 153, 255); }
void SettingWindow::on_pushButton_14Background_clicked() {
    colorSampleButtons(204, 153, 255); }
void SettingWindow::on_pushButton_15Background_clicked() {
    colorSampleButtons(255, 153, 255); }
void SettingWindow::on_pushButton_16Background_clicked() {
    colorSampleButtons(255, 153, 204); }


/***************
 * Apply the color presented in the main square to the central widget.
 ***************/
void SettingWindow::on_applyBackgroundButton_clicked()
{
    presentationData->setBackgroundColor(squareColor);
}


/***************
 * Reset the background to its default color.
 ***************/
void SettingWindow::on_defaultBackgroundButton_clicked()
{
    presentationData->setDefaultBackgroundColor();
}


/***************
 ***************/
void SettingWindow::on_customColorBackgroundButton_clicked()
{
    QColor actualColor = squareColor;
    QColor newColor = QColorDialog::getColor(actualColor, this);

    if (newColor.isValid())
    {
        squareColor = newColor;
        colorSquareUpdate();
    }
}


//      %%%% Edits %%%%


/***************
 * Called when the QLineEdit text is modified. Correspond to the red color of
 * the background.
 ***************/
void SettingWindow::on_editRedBackground_textEdited(const QString &arg1)
{
    ui->sliderRedBackground->setValue( (ceil(arg1.toInt())) / 2.57 );

    squareColor.setRed(ceil(arg1.toInt()));
    colorSquareUpdate();
}


/***************
 * Called when the QLineEdit text is modified. Correspond to the green color of
 * the background.
 ***************/
void SettingWindow::on_editGreenBackground_textEdited(const QString &arg1)
{
    ui->sliderGreenBackground->setValue( (ceil(arg1.toInt())) / 2.57 );

    squareColor.setGreen(ceil(arg1.toInt()));
    colorSquareUpdate();
}


/***************
 * Called when the QLineEdit text is modified. Correspond to the blue color of
 * the background.
 ***************/
void SettingWindow::on_editBlueBackground_textEdited(const QString &arg1)
{
    ui->sliderBlueBackground->setValue( (ceil(arg1.toInt())) / 2.57 );

    squareColor.setBlue(ceil(arg1.toInt()));
    colorSquareUpdate();
}


//      %%%% Sliders %%%%


/***************
 * Called when the QSlider is modified. Correspond to the red color of the
 * background.
 ***************/
void SettingWindow::on_sliderRedBackground_sliderMoved(int position)
{
    ui->editRedBackground->setText(QString::number(ceil(position * 2.57)));

    squareColor.setRed(ceil(position * 2.57));
    colorSquareUpdate();
}


/***************
 * Called when the QSlider is modified. Correspond to the green color of the
 * background.
 ***************/
void SettingWindow::on_sliderGreenBackground_sliderMoved(int position)
{
    ui->editGreenBackground->setText(QString::number(ceil(position * 2.57)));

    squareColor.setGreen(ceil(position * 2.57));
    colorSquareUpdate();
}


/***************
 * Called when the QSlider is modified. Correspond to the blue color of the
 * background.
 ***************/
void SettingWindow::on_sliderBlueBackground_sliderMoved(int position)
{
    ui->editBlueBackground->setText(QString::number(ceil(position * 2.57)));

    squareColor.setBlue(ceil(position * 2.57));
    colorSquareUpdate();
}


//      =================
//      %%%% Cursors %%%%
//      =================


//      %%%% Buttons %%%%


/***************
 * Set the cursor and interval colors to the one specified by the user int the
 * window.
 ***************/
void SettingWindow::on_applyCursorsButton_clicked()
{
    QColor cursorOne = this->ui->cursorOneSquare->palette().color(QPalette::Background);
    QColor cursorTwo = this->ui->cursorTwoSquare->palette().color(QPalette::Background);

    presentationData->setCursorColor(cursorOne, cursorTwo);


    presentationData->setCursorIntervalColor(this->ui->intervalSquare->palette().color(QPalette::Background));
}


/***************
 * Open a window to allow the user to select a color of its choice for the first
 * cursor.
 ***************/
void SettingWindow::on_cursorOneButton_clicked()
{
    QColor actualColor = this->ui->cursorOneSquare->palette().color(QPalette::Background);
    QColor newColor = QColorDialog::getColor(actualColor, this);

    if (newColor.isValid())
    {
        changeColorOfCursor(0, newColor);
    }
}


/***************
 * Open a window to allow the user to select a color of its choice for the
 * second cursor.
 ***************/
void SettingWindow::on_cursorTwoButton_clicked()
{
    QColor actualColor = this->ui->cursorOneSquare->palette().color(QPalette::Background);
    QColor newColor = QColorDialog::getColor(actualColor, this);

    if (newColor.isValid())
    {
        changeColorOfCursor(1, newColor);
    }
}


/***************
 * Set the cursor and interval colors to their default values.
 ***************/
void SettingWindow::on_defaultCursorsButton_clicked()
{
    presentationData->setCursorDefaultColor();
    presentationData->setCursorIntervalDefaultColor();

    changeColorOfCursor(0, presentationData->cursorOneColor);
    changeColorOfCursor(1, presentationData->cursorTwoColor);
    changeColorOfCursor(2, presentationData->cursorIntervalColor);
}


/***************
 * Open a window to allow the user to select a color of its choice for the
 * cursor's interval.
 ***************/
void SettingWindow::on_intervalButton_clicked()
{
    QColor actualColor = this->ui->cursorOneSquare->palette().color(QPalette::Background);
    QColor newColor = QColorDialog::getColor(actualColor, this);

    if (newColor.isValid())
    {
        changeColorOfCursor(2, newColor);
    }
}


//      ===============
//      %%%% Plots %%%%
//      ===============


/***************
 * Change the color of the single plot.
 ***************/
void SettingWindow::on_changeSinglePlotButton_clicked()
{
    QColor actualColor = this->ui->singleColorSquare->palette().color(QPalette::Background);
    QColor newColor = QColorDialog::getColor(actualColor, this);

    if (newColor.isValid())
    {
        QPalette pal;
        pal.setColor(QPalette::Background, newColor);
        this->ui->singleColorSquare->setPalette(pal);
    }
}


/***************
 * Ask the user what will be the new color.
 *-------------------------
 * int fileID: the plot nmber in the list.
 ***************/
void SettingWindow::selectNewColor(int fileID)
{
    QColor actualColor = plotSquares[fileID]->palette().color(QPalette::Background);
    QColor newColor = QColorDialog::getColor(actualColor, this);

    if (newColor.isValid())
    {
        changeColorOfPlot(fileID, newColor);
    }
}


/***************
 * Apply the selected colors to the plots.
 ***************/
void SettingWindow::on_applyPlotButton_clicked()
{
    QColor singleColor = this->ui->singleColorSquare->palette().color(QPalette::Background);

    QVector<QColor> multipleColors;

    for (int i_file = 0; i_file < plotSquares.length(); i_file++)
    {
        multipleColors.append(plotSquares[i_file]->palette().color(QPalette::Background));
    }

    presentationData->setPlotColors(singleColor, multipleColors);

    /*
     * Note: Plots will only update when closing the window due to how
     * QCustomPlot works. Don't know if there's a way to update them while the
     * window is still open.
     */
}


/***************
 * Set the default colors to all plot (single and multiple).
 ***************/
void SettingWindow::on_defaultPlotButton_clicked()
{
    presentationData->setDefaultPlotColors();

    /*
     * Note: Plots will only update when closing the window due to how
     * QCustomPlot works. Don't know if there's a way to update them while the
     * window is still open.
     */
}


//      ==================
//      %%%% Generals %%%%
//      ==================


/***************
 * Simply close the window by calling the destructor.
 ***************/
void SettingWindow::on_closeButton_clicked()
{
    this->~SettingWindow();
}


/***************
 * Ask the file that the user wants to load.
 ***************/
void SettingWindow::on_loadThemeButton_clicked()
{
    // Get the save file name
    //--------------------
    QFileDialog nameDialog;
    QString filename = QFileDialog::getOpenFileName(this,"Choose a file",
                                                    QDir::currentPath(),
                                                    "xml(*.xml)");

    xmlRead(filename);
}


/***************
 * Ask the user where to save the file, also the name of the save file.
 ***************/
void SettingWindow::on_saveThemeButton_clicked()
{
    // Get the save file name
    //--------------------
    QFileDialog nameDialog;
    QString filename = nameDialog.getSaveFileName(this,
                                                  tr("Choose save location"),
                                                  QDir::currentPath() + "/untitled.xml",
                                                  tr(".xml"));

    xmlWrite(filename);
}


/***************
 * Read the provided save file (.xml format) and apply the changes.
 *-------------------------
 * QString saveFilename: the path of the save file to load.
 ***************/
void SettingWindow::xmlRead(QString filename)
{
    // For info, here's the final structure of the save file in .xml format:

    /*
     *  <ROOT> main node
     *
     *      <Background Red = "int" Green = "int" Blue = "int"/> main node
     *
     *      <SinglePlot Red = "int" Green = "int" Blue = "int"/> main node
     *
     *      <MultiplePlots> main node
     *
     *          <Plot Red = "int" Green = "int" Blue = "int"/> first list node
     *          ...
     *          ...
     *          ...
     *
     *      </MultiplePlots>
     *
     *      <FirstCursor Red = "int" Green = "int" Blue = "int"/> main node
     *
     *      <SecondCursor Red = "int" Green = "int" Blue = "int"/> main node
     *
     *      <Interval Red = "int" Green = "int" Blue = "int"/> main node
     *
     *  </ROOT>
     */


    // Create a document to read XML
    //--------------------
    QDomDocument document;


    // Open a file for reading
    //--------------------
    QFile file(filename);
    document.setContent(&file);


    // Getting root element
    //--------------------
    QDomElement root = document.firstChildElement("ROOT");


    // Background
    //--------------------
    QDomNode mainNode = root.firstChild(); // -> Background

    int red = mainNode.attributes().namedItem("Red").nodeValue().toInt();
    int green = mainNode.attributes().namedItem("Green").nodeValue().toInt();
    int blue = mainNode.attributes().namedItem("Blue").nodeValue().toInt();

    squareColor = QColor(red, green, blue);
    colorSquareUpdate();

    ui->editRedBackground->setText(QString::number(red));
    ui->editGreenBackground->setText(QString::number(green));
    ui->editBlueBackground->setText(QString::number(blue));

    ui->sliderRedBackground->setValue( red / 2.57 );
    ui->sliderRedBackground->setValue( green / 2.57 );
    ui->sliderRedBackground->setValue( blue / 2.57 );


    // Single plot
    //--------------------
    mainNode = mainNode.nextSibling(); // -> SinglePlot

    red = mainNode.attributes().namedItem("Red").nodeValue().toInt();
    green = mainNode.attributes().namedItem("Green").nodeValue().toInt();
    blue = mainNode.attributes().namedItem("Blue").nodeValue().toInt();

    QPalette pal;
    pal.setColor(QPalette::Background, QColor(red, green, blue));
    this->ui->singleColorSquare->setPalette(pal);


    // Multiple plot
    //--------------------
    mainNode = mainNode.nextSibling(); // -> MultiplePlots

    QDomNodeList listNode = mainNode.childNodes(); // -> Plot

    // Just to be sure...
    if (listNode.count() == plotSquares.length())
    {
        for (int i_file = 0; i_file < listNode.count(); i_file++)
        {
            red = listNode.at(i_file).attributes().namedItem("Red").nodeValue().toInt();
            green = listNode.at(i_file).attributes().namedItem("Green").nodeValue().toInt();
            blue = listNode.at(i_file).attributes().namedItem("Blue").nodeValue().toInt();

            pal.setColor(QPalette::Background, QColor(red, green, blue));
            plotSquares[i_file]->setPalette(pal);
        }
    }


    // First cursor
    //--------------------
    mainNode = mainNode.nextSibling(); // -> FirstCursor

    red = mainNode.attributes().namedItem("Red").nodeValue().toInt();
    green = mainNode.attributes().namedItem("Green").nodeValue().toInt();
    blue = mainNode.attributes().namedItem("Blue").nodeValue().toInt();

    pal.setColor(QPalette::Background, QColor(red, green, blue));
    this->ui->cursorOneSquare->setPalette(pal);


    // Second cursor
    //--------------------
    mainNode = mainNode.nextSibling(); // -> SecondCursor

    red = mainNode.attributes().namedItem("Red").nodeValue().toInt();
    green = mainNode.attributes().namedItem("Green").nodeValue().toInt();
    blue = mainNode.attributes().namedItem("Blue").nodeValue().toInt();

    pal.setColor(QPalette::Background, QColor(red, green, blue));
    this->ui->cursorTwoSquare->setPalette(pal);


    // Interval
    //--------------------
    mainNode = mainNode.nextSibling(); // -> Interval

    red = mainNode.attributes().namedItem("Red").nodeValue().toInt();
    green = mainNode.attributes().namedItem("Green").nodeValue().toInt();
    blue = mainNode.attributes().namedItem("Blue").nodeValue().toInt();

    pal.setColor(QPalette::Background, QColor(red, green, blue));
    this->ui->intervalSquare->setPalette(pal);
}


/***************
 * Save the various elements of the themes and colors configured by the user.
 * The save file is in .xml format.
 *-------------------------
 * QString saveFilename: the path of the future save file.
 ***************/
void SettingWindow::xmlWrite(QString filename)
{
    // For info, here's the final structure of the save file in .xml format:

    /*
     *  <ROOT> main node
     *
     *      <Background Red = "int" Green = "int" Blue = "int"/> main node
     *
     *      <SinglePlot Red = "int" Green = "int" Blue = "int"/> main node
     *
     *      <MultiplePlots> main node
     *
     *          <Plot Red = "int" Green = "int" Blue = "int"/> first list node
     *          ...
     *          ...
     *          ...
     *
     *      </MultiplePlots>
     *
     *      <FirstCursor Red = "int" Green = "int" Blue = "int"/> main node
     *
     *      <SecondCursor Red = "int" Green = "int" Blue = "int"/> main node
     *
     *      <Interval Red = "int" Green = "int" Blue = "int"/> main node
     *
     *  </ROOT>
     */


    // Save data
    //--------------------

    // Create a document to write XML
    QDomDocument saveDocument;


    // Root element (must be unique! All other nodes are its children!)
    //--------------------
    QDomElement saveFile = saveDocument.createElement("ROOT");
    saveDocument.appendChild(saveFile);


    // Background
    //--------------------
    QDomElement background = saveDocument.createElement("Background");
    saveFile.appendChild(background);

    background.setAttribute("Red", presentationData->backgroundColor.red());
    background.setAttribute("Green", presentationData->backgroundColor.green());
    background.setAttribute("Blue", presentationData->backgroundColor.blue());


    // Single plot
    //--------------------
    QDomElement singlePlot = saveDocument.createElement("SinglePlot");
    saveFile.appendChild(singlePlot);

    singlePlot.setAttribute("Red", presentationData->singleFileColor.red());
    singlePlot.setAttribute("Green", presentationData->singleFileColor.green());
    singlePlot.setAttribute("Blue", presentationData->singleFileColor.blue());


    // Multiple plot
    //--------------------
    QDomElement multiplePlots = saveDocument.createElement("MultiplePlots");
    saveFile.appendChild(multiplePlots);

    for (int i_file = 0; i_file < plotSquares.length(); i_file++)
    {
        QDomElement plot = saveDocument.createElement("Plot");
        plot.setAttribute("Red", plotSquares[i_file]->palette().color(QPalette::Background).red());
        plot.setAttribute("Green", plotSquares[i_file]->palette().color(QPalette::Background).green());
        plot.setAttribute("Blue", plotSquares[i_file]->palette().color(QPalette::Background).blue());
        multiplePlots.appendChild(plot);
    }


    // First cursor
    //--------------------
    QDomElement firstCursor = saveDocument.createElement("FirstCursor");
    saveFile.appendChild(firstCursor);

    firstCursor.setAttribute("Red", presentationData->cursorOneColor.red());
    firstCursor.setAttribute("Green", presentationData->cursorOneColor.green());
    firstCursor.setAttribute("Blue", presentationData->cursorOneColor.blue());


    // Second cursor
    //--------------------
    QDomElement secondCursor = saveDocument.createElement("SecondCursor");
    saveFile.appendChild(secondCursor);

    secondCursor.setAttribute("Red", presentationData->cursorTwoColor.red());
    secondCursor.setAttribute("Green", presentationData->cursorTwoColor.green());
    secondCursor.setAttribute("Blue", presentationData->cursorTwoColor.blue());


    // Interval
    //--------------------
    QDomElement interval = saveDocument.createElement("Interval");
    saveFile.appendChild(interval);

    interval.setAttribute("Red", presentationData->cursorIntervalColor.red());
    interval.setAttribute("Green", presentationData->cursorIntervalColor.green());
    interval.setAttribute("Blue", presentationData->cursorIntervalColor.blue());


    // Saving
    //====================
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
    }
}
