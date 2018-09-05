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

#include "crossChecker/crossCheckerWindow.h"

crossCheckerWindow::crossCheckerWindow(QWidget *parent, QTreeWidgetItem *testItem) : QMainWindow(parent)
{
    table = new dataTable();
    table->setSelectionBehavior(QAbstractItemView::SelectItems);
    this->setCentralWidget(table);
    this->setWindowTitle(testItem->text(0));
    this->setWindowIcon(QIcon(":/icons/crossCheckerIcon.png"));
    this->resize(900,600);
    this->setMinimumSize(900,600);
    this->setAttribute(Qt::WA_DeleteOnClose);
    crossCheckerWindow::instance = 1;
    activeTestItem = testItem;
    createActions();
    createMenus();
    rowsParentLabel = new QLabel("");
    columnsParentLabel = new QLabel("");
    andWithLabel = new QLabel("");
    createToolBar();
    loadStoredData();
    statusBar()->show();
    hasChanges = false;
    selectedColumn = -1;

}
//----------------------------------------------------------------------------------------------------

int crossCheckerWindow::instance = 0;
QTreeWidgetItem* crossCheckerWindow::clickedItem;

//----------------------------------------------------------------------------------------------------

QList<QStringList> crossCheckerWindow::clickedItemColumns()
{
    QList<QStringList> storedTable;

    if (crossCheckerWindow::clickedItem!=0){
        QList <QVariant> designData = crossCheckerWindow::clickedItem->data(2,Qt::UserRole).toList();

        if (designData.size() > 0){

            for (int i = 3; i < designData.size(); i++){
                storedTable.append(designData.at(i).toStringList());

            }
        }
    }

    return storedTable;
}
//----------------------------------------------------------------------------------------------------

void crossCheckerWindow::clearHighlights()
{
    table->clearTableBackground();
    clearHighlightsAction->setVisible(false);
    andWithLabel->setText("");
}
//----------------------------------------------------------------------------------------------------

void crossCheckerWindow::andWithSelected()
{
    if ((crossCheckerWindow::clickedItem!=0) &&
        (crossCheckerWindow::clickedItem->data(0,Qt::UserRole).toString() == TAG_TYPE_TEST_CROSSCHECK)){
        andWithLabel->setText(" & " + crossCheckerWindow::clickedItem->text(0));
        clearHighlightsAction->setVisible(true);
        QList<QStringList> reference = clickedItemColumns();
        table->andByCells(reference);
        crossCheckerWindow::clickedItem = 0;

    }else{
        statusBar()->showMessage(tr("Must select a valid cross check to AND with..."), 3000);

    }
}
//----------------------------------------------------------------------------------------------------

void crossCheckerWindow::storeChecker()
{
    writeTableToCache();

    QList <QVariant> tableData;
    bool allDataInPlace = false;

    if((headerParents.size()>0) && (rowHeaders.size()>0) && (columnHeaders.size()>0))
        allDataInPlace = true;

    if (allDataInPlace){
        tableData.append(headerParents);
        tableData.append(rowHeaders);
        tableData.append(columnHeaders);

        QVariant dataBuff;

        for (int i = 0; i < columnsCache.size(); i++){
            dataBuff.setValue(columnsCache.at(i));
            tableData.append(dataBuff);

        }
    }

    if(tableData.size() > 0){
        activeTestItem->setData(2,Qt::UserRole, tableData);
        statusBar()->showMessage(STRING_DESIGN_STORED, 3000);
        hasChanges = false;
        emit saved();

    }else{
        statusBar()->showMessage(STRING_DESIGN_NOT_STORED, 3000);

    }
}
//----------------------------------------------------------------------------------------------------

void crossCheckerWindow::moveColumnLeft()
{
    QList<int> selIdxs = getSelectedIndexes();
    QString headerToMove;
    QStringList columnToMove;
    int nextPosIdx;
    writeTableToCache();

    for (int i = 0; i < selIdxs.size(); i++){
        headerToMove = columnHeaders.at(selIdxs.at(i));
        columnToMove = columnsCache.at(selIdxs.at(i));
        columnHeaders.removeAt(selIdxs.at(i));
        columnsCache.removeAt(selIdxs.at(i));
        nextPosIdx = selIdxs.at(i) - 1;

        if (nextPosIdx < 0){
            columnHeaders.append(headerToMove);
            columnsCache.append(columnToMove);
            nextPosIdx = columnsCache.size()-1;

        }else{
            columnHeaders.insert(nextPosIdx,headerToMove);
            columnsCache.insert(nextPosIdx,columnToMove);

        }

        selectedColumn = nextPosIdx;
    }

    updateTable();
    hasChanges = true;
}
//----------------------------------------------------------------------------------------------------

