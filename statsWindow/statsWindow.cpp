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

#include "statsWindow/statsWindow.h"

statsWindow::statsWindow(QWidget *parent, QList<statistics> *treeStatistics) : QMainWindow(parent)
{
    table = new dataTable();
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->setCentralWidget(table);
    this->setWindowTitle(tr("System Statistics"));
    this->setMinimumSize(500,500);
    createActions();
    createMenus();
    createToolBar();
    statusBar()->show();
    selectedRow = -1;
    loadStatistics(treeStatistics);

}
//----------------------------------------------------------------------------------------------------

void statsWindow::moveRowUp()
{
    writeTableToCache();
    QList<int> selIdxs = getSelectedRowsIds();
    int selectedRowId = selIdxs.at(0);
    QStringList colBuff;
    QStringList rowBuff;
    QString headerToMove;
    int nextPosIdx;

    headerToMove = rowHeaders.at(selectedRowId);
    rowHeaders.removeAt(selectedRowId);

    for (int j = 0; j < columnsCache.size(); j++){
        colBuff = columnsCache.at(j);
        rowBuff.append(colBuff.at(selectedRowId));
        colBuff.removeAt(selectedRowId);
        columnsCache.replace(j,colBuff);

    }

    nextPosIdx = selectedRowId - 1;

    if (nextPosIdx < 0){
        rowHeaders.append(headerToMove);

        for (int j = 0; j < rowBuff.size(); j++){
            colBuff = columnsCache.at(j);
            colBuff.append(rowBuff.at(j));
            columnsCache.replace(j, colBuff);

        }

        nextPosIdx = rowHeaders.size()-1;

    }else{
        rowHeaders.insert(nextPosIdx,headerToMove);

        for (int j = 0; j < rowBuff.size(); j++){
            colBuff = columnsCache.at(j);
            colBuff.insert(nextPosIdx, rowBuff.at(j));
            columnsCache.replace(j, colBuff);

        }

    }

    selectedRow = nextPosIdx;
    updateTable();
}
//----------------------------------------------------------------------------------------------------

void statsWindow::moveRowDown()
{
    writeTableToCache();
    QList<int> selIdxs = getSelectedRowsIds();
    int selectedRowId = selIdxs.at(0);
    QStringList colBuff;
    QStringList rowBuff;
    QString headerToMove;
    int nextPosIdx;
    int rowsCount = rowHeaders.size();

    headerToMove = rowHeaders.at(selectedRowId);
    rowHeaders.removeAt(selectedRowId);

    for (int j = 0; j < columnsCache.size(); j++){
        colBuff = columnsCache.at(j);
        rowBuff.append(colBuff.at(selectedRowId));
        colBuff.removeAt(selectedRowId);
        columnsCache.replace(j,colBuff);

    }

    nextPosIdx = selectedRowId + 1;

    if (nextPosIdx < rowsCount){
        rowHeaders.insert(nextPosIdx, headerToMove);

        for (int j = 0; j < rowBuff.size(); j++){
            colBuff = columnsCache.at(j);
            colBuff.insert(nextPosIdx, rowBuff.at(j));
            columnsCache.replace(j, colBuff);

        }

    }else{
        rowHeaders.insert(0,headerToMove);

        for (int j = 0; j < rowBuff.size(); j++){
            colBuff = columnsCache.at(j);
            colBuff.insert(0, rowBuff.at(j));
            columnsCache.replace(j, colBuff);

        }
        nextPosIdx = 0;
    }

    selectedRow = nextPosIdx;
    updateTable();
}
//----------------------------------------------------------------------------------------------------

void statsWindow::writeHtmlH1(int componentsCount)
{
    xmlWriter->writeStartElement("h1");
    xmlWriter->writeCharacters("Statistics for " + QString::number(componentsCount) + " components system");
    xmlWriter->writeEndElement();
}
//----------------------------------------------------------------------------------------------------

void statsWindow::writeHtmlStyle()
{
    xmlWriter->writeTextElement("style","h1 { font-family: 'Arial'; "
                                            "font-size: 14px;}"
                                        "th { font-family: 'Arial'; "
                                            "font-size: 12px;}"
                                        "td { font-family: 'Arial'; "
                                            "font-size: 12px;}"
                                        "table, th, td { border: 1px solid black;"
                                                    "border-collapse: collapse;"
                                                    "padding: 10px;"
                                                    "border-color: #AAAAAA;}");
}
//----------------------------------------------------------------------------------------------------

void statsWindow::writeColumnHeaders()
{
    xmlWriter->writeStartElement("tr");
    xmlWriter->writeTextElement("th","Component");

    for (int i = 0; i < columnHeaders.size(); i++){
        xmlWriter->writeTextElement("th",columnHeaders.at(i));

    }

    xmlWriter->writeEndElement();
}
//----------------------------------------------------------------------------------------------------

