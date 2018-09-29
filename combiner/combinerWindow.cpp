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

#include "combiner/combinerWindow.h"

combinerWindow::combinerWindow(QWidget *parent, QTreeWidgetItem *testItem) : QMainWindow(parent)
{
    table = new dataTable();
    table->setSelectionBehavior(QAbstractItemView::SelectColumns);
    table->skipEmptyCells = true;
    this->setCentralWidget(table);
    this->setWindowTitle(testItem->text(0));
    this->setWindowIcon(QIcon(":/icons/combinationIcon.png"));
    this->resize(800,600);
    this->setMinimumSize(800,600);
    this->setAttribute(Qt::WA_DeleteOnClose);
    combinerWindow::instance = 1;
    connect(table,SIGNAL(clicked(QModelIndex)),this,SLOT(calculateOnSelection(QModelIndex)));
    andWithLabel = new QLabel("");
    activeTestItem = testItem;
    createActions();
    createMenus();
    createToolBar();
    loadStoredData();
    statusBar()->show();
    hasChanges = false;

    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());
}
//----------------------------------------------------------------------------------------------------

void combinerWindow::appendTable()
{
    if ((combinerWindow::clickedItem) &&
        (combinerWindow::clickedItem->data(0,Qt::UserRole).toString()==TAG_TYPE_TEST_COMBINATION)){
        table->cacheTable();
        dataTable *tableToAppend = new dataTable();
        tableToAppend->loadFromData(combinerWindow::clickedItem->data(2,Qt::UserRole).toList());

        if(tableToAppend->columnsCache.size() == table->columnsCache.size()){
            int rowCount = tableToAppend->columnsCache.at(0).size();

            for (int n = 0; n < table->columnsCache.size(); n++){
                QStringList fromBuff = tableToAppend->columnsCache.at(n);
                QStringList toBuff = table->columnsCache.at(n);

                for (int m = 0; m < rowCount; m++){
                    toBuff.append(fromBuff.at(m));
                }

                table->columnsCache.replace(n, toBuff);
            }

            table->loadFromCache();
            hasChanges = true;

        }else{
            QMessageBox msg(QMessageBox::Critical, tr("Appending table"), tr("Table to append doesn't match destination table size."), QMessageBox::Ok);
            msg.exec();
        }

        delete tableToAppend;

    }else{
        QMessageBox msg(QMessageBox::Critical, tr("Appending table"), tr("Please select a combination table."), QMessageBox::Ok);
        msg.exec();
    }
}
//----------------------------------------------------------------------------------------------------

void combinerWindow::clearTable()
{
    table->clear();
    hasChanges = true;
}
//----------------------------------------------------------------------------------------------------

void combinerWindow::deleteSelectedColumns()
{
    table->deleteSelectedColumns();
    hasChanges = true;

}
//----------------------------------------------------------------------------------------------------

void combinerWindow::pickCollection()
{
    table->loadFromCollection(combinerWindow::clickedItem);
    hasChanges = true;

}
//----------------------------------------------------------------------------------------------------

void combinerWindow::loadStoredData()
{
    table->loadFromData(activeTestItem->data(2,Qt::UserRole).toList());

}
//----------------------------------------------------------------------------------------------------

void combinerWindow::moveColumnRight()
{
    table->moveColumnRight();
    hasChanges = true;

}
//----------------------------------------------------------------------------------------------------

void combinerWindow::moveColumnLeft()
{
    table->moveColumnLeft();
    hasChanges = true;
}
//----------------------------------------------------------------------------------------------------

void combinerWindow::randomizeSelected()
{
    QStringList vals;
    QStringList rndBuff;
    QString strBuff;
    int rndFrom;
    int rndTo;
    int rowCount = table->rowCount();
    SelectionData selDat = getSelectionData();

    for (int i = 0; i < selDat.selectedColumns.size(); i++){
        vals = selDat.selectedColumns.at(i);

        while(rndBuff.size() < rowCount){
            rndBuff.append("");

        }

        while(vals.size() > 0){
            rndFrom = getRandom(vals.size() - 1);
            strBuff = vals.at(rndFrom);
            vals.removeAt(rndFrom);
            rndTo = getRandom(rowCount - 1);

            while(rndBuff.at(rndTo) != ""){
                rndTo = getRandom(rowCount - 1);

            }

            rndBuff.replace(rndTo, strBuff);

        }

        for(int j = 0; j < rndBuff.size(); j++){

            while (rndBuff.at(j) == ""){
                rndFrom = getRandom(rndBuff.size() - 1);
                strBuff = rndBuff.at(rndFrom);
                rndBuff.replace(j, strBuff);

            }
        }

        table->columnsCache.replace(selDat.selectedIndexes.at(i),rndBuff);
        rndBuff.clear();
    }

    table->loadFromCache();
    hasChanges = true;
}
//----------------------------------------------------------------------------------------------------

