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
    selectedColumn = -1;

    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());
}
//----------------------------------------------------------------------------------------------------

int combinerWindow::instance = 0;
QTreeWidgetItem* combinerWindow::clickedItem = 0;

//----------------------------------------------------------------------------------------------------

void combinerWindow::removeCovered()
{
    QStringList colBuff;

    for(int i=0; i<andRows.size(); i++){

        if (andRows.at(andRows.size()-1-i) == true){

            for (int j = 0; j < columnsCache.size(); j++){
                colBuff = columnsCache.at(j);
                colBuff.removeAt(andRows.size()-1-i);
                columnsCache.replace(j,colBuff);

            }
        }
    }

    andRows.clear();
    updateTable();
}
//----------------------------------------------------------------------------------------------------

void combinerWindow::pickCollection()
{
    writeTableToCache();

    if ((combinerWindow::clickedItem != 0) &&
        (combinerWindow::clickedItem->data(0,Qt::UserRole).toString() == TAG_TYPE_TEST_COLLECTION)){
        CollectionData collBuffer = getCollectionFrom(combinerWindow::clickedItem);

        if (!collBuffer.variables.isEmpty()){
            table->clear();

            for (int i = 0; i < collBuffer.variables.size(); i++){
                columnHeaders.append(collBuffer.variables.at(i));
                columnsCache.append(collBuffer.valuesByVariable.at(i));

            }

            table->setColumnHeaders(columnHeaders);

            for (int i = 0; i < columnsCache.size(); i++){
                table->setColumnValues(i, columnsCache.at( i ));

            }

            table->update();
            hasChanges = true;
        }
    }

}
//----------------------------------------------------------------------------------------------------

CollectionData combinerWindow::getCollectionFrom(QTreeWidgetItem *treeItem){
    CollectionData collData;
    QStringList vars;
    QList<QStringList> vals;
    QStringList valBuff;
    QList <QVariant> designData = treeItem->data(2,Qt::UserRole).toList();

    if (designData.size() > 0){
        QVariantList headersStored = designData.at(0).toList();

        for (int i = 1; i < designData.size(); i++){
            vars.append(headersStored.at(i - 1).toString());
            QVariantList storedColumnBuff = designData.at(i).toList();

            for (int j = 0; j < storedColumnBuff.size(); j++){
                valBuff.append(storedColumnBuff.at( j ).toString());

            }

            vals.append(valBuff);
            valBuff.clear();

        }

        collData.variables = vars;
        collData.valuesByVariable = vals;

    }

    return collData;
}
//----------------------------------------------------------------------------------------------------

void combinerWindow::pickFromTree()
{
    QTreeWidgetItem *item;
    QList<QTreeWidgetItem*> varBuffer;
    QStringList columnBuffer;
    QList<QTreeWidgetItem*> variables;

    writeTableToCache();

    if (combinerWindow::clickedItem != 0){
        varBuffer = getVariableItemsFrom(combinerWindow::clickedItem);
        variables.append(varBuffer);
        table->clear();

        for (int i = 0; i < variables.size(); i++){
            item = variables.at(i);
            columnHeaders.append(item->text(0));

            for (int j=0; j < item->childCount(); j++){
                columnBuffer.append(item->child(j)->text(0));

            }

            columnsCache.append(columnBuffer);
            columnBuffer.clear();

        }

        for (int i = 0; i < columnsCache.size(); i++){
            table->setColumnValues(i, columnsCache.at( i ));

        }

        table->setColumnHeaders(columnHeaders);
        table->update();
        hasChanges = true;
    }

}
//----------------------------------------------------------------------------------------------------

QList<QTreeWidgetItem*> combinerWindow::getVariableItemsFrom(QTreeWidgetItem *treeItem)
{
    QList<QTreeWidgetItem*> lastParents;
    QList<QTreeWidgetItem*> lastPBuff;

    if (treeItem->childCount() > 1){

        for(int i = 0; i < treeItem->childCount(); i++){

            if(isValidVariable(treeItem)){
                lastParents.append(treeItem);
                break;

            }else{
                lastPBuff = getVariableItemsFrom(treeItem->child(i));

            }

            for (int n = 0; n < lastPBuff.count(); n++){
                lastParents.append(lastPBuff.at(n));

            }

        }

    }

    return lastParents;
}
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

void combinerWindow::shiftSelectedUp()
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
        selectedColumn = selIdxs.at(i);

    }

    updateTable();
    hasChanges = true;
}
//----------------------------------------------------------------------------------------------------

void combinerWindow::shiftSelectedDown()
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
        columnsCache.replace(selIdxs.at(i), cBuff);
        selectedColumn = selIdxs.at(i);

    }

    updateTable();
    hasChanges = true;
}
//----------------------------------------------------------------------------------------------------