void statsWindow::writeDataRows()
{
    QStringList colBuff;

    for (int i = 0; i < table->rowCount(); i++){
        xmlWriter->writeStartElement("tr");
        xmlWriter->writeStartElement("td");
        xmlWriter->writeCharacters(rowHeaders.at(i));
        xmlWriter->writeEndElement();

        for (int j = 0; j < columnsCache.size(); j++){
            colBuff = columnsCache.at(j);
            xmlWriter->writeStartElement("td");

            if(i < colBuff.size()){
                xmlWriter->writeCharacters(colBuff.at(i));

            }

            xmlWriter->writeEndElement();

        }

        xmlWriter->writeEndElement();
    }
}
//--------------------------------------------------------------------------------------------------------------------------------

bool statsWindow::writeHtmlTo(QString filePath)
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
        writeHtmlH1(table->rowCount());
        xmlWriter->writeStartElement("table");
        writeColumnHeaders();
        writeDataRows();
        xmlWriter->writeEndElement();
        xmlWriter->writeEndElement();
        xmlWriter->writeEndElement();
        bool hasError = xmlWriter->hasError();
        delete xmlWriter;

        return !hasError;
    }
}
//--------------------------------------------------------------------------------------------------------------------------------

int statsWindow::instance = 0;
//----------------------------------------------------------------------------------------------------

void statsWindow::loadStatistics(QList<statistics> *treeStatistics)
{
    QList<QStringList> valuesByRow;
    QStringList rowVals;
    int colCount;

    setColumnsHeaders();
    rowHeaders.clear();

    for (int i = 0; i < treeStatistics->size(); i++){
        rowHeaders.append(treeStatistics->at(i).componentName);
        rowVals.append(treeStatistics->at(i).variablesCount);
        rowVals.append(treeStatistics->at(i).fullCombinationSize);
        rowVals.append(treeStatistics->at(i).pending);
        rowVals.append(treeStatistics->at(i).pendingPercent);
        rowVals.append(treeStatistics->at(i).planned);
        rowVals.append(treeStatistics->at(i).plannedPercent);
        rowVals.append(treeStatistics->at(i).review);
        rowVals.append(treeStatistics->at(i).reviewPercent);
        rowVals.append(treeStatistics->at(i).validated);
        rowVals.append(treeStatistics->at(i).validatedPercent);
        rowVals.append(treeStatistics->at(i).failed);
        rowVals.append(treeStatistics->at(i).failedPercent);
        rowVals.append(treeStatistics->at(i).unsupported);
        rowVals.append(treeStatistics->at(i).unsupportedPercent);
        rowVals.append(treeStatistics->at(i).modulesCount);
        rowVals.append(treeStatistics->at(i).entriesCount);

        colCount = rowVals.size();
        valuesByRow.append(rowVals);
        rowVals.clear();

    }

    columnsCache.clear();

    for (int i = 0; i < colCount; i++){
       QStringList colVals;

       for(int j = 0; j < valuesByRow.size(); j++){
           rowVals = valuesByRow.at(j);
           colVals.append(rowVals.at(i));

       }

       columnsCache.append(colVals);

    }

    updateTable();

}
//----------------------------------------------------------------------------------------------------

void statsWindow::setColumnsHeaders()
{
    columnHeaders.clear();
    columnHeaders.append("Variables");
    columnHeaders.append("Combinations");
    columnHeaders.append("Pending");
    columnHeaders.append("Pending %");
    columnHeaders.append("Planned");
    columnHeaders.append("Planned %");
    columnHeaders.append("Review");
    columnHeaders.append("Review %");
    columnHeaders.append("Validated");
    columnHeaders.append("Validated %");
    columnHeaders.append("Failed");
    columnHeaders.append("Failed %");
    columnHeaders.append("Unsupported");
    columnHeaders.append("Unsupported %");
    columnHeaders.append("Modules");
    columnHeaders.append("Entries");
}
//----------------------------------------------------------------------------------------------------

void statsWindow::updateTable()
{
    table->clear();
    table->setColumnHeaders(columnHeaders);
    table->setRowHeaders(rowHeaders);
    table->setColumnValues(columnsCache);
    table->update();

    if (selectedRow > -1){
        table->selectRow(selectedRow);
        selectedRow = -1;

    }
}
//----------------------------------------------------------------------------------------------------

void statsWindow::writeTableToCache()
{
    columnHeaders = table->getColumnHeaders();
    columnsCache = table->getColumnValues();
    QStringList colWithBlanks;
    QStringList colWithoutBlanks;

    for (int i = 0; i < columnsCache.size(); i++){
        colWithBlanks = columnsCache.at(i);

        for (int j = 0; j < colWithBlanks.size(); j++){

            if (!colWithBlanks.at(j).isEmpty()){
                colWithoutBlanks.append(colWithBlanks.at(j));

            }
        }

        columnsCache.replace(i, colWithoutBlanks);
        colWithoutBlanks.clear();
    }

}
//----------------------------------------------------------------------------------------------------

