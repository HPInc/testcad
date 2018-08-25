/*********************************************************************************

  MIT License

  Copyright © 2018 HP Development Company, L.P.

  Permission  is  hereby granted,  free  of  charge, to any person obtaining  a
  copy of this software  and  associated  documentation files (the "Software"),
  to deal in the Software without restriction, including without limitation the
  rights to use, copy, modify,  merge,  publish, distribute, sublicense, and/or
  sell copies  of the Software,  and to permit persons  to whom the Software is
  furnished to do so,subject to the following conditions:

  The above copyright  notice and this  permission notice shall  be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE  IS PROVIDED  "AS IS", WITHOUT WARRANTY OF  ANY KIND, EXPRESS OR 
  IMPLIED, INCLUDING  BUT NOT  LIMITED  TO THE  WARRANTIES OF  MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.

  IN  NO EVENT  SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
  DAMAGES  OR  OTHER  LIABILITY,  WHETHER IN  AN ACTION  OF  CONTRACT, TORT  OR 
  OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE
  OR OTHER DEALINGS IN THE SOFTWARE.    

*********************************************************************************/

#include "sequencerWindow.h"

sequencerWindow::sequencerWindow(QWidget *parent, QTreeWidgetItem *testItem) : QMainWindow(parent)
{
    diagramEditor = new diagramView(0);
    this->setCentralWidget(diagramEditor);
    this->resize(800,800);
    this->statusBar()->setVisible(true);
    this->setWindowIcon(QIcon(":/icons/sequenceIcon.png"));
    treeItem = testItem;
    createActions();
    createMenu();
    createToolBar();
    loadDiagram();
}
//--------------------------------------------------------------------------------------------------------------------------------

void sequencerWindow::writeFormattedLines(QString textToWrite)
{
    for(int i=0; i<textToWrite.size();i++){

        if (i>0){

            if ((textToWrite.at(i)=='\n')&&
                    ((textToWrite.at(i-1)=='.')||(textToWrite.at(i-1)==':')||(textToWrite.at(i-1)=='\n'))){
                xmlWriter->writeEmptyElement("br");

            }else{
                xmlWriter->writeCharacters(textToWrite.at(i));

            }
        }else{
            xmlWriter->writeCharacters(textToWrite.at(i));

        }
    }
}
//--------------------------------------------------------------------------------------------------------------------------------

void sequencerWindow::writeDataRows(switchSequence testSequence)
{
    xmlWriter->writeStartElement("tr");
        xmlWriter->writeStartElement("td");
            xmlWriter->writeAttribute("class","index");
            xmlWriter->writeCharacters("1");
        xmlWriter->writeEndElement();
        xmlWriter->writeStartElement("td");
            xmlWriter->writeAttribute("class","data");
            writeFormattedLines(testSequence.switches.first().stateText + " " + testSequence.switches.first().transitionText);
        xmlWriter->writeEndElement();
        xmlWriter->writeStartElement("td");
            xmlWriter->writeAttribute("class","data");
            xmlWriter->writeCharacters(testSequence.switches.at(1).stateText);
        xmlWriter->writeEndElement();
    xmlWriter->writeEndElement();

    for (int j = 1; j < testSequence.switches.size() - 1; j++){
        xmlWriter->writeStartElement("tr");
            xmlWriter->writeStartElement("td");
                xmlWriter->writeAttribute("class","index");
                xmlWriter->writeCharacters(QString::number(j + 1));
            xmlWriter->writeEndElement();
            xmlWriter->writeStartElement("td");
                xmlWriter->writeAttribute("class","data");
                writeFormattedLines(testSequence.switches.at(j).transitionText);
            xmlWriter->writeEndElement();
            xmlWriter->writeStartElement("td");
                xmlWriter->writeAttribute("class","data");
                writeFormattedLines(testSequence.switches.at(j + 1).stateText);
            xmlWriter->writeEndElement();
        xmlWriter->writeEndElement();

    }
}
//--------------------------------------------------------------------------------------------------------------------------------

void sequencerWindow::centerOnDiagram()
{
    diagramEditor->Diagram->fitViewerTo(diagramEditor->Diagram->itemsBoundingRect());

}
//--------------------------------------------------------------------------------------------------------------------------------