void crossCheckerWindow::moveColumnRight()
{
    QList<int> selIdxs = getSelectedIndexes();
    QString headerToMove;
    QStringList columnToMove;
    int nextPosIdx;
    writeTableToCache();

    for (int i = 0; i < selIdxs.size(); i++){
        headerToMove = columnHeaders.at(selIdxs.at(i));
        columnToMove = columnsCache.at(selIdxs.at(i));
        columnHeaders.removeAt(selIdxs.at(i));
        columnsCache.removeAt(selIdxs.at(i));
        nextPosIdx = selIdxs.at(i) + 1;

        if (nextPosIdx > columnsCache.size()){
            columnHeaders.prepend(headerToMove);
            columnsCache.prepend(columnToMove);
            nextPosIdx = 0;

        }else{
            columnHeaders.insert(nextPosIdx,headerToMove);
            columnsCache.insert(nextPosIdx,columnToMove);

        }

        selectedColumn = nextPosIdx;
    }

    updateTable();
    hasChanges = true;
}
//----------------------------------------------------------------------------------------------------

void crossCheckerWindow::shiftSelectedUp()
{
    writeTableToCache();
    QList<int> selIdxs = getSelectedIndexes();
    QStringList cBuff;
    QString shiftBuff;

    for (int i = 0; i < selIdxs.size(); i++){
        cBuff = columnsCache.at(selIdxs.at(i));
        shiftBuff = cBuff.first();
        cBuff.removeFirst();
        cBuff.append(shiftBuff);
        columnsCache.replace(selIdxs.at(i),cBuff);

    }

    updateTable();
    hasChanges = true;
}
//----------------------------------------------------------------------------------------------------

void crossCheckerWindow::shiftSelectedDown()
{
    writeTableToCache();
    QList<int> selIdxs = getSelectedIndexes();
    QStringList cBuff;
    QString shiftBuff;

    for (int i = 0; i < selIdxs.size(); i++){
        cBuff = columnsCache.at(selIdxs.at(i));
        shiftBuff = cBuff.last();
        cBuff.removeLast();
        cBuff.prepend(shiftBuff);
        columnsCache.replace(selIdxs.at(i),cBuff);

    }

    updateTable();
    hasChanges = true;
}
//----------------------------------------------------------------------------------------------------

void crossCheckerWindow::applyCoverageTo(QList<QStringList> *columnsList)
{
    writeTableToCache();

    if(columnsList->size() * rowHeaders.size() > 0){
        bool ok;
        int cov = QInputDialog::getInt(this,STRING_COVERAGE,STRING_COVERAGE_PERCENT,33,1,100,1,&ok);

        if(ok){
            int nCells = columnsList->size() * rowHeaders.size();
            int toCover = qRound(nCells * static_cast<double>(cov/100.0));
            int byCol = qRound(static_cast<double>(toCover / columnsList->size()));
            if (byCol == 0) byCol = 1;
            QString tag = QDate::currentDate().toString("yyyy/MM/dd");
            QStringList colBuff;
            int n;
            int row = 0;

            for (int i = 0; i < columnsList->size(); i++){
                colBuff = columnsList->at(i);
                n = 0;

                for(int j = row; j < colBuff.size(); j++){

                    if (colBuff.at(j)==""){
                        colBuff.replace(j,tag);
                        n++;
                    }

                    row++;
                    if(row == colBuff.size()) row = 0;
                    if(n == byCol) break;
                }

                columnsList->replace(i,colBuff);
            }
        }

    }else{
        QMessageBox msg(QMessageBox::Information, STRING_CROSS_CHECKER, STRING_DESIGN_EMPTY, QMessageBox::Ok);
        msg.resize(200,100);
        msg.exec();
        statusBar()->showMessage(STRING_DESIGN_EMPTY, 2000);

    }
}
//----------------------------------------------------------------------------------------------------