void statsWindow::exportStats()
{
    QString filePath = QFileDialog::getSaveFileName(this, QObject::tr("Save statistics"),
                                         QDir::currentPath(),
                                         QObject::tr(STRING_FILETYPE_HTML));

    writeTableToCache();

    if (!filePath.endsWith(".html"))
        filePath = filePath + ".html";

    if(writeHtmlTo(filePath)){
        statusBar()->showMessage(tr("Statistics saved"),3000);

    }else{
        QMessageBox msg(QMessageBox::Warning,tr("System Statistics"),tr("File not saved"),QMessageBox::Ok);
        msg.resize(200,100);
        msg.exec();
        statusBar()->showMessage(tr("File not saved"),3000);

    }

}
//----------------------------------------------------------------------------------------------------

void statsWindow::createToolBar()
{
    QToolBar *tb = this->addToolBar(tr("Tools"));
    tb->setMovable(false);
    tb->addAction(exportStatsAction);
    tb->addSeparator();
    tb->addAction(shiftUpAction);
    tb->addAction(shiftDownAction);
    tb->addSeparator();
    tb->addAction(deleteSelectedRowsAction);

}
//----------------------------------------------------------------------------------------------------

void statsWindow::deleteSelectedRows()
{
    QList<int> selIdxs = getSelectedRowsIds();
    QStringList colBuff;

    for (int i = 0; i < selIdxs.size(); i++){
        rowHeaders.removeAt(selIdxs.at( selIdxs.size() - 1 - i ) );

        for (int j = 0; j < columnsCache.size(); j++){
            colBuff = columnsCache.at(j);
            colBuff.removeAt(selIdxs.at( selIdxs.size() - 1 - i ));
            columnsCache.replace(j,colBuff);

        }
    }

    updateTable();

}
//----------------------------------------------------------------------------------------------------

QList<int> statsWindow::getSelectedRowsIds()
{
    QList<int> selIdxs;

    if (table->model->hasIndex(0,0)){
        QModelIndex index;
        QModelIndexList indexes = table->currentSelection->selectedRows();

        foreach (index, indexes){
            selIdxs.append(index.row());

        }
    }

    return selIdxs;
}
//----------------------------------------------------------------------------------------------------

void statsWindow::showHelp()
{
    QString statsHelp = tr("Your system is composed by:<ul>"\
                           "<li><b>Components</b> that contain (or are composed by) <b>Modules</b>.</li>"\
                           "<li><b>Modules</b> that contain (or are composed by) <b>Elements</b>.</li>"\
                           "<li><b>Elements</b> that are at the bottom of the system hierarchy.</li></ul><br>"\
                           "For test design <b>Modules</b> are considered variables whereas <b>Elements</b> are "\
                           "considered the values the variable may take.<br><br>"\
                           "The <b>Statistics Table</b> shows cuantitative data about your system composition and state.");

    QMessageBox::about(this,tr("System statistics"), statsHelp);

}
//----------------------------------------------------------------------------------------------------

void statsWindow::createActions()
{
    icons = new iconsCatalog;

    exportStatsAction = new QAction(icons->htmlIcon, tr("Export statistics"),this);
    connect(exportStatsAction,SIGNAL(triggered()),this,SLOT(exportStats()));

    showHelpAction = new QAction(tr("Help"),this);
    connect(showHelpAction,SIGNAL(triggered()),this,SLOT(showHelp()));

    deleteSelectedRowsAction= new QAction(icons->deleteIcon, tr("Delete selected rows"),this);
    connect(deleteSelectedRowsAction,SIGNAL(triggered()),this,SLOT(deleteSelectedRows()));

    shiftUpAction = new QAction(icons->upIcon, tr("Move up"),this);
    connect(shiftUpAction,SIGNAL(triggered()),this,SLOT(moveRowUp()));

    shiftDownAction = new QAction(icons->downIcon, tr("Move down"),this);
    connect(shiftDownAction,SIGNAL(triggered()),this,SLOT(moveRowDown()));
}
//----------------------------------------------------------------------------------------------------

void statsWindow::createMenus()
{
    QMenuBar *mb = this->menuBar();

    QMenu *columnsMenu = mb->addMenu(tr("Rows"));
        columnsMenu->addAction(deleteSelectedRowsAction);
        columnsMenu->addAction(shiftUpAction);
        columnsMenu->addAction(shiftDownAction);

    QMenu *helpMenu = mb->addMenu(tr("Help"));
        helpMenu->addAction(showHelpAction);

}
//----------------------------------------------------------------------------------------------------

statsWindow::~statsWindow()
{

}