void sequencerWindow::writeHtmlStyle()
{
    xmlWriter->writeTextElement("style","h1 { font-family: 'Arial';\n"
                                                "\tfont-size: 14px;}\n"
                                        ".coverageTable { border-collapse: separate;\n"
                                                "\tborder-spacing: 5px;\n"
                                                "\tborder: none;\n}\n"
                                        ".coverageValue { font-family: 'Arial';\n"
                                                "\tfont-size: 11px;\n"
                                                "\tcolor: #555555;\n"
                                                "\tborder: none;\n"
                                                "\tfont-weight: bold;}\n"
                                        ".missedSwTable {border-collapse: separate;\n"
                                                "\tborder-spacing: 5px;\n"
                                                "\tborder: none;}\n"
                                        ".missed { border: none;\n"
                                                    "\tcolor: red;\n"
                                                    "\tfont-family: 'Arial';\n"
                                                    "\tfont-size: 11px;}\n"
                                        ".missedShaded { border: none;\n"
                                                    "\tcolor: red;\n"
                                                    "\tbackground-color: #FFFFCC;\n"
                                                    "\tfont-family: 'Arial';\n"
                                                    "\tfont-size: 11px;}\n"
                                        ".covered { color: green;\n"
                                                    "\tfont-family: 'Arial';\n"
                                                    "\tfont-size: 11px;\n"
                                                    "\tborder: none;}\n"
                                        ".missedLabel { border: none;\n"
                                                    "\tcolor: black;\n"
                                                    "\tfont-family: 'Arial';\n"
                                                    "\tfont-size: 11px;}\n"
                                        ".missedHeader { border: none;\n"
                                                    "\tcolor: #555555;\n"
                                                    "\tfont-family: 'Arial';\n"
                                                    "\tfont-size: 11px;}\n"
                                        ".missedRibbonHeader { border: none;\n"
                                                    "\tcolor: #FFFFFF;}\n"
                                        ".missedRibbon { border: none;\n"
                                                    "\twidth:5px;"
                                                    "\tbackground-color: #555555;}\n"
                                        ".stepsTable{margin: 30px;}\n"
                                        "th { font-family: 'Arial';\n"
                                                "\tfont-size: 12px;}\n"
                                        "td { font-family: 'Arial';\n"
                                                "\tfont-size: 12px;}\n"
                                        "table, th, td { border: 1px solid black;\n"
                                                    "\tborder-collapse: collapse;\n"
                                                    "\ttext-align: left;\n"
                                                    "\tborder-color: #DDDDDD;\n"
                                                    "\tmargin: 10px;}\n"
                                        ".index {width: 4%;\n"
                                                "\ttext-align: center;}\n"
                                        ".data {width: 48%;}\n");
}
//--------------------------------------------------------------------------------------------------------------------------------

void sequencerWindow::writeMissingNSwitches(int testIndex)
{
    xmlWriter->writeStartElement("table");
        xmlWriter->writeAttribute("class","missedSwTable");

    if (testCasesStatistics.missingNSwitches.at(testIndex).sequences.size() > 0){
        xmlWriter->writeStartElement("tr");
            xmlWriter->writeStartElement("th");
                xmlWriter->writeAttribute("class","missedLabel");
                xmlWriter->writeAttribute("colspan","3");
                xmlWriter->writeCharacters(QString::number(testCasesStatistics.missingNSwitches.at(testIndex).sequences.size()) +
                                           " (" +  QString(STRING_N) +
                                           QString::number(switchLevel) + ") " + QString(STRING_MISSED_N_SWITCHES) + ":");
            xmlWriter->writeEndElement();
        xmlWriter->writeEndElement();

        xmlWriter->writeStartElement("tr");
            xmlWriter->writeStartElement("td");
                xmlWriter->writeAttribute("class","missedRibbonHeader");
            xmlWriter->writeEndElement();
            xmlWriter->writeStartElement("th");
                xmlWriter->writeAttribute("class","missedHeader");
                xmlWriter->writeCharacters(QString(STRING_STATE));
            xmlWriter->writeEndElement();
            xmlWriter->writeStartElement("th");
                xmlWriter->writeAttribute("class","missedHeader");
                xmlWriter->writeCharacters(QString(STRING_TRANSITION));
            xmlWriter->writeEndElement();
        xmlWriter->writeEndElement();

        switchSequence seqBuff;

        for (int i = 0; i < testCasesStatistics.missingNSwitches.at(testIndex).sequences.size(); i++){
            seqBuff = testCasesStatistics.missingNSwitches.at(testIndex).sequences.at(i);

            for (int j = 0; j < seqBuff.switches.size(); j++){
                xmlWriter->writeStartElement("tr");

                if(j == 0){
                    xmlWriter->writeStartElement("td");
                        xmlWriter->writeAttribute("class","missedRibbon");
                        xmlWriter->writeAttribute("rowspan",QString::number(seqBuff.switches.size()));
                    xmlWriter->writeEndElement();
                }

                    xmlWriter->writeStartElement("td");

                    if (j % 2 == 0){
                        xmlWriter->writeAttribute("class","missedShaded");

                    }else{
                        xmlWriter->writeAttribute("class","missed");

                    }
                        xmlWriter->writeCharacters(seqBuff.switches.at(j).stateText);
                    xmlWriter->writeEndElement();
                    xmlWriter->writeStartElement("td");

                    if (j % 2 == 0){
                        xmlWriter->writeAttribute("class","missedShaded");

                    }else{
                        xmlWriter->writeAttribute("class","missed");

                    }
                        xmlWriter->writeCharacters(seqBuff.switches.at(j).transitionText);
                    xmlWriter->writeEndElement();
                xmlWriter->writeEndElement();

            }

        }
    }else{
        xmlWriter->writeStartElement("tr");
            xmlWriter->writeStartElement("th");
                xmlWriter->writeAttribute("class","covered");
                xmlWriter->writeCharacters(QString::number(switchLevel) + " " + QString(STRING_ALL_N_SWITCHES_COVERED));
            xmlWriter->writeEndElement();
        xmlWriter->writeEndElement();

    }

     xmlWriter->writeEndElement();
}
//--------------------------------------------------------------------------------------------------------------------------------