void crossCheckerWindow::coverSelected()
{
    writeTableToCache();
    QList<int> selIdxs = getSelectedIndexes();
    QList<QStringList> colsBuff;

    for (int i = 0; i < selIdxs.size(); i++){
        colsBuff.append(columnsCache.at(selIdxs.at(i)));

    }

    applyCoverageTo(&colsBuff);

    for (int i = 0; i < selIdxs.size(); i++){
        columnsCache.replace(selIdxs.at(i),colsBuff.at(i));

    }

    updateTable();
    hasChanges = true;
}
//----------------------------------------------------------------------------------------------------

void crossCheckerWindow::coverAll()
{
    writeTableToCache();
    applyCoverageTo(&columnsCache);
    updateTable();
    hasChanges = true;

}
//----------------------------------------------------------------------------------------------------

void crossCheckerWindow::loadStoredData()
{
    QStringList checkBuff;
    QList <QVariant> designData = activeTestItem->data(2,Qt::UserRole).toList();

    if (designData.size() > 0){
        headerParents = designData.at(0).toStringList();
        rowsParentLabel->setText(tr(STRING_ROWS_SOURCE) + headerParents.at(0));
        columnsParentLabel->setText(tr(STRING_COLUMNS_SOURCE) + headerParents.at(1));

        rowHeaders = designData.at(1).toStringList();
        columnHeaders = designData.at(2).toStringList();

        for (int i = 3; i < designData.size(); i++){
            columnsCache.append(designData.at(i).toStringList());

        }

        updateTable();
    }

}
//----------------------------------------------------------------------------------------------------

void crossCheckerWindow::loadHeaderParent(int element, QString parentText)
{
    while(headerParents.size() < 2){
        headerParents.append("");
    }

    if (element == crossCheckerWindow::rows){
        headerParents.replace(0, parentText);
        rowsParentLabel->setText(tr(STRING_ROWS_SOURCE) + parentText);

    }

    if (element == crossCheckerWindow::columns){
        headerParents.replace(1, parentText);
        columnsParentLabel->setText(tr(STRING_COLUMNS_SOURCE) + parentText);

    }
}
//----------------------------------------------------------------------------------------------------

void crossCheckerWindow::writeHtmlStyle()
{
    xmlWriter->writeTextElement("style","h1 { font-family: 'Arial'; "
                                            "font-size: 14px;}"
                                        ".vertical { font-family: 'Arial'; "
                                            "font-size: 12px;"
                                            "font-weight: bold;"
                                            "text-align: center;"
                                            "transform:rotate(270deg);}"
                                        ".horizontal { font-family: 'Arial'; "
                                            "font-size: 12px;"
                                            "font-weight: bold;"
                                            "text-align: center;}"
                                        "th { font-family: 'Arial'; "
                                            "font-size: 12px;"
                                            "text-align: center;}"
                                        ".rowh { font-family: 'Arial'; "
                                                "font-size: 12px;"
                                                "font-weight: bold;"
                                                "text-align: right;}"
                                        ".cell { font-family: 'Arial'; "
                                                "font-size: 12px;"
                                                "text-align: center;}"
                                        "table, th, td { border: 1px solid black;"
                                                    "border-collapse: collapse;"
                                                    "padding: 10px;"
                                                    "border-color: #AAAAAA;}");
}
//----------------------------------------------------------------------------------------------------

void crossCheckerWindow::writeHtmlH1()
{
    xmlWriter->writeStartElement("h1");
    xmlWriter->writeAttribute("onClick","selectAll();");
    xmlWriter->writeCharacters(QString(STRING_CROSS_COVERAGE) + ": " + activeTestItem->text(0));
    xmlWriter->writeEndElement();
}
//----------------------------------------------------------------------------------------------------

void crossCheckerWindow::writeColumnHeaders()
{
    xmlWriter->writeStartElement("tr");
        xmlWriter->writeStartElement("td");
            xmlWriter->writeAttribute("class","vertical");
            xmlWriter->writeAttribute("rowspan",QString::number(table->rowCount() + 2));
            xmlWriter->writeCharacters(headerParents.at(0));
        xmlWriter->writeEndElement();
        xmlWriter->writeStartElement("td");
            xmlWriter->writeAttribute("class","horizontal");
            xmlWriter->writeAttribute("colspan",QString::number(columnHeaders.count() + 2));
            xmlWriter->writeCharacters(headerParents.at(1));
        xmlWriter->writeEndElement();
    xmlWriter->writeEndElement();

    xmlWriter->writeStartElement("tr");
    xmlWriter->writeTextElement("th","");

    for (int i = 0; i < columnHeaders.size(); i++){
        xmlWriter->writeTextElement("th",columnHeaders.at(i));

    }

    xmlWriter->writeEndElement();
}
//----------------------------------------------------------------------------------------------------

