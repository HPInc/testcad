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

#include "collector/collectorWindow.h"

collectorWindow::collectorWindow(QWidget *parent, QTreeWidgetItem *testItem) : QMainWindow(parent)
{
    table = new dataTable();
    table->setSelectionBehavior(QAbstractItemView::SelectColumns);
    this->setCentralWidget(table);
    this->setWindowTitle(testItem->text(0));
    this->setWindowIcon(QIcon(":/icons/collectionIcon.png"));
    this->resize(800,600);
    this->setMinimumSize(800,600);
    this->setAttribute(Qt::WA_DeleteOnClose);
    activeTestItem = testItem;
    createActions();
    createMenus();
    createToolBar();
    loadStoredData();
    statusBar()->show();
    hasChanges = false;
    selectedColumn = -1;
    collectorWindow::instance = 1;

}
//----------------------------------------------------------------------------------------------------

int collectorWindow::instance = 0;
QTreeWidgetItem* collectorWindow::clickedItem = 0;

//----------------------------------------------------------------------------------------------------

void collectorWindow::addColumn()
{
    bool ok;
    QString dialogCaption = tr("Adding a column...");
    QString headerText = QInputDialog::getText(this, dialogCaption,tr("Header text"),QLineEdit::Normal,"",&ok);

    if(ok){

        if (headerText.isEmpty()){
            QMessageBox msg(QMessageBox::Critical, dialogCaption,
                            tr("You must provide a text for the header"),QMessageBox::Ok);
            msg.exec();
            statusBar()->showMessage(tr("Nothing added..."),3000);

        }else{
            writeTableToCache();
            columnHeaders.append(headerText);
            QStringList newColBuff;
            newColBuff.append("");
            columnsCache.append(newColBuff);
            updateTable();
        }
    }
}
//----------------------------------------------------------------------------------------------------

void collectorWindow::addRow()
{
    if (!columnsCache.isEmpty()){
        QStringList cbff;
        writeTableToCache();

        for(int i = 0; i < columnsCache.size(); i++){
            cbff = columnsCache.at(i);
            cbff.append("");
            columnsCache.replace(i, cbff);

        }

        updateTable();
    }

}
//----------------------------------------------------------------------------------------------------

void collectorWindow::clearHighlights()
{
    table->clearTableBackground();
    clearHighlightsAction->setVisible(false);
}
//----------------------------------------------------------------------------------------------------

QList<QStringList> collectorWindow::clickedItemColumns()
{
    QList<QStringList> storedTable;

    if (collectorWindow::clickedItem!=0){
        QList <QVariant> designData = collectorWindow::clickedItem->data(2,Qt::UserRole).toList();

        if (designData.size() > 0){

            for (int i = 1; i < designData.size(); i++){
                storedTable.append(designData.at(i).toStringList());
            }
        }
    }

    return storedTable;
}
//----------------------------------------------------------------------------------------------------

void collectorWindow::shiftSelectedUp()
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

void collectorWindow::shiftSelectedDown()
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

void collectorWindow::moveColumnLeft()
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

void collectorWindow::moveColumnRight()
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

void collectorWindow::updateTable()
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

void collectorWindow::writeTableToCache()
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

void collectorWindow::writeHtmlStyle()
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

void collectorWindow::writeHtmlH1(int collectionsCount)
{
    xmlWriter->writeStartElement("h1");
    xmlWriter->writeCharacters(activeTestItem->text(0) + ": " + QString::number(collectionsCount) + " " + QString(tr("Collections")));
    xmlWriter->writeEndElement();
}
//----------------------------------------------------------------------------------------------------

void collectorWindow::writeColumnHeaders()
{
    xmlWriter->writeStartElement("tr");
    xmlWriter->writeTextElement("th","#");

    for (int i = 0; i < columnHeaders.size(); i++){
        xmlWriter->writeTextElement("th",columnHeaders.at(i));

    }

    xmlWriter->writeEndElement();
}
//----------------------------------------------------------------------------------------------------

void collectorWindow::writeDataRows()
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