void sequencerWindow::writeMissingZeroSwitches(int testIndex)
{
    xmlWriter->writeStartElement("table");
        xmlWriter->writeAttribute("class","missedSwTable");

    if (testCasesStatistics.missingZeroSwitches.at(testIndex).sequences.size() > 0){
        xmlWriter->writeStartElement("tr");
            xmlWriter->writeStartElement("th");
                xmlWriter->writeAttribute("class","missedLabel");
                xmlWriter->writeAttribute("colspan","3");
                xmlWriter->writeCharacters(QString::number(testCasesStatistics.missingZeroSwitches.at(testIndex).sequences.size()) +
                                           " " + QString(STRING_MISSED_STATE_TRANSITIONS) + ":");
            xmlWriter->writeEndElement();
        xmlWriter->writeEndElement();

        xmlWriter->writeStartElement("tr");
            xmlWriter->writeStartElement("td");
                xmlWriter->writeAttribute("class","missedRibbonHeader");
            xmlWriter->writeEndElement();
            xmlWriter->writeStartElement("th");
                xmlWriter->writeAttribute("class","missedHeader");
                xmlWriter->writeCharacters(QString(STRING_STATE));
            xmlWriter->writeEndElement();
            xmlWriter->writeStartElement("th");
                xmlWriter->writeAttribute("class","missedHeader");
                xmlWriter->writeCharacters(QString(STRING_TRANSITION));
            xmlWriter->writeEndElement();
        xmlWriter->writeEndElement();

        switchSequence seqBuff;

        for (int i = 0; i < testCasesStatistics.missingZeroSwitches.at(testIndex).sequences.size(); i++){
            seqBuff = testCasesStatistics.missingZeroSwitches.at(testIndex).sequences.at(i);

            for (int j = 0; j < seqBuff.switches.size(); j++){
                xmlWriter->writeStartElement("tr");
                    xmlWriter->writeStartElement("td");
                        xmlWriter->writeAttribute("class","missedRibbon");
                        xmlWriter->writeAttribute("rowspan",QString::number(seqBuff.switches.size()));
                    xmlWriter->writeEndElement();
                    xmlWriter->writeStartElement("td");
                    if (i % 2 == 0){
                        xmlWriter->writeAttribute("class","missedShaded");

                    }else{
                        xmlWriter->writeAttribute("class","missed");

                    }
                        xmlWriter->writeCharacters(seqBuff.switches.at(j).stateText);
                    xmlWriter->writeEndElement();
                    xmlWriter->writeStartElement("td");

                    if (i % 2 == 0){
                        xmlWriter->writeAttribute("class","missedShaded");

                    }else{
                        xmlWriter->writeAttribute("class","missed");

                    }
                        xmlWriter->writeCharacters(seqBuff.switches.at(j).transitionText);
                    xmlWriter->writeEndElement();
                xmlWriter->writeEndElement();

            }
        }
    }else{
        xmlWriter->writeStartElement("tr");
            xmlWriter->writeStartElement("th");
                xmlWriter->writeAttribute("class","covered");
                xmlWriter->writeCharacters(QString(STRING_ALL_ZERO_SWITCHES_COVERED));
            xmlWriter->writeEndElement();
        xmlWriter->writeEndElement();

    }

    xmlWriter->writeEndElement();
}
//--------------------------------------------------------------------------------------------------------------------------------