void crossCheckerWindow::writeDataRows()
{
    QStringList colBuff;

    for (int i = 0; i < table->rowCount(); i++){
        xmlWriter->writeStartElement("tr");
            xmlWriter->writeStartElement("td");
                xmlWriter->writeAttribute("class","rowh");
                xmlWriter->writeCharacters(rowHeaders.at(i));
            xmlWriter->writeEndElement();

        for (int j = 0; j < columnsCache.size(); j++){
            colBuff = columnsCache.at(j);

            if(i < colBuff.size()){
                xmlWriter->writeStartElement("td");
                    xmlWriter->writeAttribute("class","cell");
                    xmlWriter->writeCharacters(colBuff.at(i));
                xmlWriter->writeEndElement();

            }
        }

        xmlWriter->writeEndElement();
    }
}
//--------------------------------------------------------------------------------------------------------------------------------

bool crossCheckerWindow::writeHtmlTo(QString filePath)
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
                writeHtmlH1();
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
//----------------------------------------------------------------------------------------------------

QStringList crossCheckerWindow::getSelectionChildren()
{
    QTreeWidgetItem *item;
    QStringList output;

    if (crossCheckerWindow::clickedItem != 0){

        for (int i = 0; i < crossCheckerWindow::clickedItem->childCount(); i++){
            item = crossCheckerWindow::clickedItem->child(i);
            output.append(item->text(0));

        }
    }

    return output;
}
//----------------------------------------------------------------------------------------------------

void crossCheckerWindow::setHeaderFor(int tablePart)
{
    QStringList hBuff = getSelectionChildren();

    if (hBuff.size() > 0){
        writeTableToCache();

        if(tablePart == crossCheckerWindow::rows){
            this->rowHeaders = hBuff;
            loadHeaderParent(tablePart, crossCheckerWindow::clickedItem->text(0));

        }else if(tablePart == crossCheckerWindow::columns){
            this->columnHeaders = hBuff;
            loadHeaderParent(tablePart, crossCheckerWindow::clickedItem->text(0));

        }

        resetCache();
        updateTable();
        hasChanges = true;
    }
}
//----------------------------------------------------------------------------------------------------

void crossCheckerWindow::resetCache()
{
    columnsCache.clear();
    QStringList vals;

    for(int n = 0; n < rowHeaders.size(); n++){
        vals.append("");
    }

    for(int n = 0; n < columnHeaders.size(); n++){
        columnsCache.append(vals);
    }
}
//----------------------------------------------------------------------------------------------------

void crossCheckerWindow::pickColumnHeadersFromTree()
{
    setHeaderFor(crossCheckerWindow::columns);
}
//----------------------------------------------------------------------------------------------------

void crossCheckerWindow::pickRowHeadersFromTree()
{
    setHeaderFor(crossCheckerWindow::rows);
}
//----------------------------------------------------------------------------------------------------

void crossCheckerWindow::closeEvent(QCloseEvent *event)
{
    bool doClose = false;

    if (hasChanges) {

        QMessageBox msg(QMessageBox::Warning, STRING_CROSS_CHECKER, STRING_DESIGN_HAS_UNSAVED_CHANGES, QMessageBox::Save | QMessageBox::Ignore | QMessageBox::Cancel);
        int decission = msg.exec();

        if (decission == QMessageBox::Save ){
            storeChecker();
            doClose = true;

        }else if(decission == QMessageBox::Cancel){
            event->ignore();

        }else if(decission == QMessageBox::Ignore){
            doClose = true;

        }

    } else {
        doClose = true;

    }

    if (doClose){
        crossCheckerWindow::instance = 0;
        event->accept();

    }
}
//----------------------------------------------------------------------------------------------------