void combinerWindow::moveColumnLeft()
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

void combinerWindow::moveColumnRight()
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

void combinerWindow::updateTable()
{
    table->clear();
    table->setColumnHeaders(columnHeaders);
    table->setColumnValues(columnsCache);
    table->update();

    if (selectedColumn > -1){
        table->selectColumn(selectedColumn);
        selectedColumn = -1;

    }
}
//----------------------------------------------------------------------------------------------------

void combinerWindow::writeTableToCache()
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

    for (int i = 0; i < columnHeaders.size(); i++){
        xmlWriter->writeTextElement("th",columnHeaders.at(i));

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
            storeCombination();
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

    writeTableToCache();

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

void combinerWindow::randomizeSelected()
{
    SelectionData selDat = getSelectionData();
    QStringList vals;
    QStringList rndBuff;
    QString strBuff;
    int rndFrom;
    int rndTo;
    int rowCount = table->rowCount();

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

        columnsCache.replace(selDat.selectedIndexes.at(i),rndBuff);
        rndBuff.clear();
    }

    updateTable();
    hasChanges = true;
}
//----------------------------------------------------------------------------------------------------

int combinerWindow::getRandom(int upper)
{
    return qrand() % (upper + 1);

}
//----------------------------------------------------------------------------------------------------

void combinerWindow::fillDownSelected()
{
    SelectionData selDat = getSelectionData();
    QStringList vals;
    int n;

    for (int i = 0; i < selDat.selectedColumns.size(); i++){
        n = 0;
        vals = selDat.selectedColumns.at(i);

        for (int j = vals.size() - 1; j < table->rowCount() - 1 ; j++){
            vals.append(vals.at(n));
            n++;

        }

        columnsCache.replace(selDat.selectedIndexes.at(i),vals);
        vals.clear();
    }

    updateTable();
    hasChanges = true;
}
//----------------------------------------------------------------------------------------------------

void combinerWindow::combineSelected()
{
    QList<QStringList> combinedColumns;
    QStringList combinedCol;
    QStringList vals;
    SelectionData selDat = getSelectionData();
    int rowsDivider = 1;
    int rows;

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

    for(int i = 0; i < columnsCache.size(); i++){

        if(!selDat.selectedIndexes.contains(i)){
            combinedColumns.append( columnsCache.at(i) );
            selDat.selectionHeaders.append( columnHeaders.at(i) );

        }
    }

    columnHeaders = selDat.selectionHeaders;
    columnsCache = combinedColumns;
    updateTable();
    hasChanges = true;

}
//----------------------------------------------------------------------------------------------------

void combinerWindow::deleteSelectedColumns()
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

QList<int> combinerWindow::getSelectedIndexes()
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

SelectionData combinerWindow::getSelectionData()
{
    SelectionData selData;
    QList<int> selIdxs = getSelectedIndexes();
    writeTableToCache();

    if (selIdxs.size() > 0){
        selData.RowsMultiplied = 1;

        for (int i = 0; i < selIdxs.size(); i++){
            selData.selectedIndexes.append( selIdxs.at(i) );
            selData.selectedColumns.append( columnsCache.at( selIdxs.at(i) ));
            selData.selectionHeaders.append( columnHeaders.at( selIdxs.at(i) ));
            selData.RowsMultiplied = selData.RowsMultiplied * columnsCache.at( selIdxs.at(i) ).size();

        }
    }

    return selData;
}
//----------------------------------------------------------------------------------------------------

void combinerWindow::loadStoredData()
{
    QStringList combinationBuff;
    QList <QVariant> designData = activeTestItem->data(2,Qt::UserRole).toList();

    if (designData.size() > 0){
        QVariantList headersStored = designData.at(0).toList();

        for (int i = 1; i < designData.size(); i++){
            columnHeaders.append(headersStored.at(i - 1).toString());
            QVariantList storedColumnBuff = designData.at(i).toList();

            for (int j = 0; j < storedColumnBuff.size(); j++){
                combinationBuff.append(storedColumnBuff.at( j ).toString());

            }

            columnsCache.append(combinationBuff);
            combinationBuff.clear();

        }

        updateTable();

    }

}
//----------------------------------------------------------------------------------------------------