void sequencerWindow::writeCoverageStatistics(int testIndex)
{
    xmlWriter->writeStartElement("table");
        xmlWriter->writeAttribute("class","coverageTable");
        xmlWriter->writeStartElement("tr");
            xmlWriter->writeStartElement("td");
                xmlWriter->writeAttribute("class","coverageValue");
                xmlWriter->writeCharacters(QString(STRING_SWITCHES_COVERAGE) + " " + testCasesStatistics.switchesCoverage.at(testIndex));
            xmlWriter->writeEndElement();
            xmlWriter->writeStartElement("td");
                xmlWriter->writeAttribute("class","coverageValue");
                xmlWriter->writeCharacters(QString(STRING_STATES_COVERAGE) + " " + testCasesStatistics.statesCoverage.at(testIndex));
            xmlWriter->writeEndElement();
            xmlWriter->writeStartElement("td");
                xmlWriter->writeAttribute("class","coverageValue");
                xmlWriter->writeCharacters(QString(STRING_TRANSITIONS_COVERAGE) + " " + testCasesStatistics.transitionsCoverage.at(testIndex));
            xmlWriter->writeEndElement();
        xmlWriter->writeEndElement();
    xmlWriter->writeEndElement();
}
//--------------------------------------------------------------------------------------------------------------------------------

void sequencerWindow::writeTestStatistics(int testIndex)
{
        writeCoverageStatistics(testIndex);
        writeMissingZeroSwitches(testIndex);

        if(switchLevel > 0){
            writeMissingNSwitches(testIndex);

        }

}
//--------------------------------------------------------------------------------------------------------------------------------

void sequencerWindow::writeColumnHeaders()
{
    xmlWriter->writeStartElement("tr");
    xmlWriter->writeTextElement("th",STRING_STEP);
    xmlWriter->writeTextElement("th",STRING_ACTION);
    xmlWriter->writeTextElement("th",STRING_EXPECTED_RESULT);
    xmlWriter->writeEndElement();
}
//--------------------------------------------------------------------------------------------------------------------------------

void sequencerWindow::exportHtml()
{
    exportTestsAs(html);
}
//--------------------------------------------------------------------------------------------------------------------------------

void sequencerWindow::exportTxt()
{
    exportTestsAs(txt);
}
//--------------------------------------------------------------------------------------------------------------------------------

void sequencerWindow::exportTestsAs(int fileFormat)
{
    if(diagramEditor->Diagram->isValid()){
        bool ok;
        switchLevel = QInputDialog::getInt(this,STRING_DIALOG_TEST_GENERATION,STRING_DIALOG_SWITCH_LENGTH,0,0,3,1,&ok);

        if(ok){
            fsmRouter router(diagramEditor->Diagram, switchLevel);
            testCases = router.tests;

            if (testCases.sequences.size() == 0){
                QMessageBox msg(QMessageBox::Critical,STRING_SEQUENCE_DESIGNER,
                                tr("Your Design is not a Finite State Machine, modify it to avoid trap paths."),QMessageBox::Ok);
                msg.exec();
                statusBar()->showMessage(tr(STRING_FILE_NOT_SAVED),3000);

            }else{

                testCasesStatistics = router.testsStats;
                QString formatDescription = STRING_FILETYPE_HTML;

                if(fileFormat == txt)
                    formatDescription = STRING_FILETYPE_TXT;

                QString filePath = QFileDialog::getSaveFileName(this, QObject::tr(STRING_SAVE_TESTSTEPS_FILE),
                                                     QDir::currentPath(),
                                                     formatDescription);

                bool saved = false;

                if (fileFormat == html){

                    if (!filePath.endsWith(".html"))
                        filePath = filePath + ".html";

                    saved = writeHtmlTo(filePath);

                }else if(fileFormat == txt){

                    if (!filePath.endsWith(".txt"))
                        filePath = filePath + ".txt";

                    saved = writeTxtTo(filePath);
                }

                if(saved){
                    statusBar()->showMessage(tr(STRING_FILE_SAVED),3000);

                }else{
                    QMessageBox msg(QMessageBox::Warning,STRING_SEQUENCE_DESIGNER,STRING_FILE_NOT_SAVED,QMessageBox::Ok);
                    msg.exec();
                    statusBar()->showMessage(tr(STRING_FILE_NOT_SAVED),3000);

                }
            }

        }else{
            statusBar()->showMessage(STRING_OPERATION_CANCELLED, 3000);

        }

    }else{
        statusBar()->showMessage(STRING_DIAGRAM_PLEASE_CORRECT_ERRORS, 3000);

    }

}
//--------------------------------------------------------------------------------------------------------------------------------