void crossCheckerWindow::exportChecker()
{
    QString filePath = QFileDialog::getSaveFileName(this, QObject::tr(STRING_SAVE_TESTCAD_FILE),
                                         QDir::currentPath(),
                                         QObject::tr(STRING_FILETYPE_HTML));

    writeTableToCache();

    if (!filePath.endsWith(".html"))
        filePath = filePath + ".html";

    if(writeHtmlTo(filePath)){
        statusBar()->showMessage(tr(STRING_FILE_SAVED),3000);

    }else{
        QMessageBox msg(QMessageBox::Warning,STRING_CROSS_CHECKER,STRING_FILE_NOT_SAVED,QMessageBox::Ok);
        msg.resize(200,100);
        msg.exec();
        statusBar()->showMessage(tr(STRING_FILE_NOT_SAVED),3000);

    }

}
//----------------------------------------------------------------------------------------------------

void crossCheckerWindow::createToolBar()
{
    QToolBar *tb = this->addToolBar(tr(STRING_MENU_TOOLS));
    tb->setMovable(false);
    tb->addAction(saveCheckerAction);
    tb->addAction(exportCheckerAction);
    tb->addAction(pickColumnHeadersAction);
    tb->addAction(pickRowHeadersAction);
    tb->addAction(coverAllAction);
    tb->addAction(coverSelectedAction);
    tb->addAction(moveColumnLeftAction);
    tb->addAction(moveColumnRightAction);
    tb->addAction(deleteSelectedColumnsAction);
    tb->addSeparator();
    tb->addAction(shiftUpAction);
    tb->addAction(shiftDownAction);
    tb->addSeparator();
    tb->addAction(andAction);
    tb->addSeparator();
    tb->addAction(clearHighlightsAction);
    tb->addWidget(andWithLabel);
    tb->addSeparator();
    tb->addWidget(rowsParentLabel);
    tb->addSeparator();
    tb->addWidget(columnsParentLabel);

}
//----------------------------------------------------------------------------------------------------

void crossCheckerWindow::deleteSelectedColumns()
{
    QList<int> selIdxs = getSelectedIndexes();

    for (int i = 0; i < selIdxs.size(); i++){
        columnHeaders.removeAt(selIdxs.at( selIdxs.size() - 1 - i ) );
        columnsCache.removeAt(selIdxs.at( selIdxs.size() - 1 - i ));

    }

    updateTable();
    hasChanges = true;

}
//----------------------------------------------------------------------------------------------------

QList<int> crossCheckerWindow::getSelectedIndexes()
{
    QList<int> selIdxs;

    if (table->model->hasIndex(0,0)){
        QModelIndex index;
        QModelIndexList indexes = table->currentSelection->selectedColumns();

        foreach (index, indexes){
            selIdxs.append(index.column());

        }
    }

    return selIdxs;
}
//----------------------------------------------------------------------------------------------------

void crossCheckerWindow::showHelp()
{
    QMessageBox::about(this,tr(STRING_CROSS_CHECKER), tr(STRING_CROSS_CHECKER_HELP));

}
//----------------------------------------------------------------------------------------------------

void crossCheckerWindow::clearTable()
{
    QStringList colBuff;

    for (int i = 0; i < columnsCache.size(); i++){
        colBuff = columnsCache.at(i);

        for (int j = 0; j < colBuff.size(); j++){
            colBuff.replace(j,"");
        }

        columnsCache.replace(i,colBuff);
    }

    updateTable();
}
//----------------------------------------------------------------------------------------------------

void crossCheckerWindow::updateTable()
{
    table->clear();
    table->setColumnHeaders(columnHeaders);
    table->setRowHeaders(rowHeaders);
    table->setColumnValues(columnsCache);
    table->update();

    if (selectedColumn > -1){
        table->selectColumn(selectedColumn);
        selectedColumn = -1;

    }

}
//----------------------------------------------------------------------------------------------------