void combinerWindow::combineSelected()
{
    QList<QStringList> combinedColumns;
    QStringList combinedCol;
    QStringList vals;
    int rowsDivider = 1;
    int rows;
    SelectionData selDat = getSelectionData();

    for (int i = 0; i < selDat.selectedColumns.size(); i++){
        vals = selDat.selectedColumns.at(i);
        rowsDivider = rowsDivider * vals.size();
        rows = 0;

        while (rows < selDat.RowsMultiplied){

            for (int j = 0; j < vals.size(); j++){

                for (int k = 0; k < selDat.RowsMultiplied/rowsDivider; k++){
                    combinedCol.append( vals.at(j) );
                    rows ++;
                }
            }
        }

        combinedColumns.append(combinedCol);
        combinedCol.clear();
        vals.clear();
    }

    for(int i = 0; i < table->columnsCache.size(); i++){

        if(!selDat.selectedIndexes.contains(i)){
            combinedColumns.append( table->columnsCache.at(i) );
            selDat.selectionHeaders.append( table->columnHeaders.at(i) );

        }
    }

    table->columnHeaders = selDat.selectionHeaders;
    table->columnsCache = combinedColumns;
    table->loadFromCache();
    hasChanges = true;

}
//----------------------------------------------------------------------------------------------------

void combinerWindow::removeCovered()
{
    QStringList colBuff;

    for(int i = 0; i < andRows.size(); i++){

        if (andRows.at(andRows.size()-1-i) == true){

            for (int j = 0; j < table->columnsCache.size(); j++){
                colBuff = table->columnsCache.at(j);
                colBuff.removeAt(andRows.size()-1-i);
                table->columnsCache.replace(j,colBuff);

            }
        }
    }

    andRows.clear();
    table->loadFromCache();
    hasChanges = true;
}
//----------------------------------------------------------------------------------------------------

void combinerWindow::fillDownSelected()
{
    SelectionData selDat = getSelectionData();
    QStringList vals;
    int n;
    table->cacheTable();

    for (int i = 0; i < selDat.selectedColumns.size(); i++){
        n = 0;
        vals = selDat.selectedColumns.at(i);

        for (int j = vals.size() - 1; j < table->rowCount() - 1 ; j++){
            vals.append(vals.at(n));
            n++;

        }

        table->columnsCache.replace(selDat.selectedIndexes.at(i),vals);
        vals.clear();
    }

    table->loadFromCache();
    hasChanges = true;
}
//----------------------------------------------------------------------------------------------------

void combinerWindow::shiftSelected(shiftingDirection direction)
{
    QStringList cBuff;
    QString shiftBuff;

    table->cacheTable();
    QList<int> selIdxs = table->getSelectedIndexes(dataTable::selectionColumn);

    for (int i = 0; i < selIdxs.size(); i++){
        cBuff = table->columnsCache.at(selIdxs.at(i));

        if (direction == combinerWindow::shiftDown){
            shiftBuff = cBuff.last();
            cBuff.removeLast();
            cBuff.prepend(shiftBuff);

        }else if(direction == combinerWindow::shiftUp){
            shiftBuff = cBuff.first();
            cBuff.removeFirst();
            cBuff.append(shiftBuff);

        }

        table->columnsCache.replace(selIdxs.at(i), cBuff);

    }

    if (!selIdxs.isEmpty()){
        table->loadFromCache();
        table->selectColumn(selIdxs.first());
        hasChanges = true;
    }
}
//----------------------------------------------------------------------------------------------------

void combinerWindow::shiftSelectedUp()
{
    shiftSelected(combinerWindow::shiftUp);
}
//----------------------------------------------------------------------------------------------------

void combinerWindow::shiftSelectedDown()
{
    shiftSelected(combinerWindow::shiftDown);
}
//----------------------------------------------------------------------------------------------------