bool sequencerWindow::writeTxtTo(QString filePath)
{
    if (filePath.isEmpty())
        return false;

    QFile file(filePath);

    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this,STRING_TESTCAD,
                             QObject::tr(STRING_CANNOT_WRITE_FILE)
                             .arg(filePath)
                             .arg(file.errorString()));

        return false;

    }else{
        QString output="";

        for (int i = 0; i < testCases.sequences.size(); i++){
            switchSequence seq = testCases.sequences.at(i);

            if(seq.switches.size() > 1){

                for (int j = 0; j < seq.switches.size(); j++){
                    output = output + "\n" + seq.switches.at(j).stateText + "\n" + seq.switches.at(j).transitionText;

                }
            }
        }

        file.write(output.toLocal8Bit());
        return true;
    }
}
//--------------------------------------------------------------------------------------------------------------------------------

void sequencerWindow::loadDiagram(QTreeWidgetItem *fromTreeItem)
{
    treeItem = fromTreeItem;
    loadDiagram();

}
//--------------------------------------------------------------------------------------------------------------------------------

void sequencerWindow::loadDiagram()
{
    clearDiagram();

    if (treeItem != 0 ){
        diagramStructure structure = structureFromItem(treeItem);
        this->setWindowTitle(treeItem->text(0));

        if (structure.shapesInfo.size() > 0){
            diagramEditor->Diagram->loadFromStructure(structure);
            diagramEditor->Diagram->hasChanged = false;

        }
    }
}
//--------------------------------------------------------------------------------------------------------------------------------

void sequencerWindow::showHelp()
{
    QMessageBox::about(this,tr(STRING_SEQUENCE_DESIGNER), tr(STRING_SEQUENCER_HELP));
}
//--------------------------------------------------------------------------------------------------------------------------------

void sequencerWindow::writeHtmlH1(int testsCount)
{
    xmlWriter->writeStartElement("h1");
    xmlWriter->writeCharacters(QString::number(testsCount) + " " +
                               QString(STRING_TEST_CASES) +
                               " (" +  QString::number(switchLevel) + " " + QString(STRING_SWITCH_LEVEL) + ") " +
                               treeItem->text(0));
    xmlWriter->writeEndElement();
}
//--------------------------------------------------------------------------------------------------------------------------------

bool sequencerWindow::writeHtmlTo(QString filePath)
{
    if (filePath.isEmpty())
        return false;

    QFile file(filePath);

    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this,STRING_TESTCAD,
                             QObject::tr(STRING_CANNOT_WRITE_FILE)
                             .arg(filePath)
                             .arg(file.errorString()));

        return false;

    }else{
        xmlWriter = new QXmlStreamWriter(&file);
        xmlWriter->setAutoFormatting(true);
        xmlWriter->writeStartElement("html");
        xmlWriter->writeStartElement("body");
        writeHtmlStyle();
        writeHtmlH1(testCases.sequences.size());

        for (int i = 0; i < testCases.sequences.size(); i++){
            switchSequence seq = testCases.sequences.at(i);

            if(seq.switches.size() > 1){
                xmlWriter->writeTextElement("h1", QString(STRING_TEST_CASE) + " " + QString::number(i + 1));
                writeTestStatistics(i);
                xmlWriter->writeStartElement("table");
                xmlWriter->writeAttribute("class","stepsTable");
                    writeColumnHeaders();
                    writeDataRows(seq);
                xmlWriter->writeEndElement();
            }
        }

        xmlWriter->writeEndElement();
        xmlWriter->writeEndElement();
        bool hasError = xmlWriter->hasError();
        delete xmlWriter;

        return !hasError;
    }
}
//--------------------------------------------------------------------------------------------------------------------------------