void crossCheckerWindow::createActions()
{
    icons = new iconsCatalog;

    saveCheckerAction = new QAction(icons->saveIcon, STRING_ACTION_STORE,this);
    connect(saveCheckerAction,SIGNAL(triggered()),this,SLOT(storeChecker()));

    exportCheckerAction = new QAction(icons->htmlIcon, STRING_ACTION_EXPORT,this);
    connect(exportCheckerAction,SIGNAL(triggered()),this,SLOT(exportChecker()));

    pickColumnHeadersAction = new QAction(icons->pickColumnHeadersIcon, STRING_ACTION_GET_COLUMN_HEADERS,this);
    connect(pickColumnHeadersAction,SIGNAL(triggered()),this,SLOT(pickColumnHeadersFromTree()));

    pickRowHeadersAction = new QAction(icons->pickRowHeadersIcon, STRING_ACTION_GET_ROW_HEADERS,this);
    connect(pickRowHeadersAction,SIGNAL(triggered()),this,SLOT(pickRowHeadersFromTree()));

    showHelpAction = new QAction(STRING_MENU_HELP,this);
    connect(showHelpAction,SIGNAL(triggered()),this,SLOT(showHelp()));

    deleteSelectedColumnsAction= new QAction(icons->deleteIcon, STRING_ACTION_DELETE_SELECTED_COLUMNS,this);
    connect(deleteSelectedColumnsAction,SIGNAL(triggered()),this,SLOT(deleteSelectedColumns()));

    clearTableAction = new QAction(QIcon(":/icons/clearIcon.png"),STRING_ACTION_CLEAR_TABLE,this);
    connect(clearTableAction,SIGNAL(triggered()),this,SLOT(clearTable()));

    coverSelectedAction = new QAction(icons->fillIcon, STRING_ACTION_COVER_SELECTED,this);
    connect(coverSelectedAction,SIGNAL(triggered()),this,SLOT(coverSelected()));

    moveColumnLeftAction = new QAction(icons->leftIcon, STRING_ACTION_MOVE_LEFT,this);
    connect(moveColumnLeftAction,SIGNAL(triggered()),this,SLOT(moveColumnLeft()));

    moveColumnRightAction = new QAction(icons->rightIcon, STRING_ACTION_MOVE_RIGHT,this);
    connect(moveColumnRightAction,SIGNAL(triggered()),this,SLOT(moveColumnRight()));

    coverAllAction = new QAction(icons->coverIcon, STRING_ACTION_COVER_ALL,this);
    connect(coverAllAction,SIGNAL(triggered()),this,SLOT(coverAll()));

    shiftUpAction = new QAction(icons->upIcon, STRING_ACTION_SHIFT_UP,this);
    connect(shiftUpAction,SIGNAL(triggered()),this,SLOT(shiftSelectedUp()));

    shiftDownAction = new QAction(icons->downIcon, STRING_ACTION_SHIFT_DOWN,this);
    connect(shiftDownAction,SIGNAL(triggered()),this,SLOT(shiftSelectedDown()));

    andAction = new QAction(icons->andIcon, "And with selected",this);
    connect(andAction,SIGNAL(triggered()),this,SLOT(andWithSelected()));

    clearHighlightsAction = new QAction(icons->clearHighlightsIcon, QObject::tr(STRING_ACTION_CLEAR_HIGHLIGHTS), this);
    clearHighlightsAction->setVisible(false);
    connect(clearHighlightsAction, SIGNAL(triggered()), this, SLOT(clearHighlights()));

}
//----------------------------------------------------------------------------------------------------

void crossCheckerWindow::createMenus()
{
    QMenuBar *mb = this->menuBar();

    QMenu *saveMenu = mb->addMenu(STRING_DESIGN);
        saveMenu->addAction(saveCheckerAction);
        saveMenu->addAction(exportCheckerAction);
        saveMenu->addAction(clearTableAction);

    QMenu *variablesMenu = mb->addMenu(STRING_MENU_VARIABLES);
        variablesMenu->addAction(pickRowHeadersAction);
        variablesMenu->addAction(pickColumnHeadersAction);
        variablesMenu->addSeparator();
        variablesMenu->addAction(coverAllAction);
        variablesMenu->addAction(coverSelectedAction);

    QMenu *columnsMenu = mb->addMenu(STRING_MENU_COLUMNS);
        columnsMenu->addAction(deleteSelectedColumnsAction);
        columnsMenu->addAction(moveColumnLeftAction);
        columnsMenu->addAction(moveColumnRightAction);

    QMenu *helpMenu = mb->addMenu(STRING_MENU_HELP);
        helpMenu->addAction(showHelpAction);

}
//----------------------------------------------------------------------------------------------------

void crossCheckerWindow::writeTableToCache()
{
    columnHeaders = table->getColumnHeaders();
    rowHeaders = table->getRowHeaders();
    columnsCache = table->getColumnValues();

}
//----------------------------------------------------------------------------------------------------

crossCheckerWindow::~crossCheckerWindow()
{

}