int combinerWindow::instance = 0;
QTreeWidgetItem* combinerWindow::clickedItem = 0;

//----------------------------------------------------------------------------------------------------

void combinerWindow::clearHighlights()
{
    table->clearTableBackground();
    clearHighlightsAction->setVisible(false);
    andWithLabel->setText("");
}
//----------------------------------------------------------------------------------------------------

void combinerWindow::andWithSelected()
{
    if ((combinerWindow::clickedItem!=0) &&
        (combinerWindow::clickedItem->data(0,Qt::UserRole).toString() == TAG_TYPE_TEST_COMBINATION)){
        andWithLabel->setText(" & " + combinerWindow::clickedItem->text(0));
        clearHighlightsAction->setVisible(true);
        QList<QStringList> reference = clickedItemColumns();
        andRows = table->andByRows(reference);
        combinerWindow::clickedItem = 0;

    }else{
        statusBar()->showMessage(tr("Must select a valid combination to AND rows with..."), 3000);

    }
}
//----------------------------------------------------------------------------------------------------

QList<QStringList> combinerWindow::clickedItemColumns()
{
    QList<QStringList> storedTable;

    if (combinerWindow::clickedItem!=0){
        QList <QVariant> designData = combinerWindow::clickedItem->data(2,Qt::UserRole).toList();

        if (designData.size() > 0){

            for (int i = 1; i < designData.size(); i++){
                storedTable.append(designData.at(i).toStringList());
            }
        }
    }

    return storedTable;
}
//----------------------------------------------------------------------------------------------------

void combinerWindow::writeHtmlStyle()
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

void combinerWindow::writeHtmlH1(int combinationsCount)
{
    xmlWriter->writeStartElement("h1");
    xmlWriter->writeCharacters(activeTestItem->text(0) + ": " + QString::number(combinationsCount) + " " + QString(STRING_COMBINATIONS));
    xmlWriter->writeEndElement();
}
//----------------------------------------------------------------------------------------------------

void combinerWindow::writeColumnHeaders()
{
    xmlWriter->writeStartElement("tr");
    xmlWriter->writeTextElement("th","#");

    for (int i = 0; i < table->columnHeaders.size(); i++){
        xmlWriter->writeTextElement("th",table->columnHeaders.at(i));

    }

    xmlWriter->writeEndElement();
}
//----------------------------------------------------------------------------------------------------