diagramStructure sequencerWindow::structureFromItem(QTreeWidgetItem *treeItem)
{
    diagramStructure structure;

    if (treeItem != 0 ){
        QList<shapeProperties>shapesProps;
        QList<connectorProperties>connectorsProps;
        QVariant storedVariant;
        QList<QVariant> storedStructure;
        storedVariant = treeItem->data(2, Qt::UserRole);

        if (storedVariant.isValid()){
            storedStructure = storedVariant.toList();
            QList <QVariant> shPropsList = storedStructure.at(0).toList();
            shapeProperties shProps;
            QList <QVariant> cnnPropsList = storedStructure.at(1).toList();
            connectorProperties cnnProps;

            for(int i = 0; i < shPropsList.size(); i++){
                shProps = sequencerWindow::variantToShapeProperties(shPropsList.at(i));
                shapesProps.append(shProps);

            }

            for(int i = 0; i < cnnPropsList.size(); i++){
                cnnProps = sequencerWindow::variantToConnectorProperties(cnnPropsList.at(i));
                connectorsProps.append(cnnProps);
            }

            structure.shapesInfo = shapesProps;
            structure.connectorsInfo = connectorsProps;
       }
    }

    return structure;
}
//--------------------------------------------------------------------------------------------------------------------------------

shapeProperties sequencerWindow::variantToShapeProperties(QVariant shapePropertiesVariant)
{
    shapeProperties output;
    QList<QVariant> variantList = shapePropertiesVariant.toList();
    bool success;
    Q_UNUSED(success);

    output.ID = variantList.at(0).toInt(&success);
    output.position = variantList.at(1).toPointF();
    output.shapeType = variantList.at(2).toInt(&success);
    output.text = variantList.at(3).toString();
    output.textBoxPosition = variantList.at(4).toPointF();

    return output;
}
//--------------------------------------------------------------------------------------------------------------------------------

connectorProperties sequencerWindow::variantToConnectorProperties(QVariant connectorPropertiesVariant)
{
    connectorProperties output;
    QList<QVariant> variantList = connectorPropertiesVariant.toList();
    bool success;
    Q_UNUSED(success);

    output.ID = variantList.at(0).toInt(&success);
    output.startPoint = variantList.at(1).toPointF();
    output.endPoint = variantList.at(2).toPointF();
    output.bezierFromPosition = variantList.at(3).toPointF();
    output.bezierToPosition = variantList.at(4).toPointF();
    output.fromHookPointIndex = variantList.at(5).toInt(&success);
    output.toHookPointIndex = variantList.at(6).toInt(&success);
    output.fromID = variantList.at(7).toInt(&success);
    output.toID = variantList.at(8).toInt(&success);
    output.text = variantList.at(9).toString();
    output.textBoxPosition = variantList.at(10).toPointF();

    return output;
}
//--------------------------------------------------------------------------------------------------------------------------------

QVariant sequencerWindow::propertiesToVariant(shapeProperties properties)
{
    QList<QVariant> output;
    output.append(QVariant::fromValue(properties.ID));
    output.append(QVariant::fromValue(properties.position));
    output.append(QVariant::fromValue(properties.shapeType));
    output.append(QVariant::fromValue(properties.text));
    output.append(QVariant::fromValue(properties.textBoxPosition));

    return QVariant::fromValue(output);
}

//--------------------------------------------------------------------------------------------------------------------------------

QVariant sequencerWindow::propertiesToVariant(connectorProperties properties)
{
    QList<QVariant> output;
    output.append(QVariant::fromValue(properties.ID));
    output.append(QVariant::fromValue(properties.startPoint));
    output.append(QVariant::fromValue(properties.endPoint));
    output.append(QVariant::fromValue(properties.bezierFromPosition));
    output.append(QVariant::fromValue(properties.bezierToPosition));
    output.append(QVariant::fromValue(properties.fromHookPointIndex));
    output.append(QVariant::fromValue(properties.toHookPointIndex));
    output.append(QVariant::fromValue(properties.fromID));
    output.append(QVariant::fromValue(properties.toID));
    output.append(QVariant::fromValue(properties.text));
    output.append(QVariant::fromValue(properties.textBoxPosition));

    return QVariant::fromValue(output);
}
//--------------------------------------------------------------------------------------------------------------------------------