bool collectorWindow::writeHtmlTo(QString filePath)
{
    if (filePath.isEmpty())
        return false;

    QFile file(filePath);

    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this,STRING_TESTCAD,
                             tr("Cannot write to file")
                             .arg(filePath)
                             .arg(file.errorString()));

        return false;

    }else{
        xmlWriter = new QXmlStreamWriter(&file);
        xmlWriter->setAutoFormatting(true);
        xmlWriter->writeStartElement("html");
        xmlWriter->writeStartElement("body");
        writeHtmlStyle();
        writeHtmlH1(table->columnCount());
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

void collectorWindow::closeEvent(QCloseEvent *event)
{
    bool doClose = false;

    if (hasChanges) {
        QMessageBox msg(QMessageBox::Warning, tr("Collector"), tr("Collection has unsaved changes"), QMessageBox::Save | QMessageBox::Ignore | QMessageBox::Cancel);
        int decission = msg.exec();

        if (decission == QMessageBox::Save ){
            storeCollection();
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
        collectorWindow::instance = 0;
        event->accept();

    }

}
//----------------------------------------------------------------------------------------------------

void collectorWindow::exportCollection()
{
    QString filePath = QFileDialog::getSaveFileName(this, QObject::tr("Save file"),
                                         QDir::currentPath(),
                                         QObject::tr(STRING_FILETYPE_HTML));

    writeTableToCache();

    if (!filePath.endsWith(".html"))
        filePath = filePath + ".html";

    if(writeHtmlTo(filePath)){
        statusBar()->showMessage(tr("File saved"),3000);

    }else{
        QMessageBox msg(QMessageBox::Warning, tr("Collector"), tr("File not saved"), QMessageBox::Ok);
        msg.resize(200,100);
        msg.exec();
        statusBar()->showMessage(tr("File not saved"),3000);

    }

}
//----------------------------------------------------------------------------------------------------

void collectorWindow::createToolBar()
{

    QToolBar *tb = this->addToolBar(tr("Tools"));
    tb->setMovable(false);
    tb->addAction(saveCollectionAction);
    tb->addAction(exportCollectionAction);
    tb->addSeparator();
    tb->addAction(pickVariablesAction);
    tb->addAction(addColumnAction);
    tb->addAction(addRowAction);
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

}
//----------------------------------------------------------------------------------------------------

void collectorWindow::deleteSelectedColumns()
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

QList<int> collectorWindow::getSelectedIndexes()
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

void collectorWindow::loadStoredData()
{
    QStringList collectionBuff;
    QList <QVariant> designData = activeTestItem->data(2,Qt::UserRole).toList();

    if (designData.size() > 0){
        QVariantList headersStored = designData.at(0).toList();

        for (int i = 1; i < designData.size(); i++){
            columnHeaders.append(headersStored.at(i - 1).toString());
            QVariantList storedColumnBuff = designData.at(i).toList();

            for (int j = 0; j < storedColumnBuff.size(); j++){
                collectionBuff.append(storedColumnBuff.at( j ).toString());

            }

            columnsCache.append(collectionBuff);
            collectionBuff.clear();

        }

        updateTable();

    }

}
//----------------------------------------------------------------------------------------------------

void collectorWindow::pickFromTree()
{
    QTreeWidgetItem *item;
    QList<QTreeWidgetItem*> varBuffer;
    QStringList columnBuffer;
    writeTableToCache();
    variables.clear();

    if (collectorWindow::clickedItem != 0){
        varBuffer = getVariableItemsFrom(collectorWindow::clickedItem);
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

void collectorWindow::storeCollection()
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
        statusBar()->showMessage(tr("Collection stored"), 3000);
        hasChanges = false;
        emit saved();
    }
}
//----------------------------------------------------------------------------------------------------

void collectorWindow::showHelp()
{
    QString  help = tr("The <b>Collection</b> designer window allows you to pick variables from clicked tree items.<br><br>"\
                    "Each item in the tree that has bottom items is chosen as a variable where each child item is a value for it.<br><br>"\
                    "Creating a collection is the right way to pick values for combination tables as it allows you to keep the source variables from where further combinations are derived.<br><br>"\
                    "Once you have created a variables collection you can pick it from the <b>Combination</b> designer window so that you create combinations between the collection variables");

    QMessageBox::about(this,tr("Collection"), help);

}
//----------------------------------------------------------------------------------------------------

void collectorWindow::clearTable()
{
    table->clear();
    variables.clear();
    columnHeaders.clear();
    columnsCache.clear();

}
//----------------------------------------------------------------------------------------------------

void collectorWindow::createActions()
{
    icons = new iconsCatalog;

    addColumnAction = new QAction(QIcon(":/icons/addColumnIcon.png"),tr("Add a column"),this);
    connect(addColumnAction,SIGNAL(triggered()),this,SLOT(addColumn()));

    addRowAction = new QAction(QIcon(":/icons/addRowIcon.png"),tr("Add a row"),this);
    connect(addRowAction,SIGNAL(triggered()),this,SLOT(addRow()));

    saveCollectionAction = new QAction(icons->saveIcon, tr("Store"),this);
    connect(saveCollectionAction,SIGNAL(triggered()),this,SLOT(storeCollection()));

    exportCollectionAction = new QAction(icons->htmlIcon, tr("Export"),this);
    connect(exportCollectionAction,SIGNAL(triggered()),this,SLOT(exportCollection()));

    pickVariablesAction = new QAction(icons->pickIcon, tr("Get variables"),this);
    connect(pickVariablesAction,SIGNAL(triggered()),this,SLOT(pickFromTree()));

    showHelpAction = new QAction(tr("Help"),this);
    connect(showHelpAction,SIGNAL(triggered()),this,SLOT(showHelp()));

    deleteSelectedColumnsAction= new QAction(icons->deleteIcon, tr("Delete selected columns"),this);
    connect(deleteSelectedColumnsAction,SIGNAL(triggered()),this,SLOT(deleteSelectedColumns()));

    clearTableAction = new QAction(QIcon(":/icons/clearIcon.png"),tr("Clear table"),this);
    connect(clearTableAction,SIGNAL(triggered()),this,SLOT(clearTable()));

    moveColumnLeftAction = new QAction(icons->leftIcon, tr("Move column to the left"),this);
    connect(moveColumnLeftAction,SIGNAL(triggered()),this,SLOT(moveColumnLeft()));

    moveColumnRightAction = new QAction(icons->rightIcon, tr("Move column to the right"),this);
    connect(moveColumnRightAction,SIGNAL(triggered()),this,SLOT(moveColumnRight()));

    shiftUpAction = new QAction(icons->upIcon, tr("Shift up"),this);
    connect(shiftUpAction,SIGNAL(triggered()),this,SLOT(shiftSelectedUp()));

    shiftDownAction = new QAction(icons->downIcon, tr("Shift down"),this);
    connect(shiftDownAction,SIGNAL(triggered()),this,SLOT(shiftSelectedDown()));

    clearHighlightsAction = new QAction(icons->clearHighlightsIcon, QObject::tr("Clear highlights"), this);
    clearHighlightsAction->setVisible(false);
    connect(clearHighlightsAction, SIGNAL(triggered()), this, SLOT(clearHighlights()));
}
//----------------------------------------------------------------------------------------------------

void collectorWindow::createMenus()
{
    QMenuBar *mb = this->menuBar();

    QMenu *saveMenu = mb->addMenu(tr("Collection"));
        saveMenu->addAction(saveCollectionAction);
        saveMenu->addAction(exportCollectionAction);
        saveMenu->addAction(clearTableAction);

    QMenu *variablesMenu = mb->addMenu(tr("Variables"));
        variablesMenu->addAction(pickVariablesAction);
        variablesMenu->addSeparator();

    QMenu *columnsMenu = mb->addMenu(tr("Columns"));
        columnsMenu->addAction(deleteSelectedColumnsAction);
        columnsMenu->addAction(moveColumnLeftAction);
        columnsMenu->addAction(moveColumnRightAction);

    QMenu *helpMenu = mb->addMenu(tr("Help"));
        helpMenu->addAction(showHelpAction);

}
//----------------------------------------------------------------------------------------------------

QList<QTreeWidgetItem*> collectorWindow::getVariableItemsFrom(QTreeWidgetItem *treeItem)
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

bool collectorWindow::isValidVariable(QTreeWidgetItem *treeItem)
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

collectorWindow::~collectorWindow()
{

}