void combinerWindow::writeDataRows()
{
    QStringList colBuff;

    for (int i = 0; i < table->rowCount(); i++){
        xmlWriter->writeStartElement("tr");
        xmlWriter->writeStartElement("td");
        xmlWriter->writeCharacters(QString::number(i + 1));
        xmlWriter->writeEndElement();

        for (int j = 0; j < table->columnsCache.size(); j++){
            colBuff = table->columnsCache.at(j);
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

bool combinerWindow::writeHtmlTo(QString filePath)
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
//----------------------------------------------------------------------------------------------------

void combinerWindow::closeEvent(QCloseEvent *event)
{
    bool doClose = false;

    if (hasChanges) {

        QMessageBox msg(QMessageBox::Warning, STRING_COMBINER, STRING_DESIGN_HAS_UNSAVED_CHANGES, QMessageBox::Save | QMessageBox::Ignore | QMessageBox::Cancel);
        int decission = msg.exec();

        if (decission == QMessageBox::Save ){
            saveCombination();
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
        combinerWindow::instance = 0;
        event->accept();

    }
}
//----------------------------------------------------------------------------------------------------

void combinerWindow::exportCombination()
{
    QString filePath = QFileDialog::getSaveFileName(this, QObject::tr(STRING_SAVE_TESTCAD_FILE),
                                         QDir::currentPath(),
                                         QObject::tr(STRING_FILETYPE_HTML));

    table->cacheTable();

    if (!filePath.endsWith(".html"))
        filePath = filePath + ".html";

    if(writeHtmlTo(filePath)){
        statusBar()->showMessage(tr(STRING_FILE_SAVED),3000);

    }else{
        QMessageBox msg(QMessageBox::Warning,STRING_COMBINER,STRING_FILE_NOT_SAVED,QMessageBox::Ok);
        msg.resize(200,100);
        msg.exec();
        statusBar()->showMessage(tr(STRING_FILE_NOT_SAVED),3000);

    }

}
//----------------------------------------------------------------------------------------------------

void combinerWindow::calculateOnSelection(const QModelIndex & index)
{
    Q_UNUSED(index);
    SelectionData selDat = getSelectionData();
    QString resultingRows = QString::number(selDat.RowsMultiplied) + STRING_COMBINATIONS_COUNT;
    statusBar()->showMessage(tr(resultingRows.toLocal8Bit()),3000);

}
//----------------------------------------------------------------------------------------------------

void combinerWindow::createToolBar()
{

    QToolBar *tb = this->addToolBar(tr(STRING_MENU_TOOLS));
    tb->setMovable(false);
    tb->addAction(saveCombinationAction);
    tb->addAction(exportCombinationAction);
    tb->addSeparator();
    tb->addAction(pickCollectionAction);
    tb->addAction(pickVariablesAction);
    tb->addSeparator();
    tb->addAction(combineAction);
    tb->addAction(randomizeAction);
    tb->addAction(fillDownAction);
    tb->addSeparator();
    tb->addAction(andAction);
    tb->addAction(removeCoveredAction);
    tb->addAction(appendTableAction);
    tb->addSeparator();
    tb->addAction(moveColumnLeftAction);
    tb->addAction(moveColumnRightAction);
    tb->addSeparator();
    tb->addAction(shiftUpAction);
    tb->addAction(shiftDownAction);
    tb->addSeparator();
    tb->addAction(deleteSelectedColumnsAction);
    tb->addSeparator();
    tb->addAction(clearHighlightsAction);
    tb->addSeparator();
    tb->addWidget(andWithLabel);

}
//----------------------------------------------------------------------------------------------------

int combinerWindow::getRandom(int upper)
{
    return qrand() % (upper + 1);

}
//----------------------------------------------------------------------------------------------------

SelectionData combinerWindow::getSelectionData()
{
    SelectionData selData;
    QList<int> selIdxs = table->getSelectedIndexes(dataTable::selectionColumn);
    table->cacheTable();

    if (selIdxs.size() > 0){
        selData.RowsMultiplied = 1;

        for (int i = 0; i < selIdxs.size(); i++){
            selData.selectedIndexes.append( selIdxs.at(i) );
            selData.selectedColumns.append( table->columnsCache.at( selIdxs.at(i) ));
            selData.selectionHeaders.append( table->columnHeaders.at( selIdxs.at(i) ));
            selData.RowsMultiplied = selData.RowsMultiplied * table->columnsCache.at( selIdxs.at(i) ).size();

        }
    }

    return selData;
}
//----------------------------------------------------------------------------------------------------

void combinerWindow::saveCombination()
{
    if(table->columnHeaders.size() > 0){
        QVariant dataBuff;
        QList <QVariant> tableData;
        table->cacheTable();
        tableData.append(table->columnHeaders);

        for (int i = 0; i < table->columnsCache.size(); i++){
            dataBuff.setValue(table->columnsCache.at(i));
            tableData.append(dataBuff);

        }

        activeTestItem->setData(2,Qt::UserRole, tableData);
        statusBar()->showMessage(STRING_DESIGN_STORED, 3000);
        hasChanges = false;
        emit saved();
    }
}
//----------------------------------------------------------------------------------------------------

void combinerWindow::showHelp()
{
    QMessageBox::about(this,tr(STRING_COMBINER), tr(STRING_COMBINER_HELP));

}
//----------------------------------------------------------------------------------------------------

void combinerWindow::createActions()
{
    icons = new iconsCatalog;

    andAction= new QAction(icons->andIcon, tr("And with selected"),this);
    connect(andAction,SIGNAL(triggered()),this,SLOT(andWithSelected()));

    saveCombinationAction = new QAction(icons->saveIcon,  tr("Save combination"),this);
    connect(saveCombinationAction,SIGNAL(triggered()),this,SLOT(saveCombination()));

    exportCombinationAction = new QAction(icons->htmlIcon,  tr("Export html"),this);
    connect(exportCombinationAction,SIGNAL(triggered()),this,SLOT(exportCombination()));

    pickVariablesAction = new QAction(icons->pickIcon, tr("Pick selected variable"),this);
    connect(pickVariablesAction,SIGNAL(triggered()),this,SLOT(pickFromTree()));

    pickCollectionAction = new QAction(icons->pickCollectionIcon, tr("Pick selected collection"),this);
    connect(pickCollectionAction,SIGNAL(triggered()),this,SLOT(pickCollection()));

    combineAction = new QAction(icons->combineIcon,  tr("Multiply selection"),this);
    connect(combineAction,SIGNAL(triggered()),this,SLOT(combineSelected()));

    randomizeAction = new QAction(icons->randomIcon,  tr("Randomize selection"),this);
    connect(randomizeAction,SIGNAL(triggered()),this,SLOT(randomizeSelected()));

    showHelpAction = new QAction( tr("Help"),this);
    connect(showHelpAction,SIGNAL(triggered()),this,SLOT(showHelp()));

    deleteSelectedColumnsAction= new QAction(icons->deleteIcon,  tr("Delete column"),this);
    connect(deleteSelectedColumnsAction,SIGNAL(triggered()),this,SLOT(deleteSelectedColumns()));

    clearTableAction = new QAction(QIcon(":/icons/clearIcon.png"), tr("Clear"),this);
    connect(clearTableAction,SIGNAL(triggered()),this,SLOT(clearTable()));

    fillDownAction = new QAction(icons->fillIcon,  tr("Fill values down"),this);
    connect(fillDownAction,SIGNAL(triggered()),this,SLOT(fillDownSelected()));

    moveColumnLeftAction = new QAction(icons->leftIcon,  tr("Move left"),this);
    connect(moveColumnLeftAction,SIGNAL(triggered()),this,SLOT(moveColumnLeft()));

    moveColumnRightAction = new QAction(icons->rightIcon,  tr("Move right"),this);
    connect(moveColumnRightAction,SIGNAL(triggered()),this,SLOT(moveColumnRight()));

    shiftUpAction = new QAction(icons->upIcon,  tr("Shift up"),this);
    connect(shiftUpAction,SIGNAL(triggered()),this,SLOT(shiftSelectedUp()));

    shiftDownAction = new QAction(icons->downIcon,  tr("Shift down"),this);
    connect(shiftDownAction,SIGNAL(triggered()),this,SLOT(shiftSelectedDown()));

    clearHighlightsAction = new QAction(icons->clearHighlightsIcon, QObject::tr("Clear highlights"), this);
    clearHighlightsAction->setVisible(false);
    connect(clearHighlightsAction, SIGNAL(triggered()), this, SLOT(clearHighlights()));

    removeCoveredAction = new QAction(QIcon(":/icons/nandIcon.png"), tr("Remove covered"),this);
    connect(removeCoveredAction,SIGNAL(triggered()),this,SLOT(removeCovered()));

    appendTableAction = new QAction(QIcon(":/icons/orIcon.png"), tr("Append table"),this);
    connect(appendTableAction,SIGNAL(triggered()),this,SLOT(appendTable()));
}
//----------------------------------------------------------------------------------------------------

void combinerWindow::createMenus()
{
    QMenuBar *mb = this->menuBar();

    QMenu *saveMenu = mb->addMenu(tr("Design"));
        saveMenu->addAction(saveCombinationAction);
        saveMenu->addAction(exportCombinationAction);
        saveMenu->addAction(clearTableAction);

    QMenu *variablesMenu = mb->addMenu(tr("Variables"));
        variablesMenu->addAction(pickCollectionAction);
        variablesMenu->addAction(pickVariablesAction);
        variablesMenu->addSeparator();
        variablesMenu->addAction(combineAction);
        variablesMenu->addAction(fillDownAction);
        variablesMenu->addAction(randomizeAction);
        variablesMenu->addAction(andAction);
        variablesMenu->addAction(removeCoveredAction);
        variablesMenu->addSeparator();
        variablesMenu->addAction(appendTableAction);

    QMenu *columnsMenu = mb->addMenu(tr("Columns"));
        columnsMenu->addAction(deleteSelectedColumnsAction);
        columnsMenu->addAction(moveColumnLeftAction);
        columnsMenu->addAction(moveColumnRightAction);

    QMenu *helpMenu = mb->addMenu(tr("Help"));
        helpMenu->addAction(showHelpAction);

}
//----------------------------------------------------------------------------------------------------

void combinerWindow::pickFromTree()
{
    table->loadFromItem(combinerWindow::clickedItem);
    hasChanges = true;

}
//----------------------------------------------------------------------------------------------------

combinerWindow::~combinerWindow()
{

}