void sequencerWindow::storeDiagram()
{
    if (treeItem!=0){
        diagramStructure structure;
        structure = diagramEditor->Diagram->getStructure();

        QList <QVariant> statesInfo;
        for(int i = 0; i < structure.shapesInfo.size(); i++){
            statesInfo.append(propertiesToVariant(structure.shapesInfo.at(i)));

        }

        QList <QVariant> connectionsInfo;
        connectorProperties connBuff;

        for(int i = 0; i < structure.connectorsInfo.size(); i++){
            connBuff = structure.connectorsInfo.at(i);

            if (connBuff.toID != -1){
                connectionsInfo.append(propertiesToVariant(structure.connectorsInfo.at(i)));
            }
        }

        QList <QVariant> structureVariant;
        structureVariant.append(QVariant::fromValue(statesInfo));
        structureVariant.append(QVariant::fromValue(connectionsInfo));
        treeItem->setData(2,Qt::UserRole, structureVariant);
        statusBar()->showMessage(STRING_DESIGN_STORED, 3000);
        diagramEditor->Diagram->hasChanged = false;
        emit saved();
    }

}
//--------------------------------------------------------------------------------------------------------------------------------

void sequencerWindow::saveAsPicture()
{
    QString result = STRING_DESIGN_EMPTY;
    QMessageBox::Icon icon = QMessageBox::Warning;

    if (diagramEditor->Diagram->items().size() > 0){
          QString imageFileName = QFileDialog::getSaveFileName(0, QObject::tr(STRING_ACTION_EXPORT_IMAGE),
                                                                 QDir::currentPath(),
                                                                 QObject::tr(STRING_PNG_IMAGE_FILE));

        if (!imageFileName.isEmpty()){

            if (!imageFileName.contains("png",Qt::CaseInsensitive)){
                imageFileName = imageFileName + ".png";
            }

            if (diagramEditor->Diagram->saveImage(imageFileName)){
                result = STRING_FILE_SAVED;
                icon = QMessageBox::Information;

            }

        }else{
            result = STRING_FILE_NOT_SAVED;
            icon = QMessageBox::Critical;

        }
    }

    QMessageBox msg(icon, STRING_ACTION_EXPORT_IMAGE, result, QMessageBox::Ok);
    msg.exec();

    statusBar()->showMessage(result, 2000);
}
//--------------------------------------------------------------------------------------------------------------------------------

void sequencerWindow::createActions()
{
    addStateAction = new QAction(QIcon(":/icons/addStateIcon.png"),STRING_ADD_STATE, this);
    connect(addStateAction,SIGNAL(triggered()), diagramEditor->Diagram, SLOT(addState()));

    addInitialStateAction = new QAction(QIcon(":/icons/addInitialStateIcon.png"),STRING_ADD_INITIAL_STATE, this);
    connect(addInitialStateAction,SIGNAL(triggered()), diagramEditor->Diagram, SLOT(addInitialState()));

    addFinalStateAction = new QAction(QIcon(":/icons/addFinalStateIcon.png"),STRING_ADD_FINAL_STATE, this);
    connect(addFinalStateAction,SIGNAL(triggered()), diagramEditor->Diagram, SLOT(addFinalState()));

    connectAction = new QAction(QIcon(":/icons/connectIcon.png"),STRING_CONNECT_STATES, this);
    connect(connectAction,SIGNAL(triggered()), diagramEditor->Diagram, SLOT(connectStates()));

    deleteSelectedAction = new QAction(QIcon(":/icons/deleteIcon.png"),STRING_ACTION_DELETE, this);
    deleteSelectedAction->setShortcut(Qt::Key_Delete);
    connect(deleteSelectedAction,SIGNAL(triggered()), diagramEditor->Diagram, SLOT(deleteSelected()));

    storeDiagramAction = new QAction(QIcon(":/icons/saveIcon.png"),STRING_ACTION_STORE, this);
    connect(storeDiagramAction,SIGNAL(triggered()),this, SLOT(storeDiagram()));

    exportAsHtmlAction = new QAction(QIcon(":/icons/exportHtmlIcon.png"),STRING_TESTS_AS_HTML, this);
    connect(exportAsHtmlAction,SIGNAL(triggered()),this, SLOT(exportHtml()));

    exportAsTxtAction = new QAction(QIcon(":/icons/exportTxtIcon.png"),STRING_TESTS_AS_TXT, this);
    connect(exportAsTxtAction,SIGNAL(triggered()),this, SLOT(exportTxt()));

    newDiagramAction = new QAction(QIcon(":/icons/newSequenceIcon.png"), STRING_NEW_DIAGRAM, this);
    connect(newDiagramAction,SIGNAL(triggered()),this, SLOT(newDiagram()));

    resetZoomAction = new QAction(QIcon(":/icons/resetZoomIcon.png"),STRING_RESET_ZOOM, this);
    connect(resetZoomAction,SIGNAL(triggered()),this, SLOT(resetZoom()));

    showHelpAction = new QAction(STRING_MENU_HELP, this);
    connect(showHelpAction,SIGNAL(triggered()),this, SLOT(showHelp()));

    clearDiagramAction = new QAction(QIcon(":/icons/clearIcon.png"),STRING_CLEAR, this);
    connect(clearDiagramAction,SIGNAL(triggered()),this, SLOT(clearDiagram()));

    centerDiagramAction = new QAction(QIcon(":/icons/centerIcon.png"), STRING_CENTER_DIAGRAM, this);
    connect(centerDiagramAction,SIGNAL(triggered()),this, SLOT(centerOnDiagram()));

    saveImageAction = new QAction(QIcon(":/icons/pictureIcon.png"), STRING_ACTION_EXPORT_IMAGE, this);
    connect(saveImageAction,SIGNAL(triggered()),this, SLOT(saveAsPicture()));

}
//--------------------------------------------------------------------------------------------------------------------------------