void combinerWindow::storeCombination()
{
    if(columnHeaders.size() > 0){
        QVariant dataBuff;
        QList <QVariant> tableData;
        writeTableToCache();
        tableData.append(columnHeaders);

        for (int i = 0; i < columnsCache.size(); i++){
            dataBuff.setValue(columnsCache.at(i));
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

void combinerWindow::clearTable()
{
    table->clear();
    columnHeaders.clear();
    columnsCache.clear();

}
//----------------------------------------------------------------------------------------------------

void combinerWindow::createActions()
{
    icons = new iconsCatalog;

    andAction= new QAction(icons->andIcon, tr("And with selected"),this);
    connect(andAction,SIGNAL(triggered()),this,SLOT(andWithSelected()));

    saveCombinationAction = new QAction(icons->saveIcon, STRING_ACTION_STORE,this);
    connect(saveCombinationAction,SIGNAL(triggered()),this,SLOT(storeCombination()));

    exportCombinationAction = new QAction(icons->htmlIcon, STRING_ACTION_EXPORT,this);
    connect(exportCombinationAction,SIGNAL(triggered()),this,SLOT(exportCombination()));

    pickVariablesAction = new QAction(icons->pickIcon, tr("Pick selected variable"),this);
    connect(pickVariablesAction,SIGNAL(triggered()),this,SLOT(pickFromTree()));

    pickCollectionAction = new QAction(icons->pickCollectionIcon, tr("Pick selected collection"),this);
    connect(pickCollectionAction,SIGNAL(triggered()),this,SLOT(pickCollection()));

    combineAction = new QAction(icons->combineIcon, STRING_ACTION_COMBINE_SELECTION,this);
    connect(combineAction,SIGNAL(triggered()),this,SLOT(combineSelected()));

    randomizeAction = new QAction(icons->randomIcon, STRING_ACTION_RANDOMIZE_SELECTION,this);
    connect(randomizeAction,SIGNAL(triggered()),this,SLOT(randomizeSelected()));

    showHelpAction = new QAction(STRING_MENU_HELP,this);
    connect(showHelpAction,SIGNAL(triggered()),this,SLOT(showHelp()));

    deleteSelectedColumnsAction= new QAction(icons->deleteIcon, STRING_ACTION_DELETE_SELECTED_COLUMNS,this);
    connect(deleteSelectedColumnsAction,SIGNAL(triggered()),this,SLOT(deleteSelectedColumns()));

    clearTableAction = new QAction(QIcon(":/icons/clearIcon.png"),STRING_ACTION_CLEAR_TABLE,this);
    connect(clearTableAction,SIGNAL(triggered()),this,SLOT(clearTable()));

    fillDownAction = new QAction(icons->fillIcon, STRING_ACTION_FILLDOWN_SELECTED_COLUMNS,this);
    connect(fillDownAction,SIGNAL(triggered()),this,SLOT(fillDownSelected()));

    moveColumnLeftAction = new QAction(icons->leftIcon, STRING_ACTION_MOVE_LEFT,this);
    connect(moveColumnLeftAction,SIGNAL(triggered()),this,SLOT(moveColumnLeft()));

    moveColumnRightAction = new QAction(icons->rightIcon, STRING_ACTION_MOVE_RIGHT,this);
    connect(moveColumnRightAction,SIGNAL(triggered()),this,SLOT(moveColumnRight()));

    shiftUpAction = new QAction(icons->upIcon, STRING_ACTION_SHIFT_UP,this);
    connect(shiftUpAction,SIGNAL(triggered()),this,SLOT(shiftSelectedUp()));

    shiftDownAction = new QAction(icons->downIcon, STRING_ACTION_SHIFT_DOWN,this);
    connect(shiftDownAction,SIGNAL(triggered()),this,SLOT(shiftSelectedDown()));

    clearHighlightsAction = new QAction(icons->clearHighlightsIcon, QObject::tr(STRING_ACTION_CLEAR_HIGHLIGHTS), this);
    clearHighlightsAction->setVisible(false);
    connect(clearHighlightsAction, SIGNAL(triggered()), this, SLOT(clearHighlights()));

    removeCoveredAction = new QAction(QIcon(":/icons/nandIcon.png"), tr("Remove covered"),this);
    connect(removeCoveredAction,SIGNAL(triggered()),this,SLOT(removeCovered()));
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

    QMenu *columnsMenu = mb->addMenu(tr("Columns"));
        columnsMenu->addAction(deleteSelectedColumnsAction);
        columnsMenu->addAction(moveColumnLeftAction);
        columnsMenu->addAction(moveColumnRightAction);

    QMenu *helpMenu = mb->addMenu(tr("Help"));
        helpMenu->addAction(showHelpAction);

}
//----------------------------------------------------------------------------------------------------

bool combinerWindow::isValidVariable(QTreeWidgetItem *treeItem)
{
    bool isValid = true;

    if(treeItem->childCount()>1){

        for(int i = 0; i < treeItem->childCount(); i++){

            if (treeItem->child(i)->childCount() > 0){
                isValid = false;
                break;

            }

        }

    }else{
       isValid = false;

    }

    return isValid;
}
//----------------------------------------------------------------------------------------------------

combinerWindow::~combinerWindow()
{

}