void sequencerWindow::createToolBar()
{
    QToolBar *tb = new QToolBar(this);
    tb->setMovable(false);
    tb->addAction(storeDiagramAction);
    tb->addSeparator();
    tb->addAction(addStateAction);
    tb->addAction(connectAction);
    tb->addSeparator();
    tb->addAction(addInitialStateAction);
    tb->addAction(addFinalStateAction);
    tb->addSeparator();
    tb->addAction(resetZoomAction);
    tb->addAction(centerDiagramAction);
    tb->addSeparator();
    tb->addAction(deleteSelectedAction);
    tb->addSeparator();
    tb->addAction(exportAsHtmlAction);
    tb->addAction(exportAsTxtAction);
    tb->addAction(saveImageAction);

    this->addToolBar(tb);

}
//--------------------------------------------------------------------------------------------------------------------------------

void sequencerWindow::closeEvent(QCloseEvent *event)
{

    if (diagramEditor->Diagram->hasChanged) {

        QMessageBox msg(QMessageBox::Warning, STRING_SEQUENCE_DESIGNER, STRING_DESIGN_HAS_UNSAVED_CHANGES, QMessageBox::Save | QMessageBox::Ignore | QMessageBox::Cancel);
        int decission = msg.exec();

        if (decission == QMessageBox::Save ){
            storeDiagram();
            event->accept();

        }else if(decission == QMessageBox::Cancel){
            event->ignore();

        }else if(decission == QMessageBox::Ignore){
            event->accept();

        }

    } else {
        event->accept();

    }
}
//--------------------------------------------------------------------------------------------------------------------------------

void sequencerWindow::newDiagram()
{
    diagramEditor->Diagram->addInitialState();
    diagramEditor->Diagram->addState();
    diagramEditor->Diagram->addFinalState();
}
//--------------------------------------------------------------------------------------------------------------------------------

void sequencerWindow::resetZoom()
{
    diagramEditor->resetTransform();
}
//--------------------------------------------------------------------------------------------------------------------------------

void sequencerWindow::createMenu()
{
    QMenu *mnu = menuBar()->addMenu(STRING_DESIGN);
    mnu->addAction(storeDiagramAction);
    mnu->addAction(newDiagramAction);
    mnu->addAction(resetZoomAction);
    mnu->addAction(saveImageAction);
    mnu->addAction(centerDiagramAction);
    mnu->addAction(clearDiagramAction);

    QMenu *edit = menuBar()->addMenu(STRING_MENU_EDIT);
    edit->addAction(addStateAction);
    edit->addAction(connectAction);
    edit->addAction(addInitialStateAction);
    edit->addAction(addFinalStateAction);
    edit->addAction(deleteSelectedAction);

    QMenu *tests = menuBar()->addMenu(STRING_TESTS);
    tests->addAction(exportAsHtmlAction);
    tests->addAction(exportAsTxtAction);

    QMenu *hlp = menuBar()->addMenu(STRING_MENU_HELP);
    hlp->addAction(showHelpAction);

}
//--------------------------------------------------------------------------------------------------------------------------------

void sequencerWindow::clearDiagram()
{
    diagramEditor->Diagram->clearDiagram();

}
//--------------------------------------------------------------------------------------------------------------------------------

sequencerWindow::~sequencerWindow()
{
    delete diagramEditor;

}
//--------------------------------------------------------------------------------------------------------------------------------

int sequencerWindow::instance = 0;
