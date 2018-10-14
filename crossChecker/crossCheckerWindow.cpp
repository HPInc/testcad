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
    table->skipEmptyCells = false;
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
    createToolBar();
    loadStoredData();
    statusBar()->show();
    hasChanges = false;

}
//----------------------------------------------------------------------------------------------------

void crossCheckerWindow::addColumn()
{
    table->addColumn();
    hasChanges = true;
}
//----------------------------------------------------------------------------------------------------

void crossCheckerWindow::pickVariables()
{
    statsWindow *statsTable = new statsWindow(0,0);
    variableStatistics vs = statsTable->getVariableStatistics(clickedItem);

    delete statsTable;

    setHeaderFor(crossCheckerWindow::rows, vs.variables);

}
//----------------------------------------------------------------------------------------------------

void crossCheckerWindow::setHeaderFor(int tablePart, QStringList headersList)
{
    if (headersList.size() > 0){
        table->cacheTable();

        if(tablePart == crossCheckerWindow::rows){
            table->rowHeaders = headersList;
            loadHeaderParent(tablePart, crossCheckerWindow::clickedItem->text(0));

        }else if(tablePart == crossCheckerWindow::columns){
            table->columnHeaders = headersList;
            loadHeaderParent(tablePart, crossCheckerWindow::clickedItem->text(0));

        }

        resetCache();
        table->loadFromCache();
        hasChanges = true;
    }
}
//----------------------------------------------------------------------------------------------------

void crossCheckerWindow::pickColumnHeadersFromTree()
{
    setHeaderFor(crossCheckerWindow::columns, getSelectionChildren());
}
//----------------------------------------------------------------------------------------------------

void crossCheckerWindow::pickRowHeadersFromTree()
{
    setHeaderFor(crossCheckerWindow::rows, getSelectionChildren());
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

QString crossCheckerWindow::getTagFromUser()
{
    QStringList tags;
    tags << tr("Current Date") << "X" << tr("Custom value");

    bool ok;
    QString tag = QInputDialog::getItem(this, tr("Cover the table using..."),
                                            tr("Tag:"), tags, 0, false, &ok);
    if (ok && !tag.isEmpty()){

        if(tag == tr("Current Date")){
            tag = QDate::currentDate().toString("yyyy/MM/dd");

        }else if(tag == "X"){
            tag = "X";

        }else{
            tag = QInputDialog::getText(this, tr("Enter custom value"),
                                        tr("Value:"), QLineEdit::Normal,
                                        "X", &ok);
            if (ok && !tag.isEmpty()){
                return tag;

            }else{
                return "";

            }
        }

        return tag;

    }else{
        return "";

    }
}
//----------------------------------------------------------------------------------------------------

void crossCheckerWindow::applyCoverageTo(QList<QStringList> *columnsList)
{
    table->cacheTable();

    if(columnsList->size() * table->rowHeaders.size() > 0){
        bool ok;
        QString tag = getTagFromUser();

        if (!tag.isEmpty()){
            int cov = QInputDialog::getInt(this,tr("Cover empty cells"),tr("Coverage percent"),33,1,100,1,&ok);

            if(ok){
                int nCells = columnsList->size() * table->rowHeaders.size();
                int toCover = qRound(nCells * static_cast<double>(cov/100.0));
                int byCol = qRound(static_cast<double>(toCover / columnsList->size()));
                if (byCol == 0) byCol = 1;
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
        }

    }else{
        QMessageBox msg(QMessageBox::Information, tr("Cross checker."), tr("Design is empty..."), QMessageBox::Ok);
        msg.resize(200,100);
        msg.exec();
        statusBar()->showMessage(tr("Selection is empty..."), 2000);

    }
}
//----------------------------------------------------------------------------------------------------

void crossCheckerWindow::saveChecker()
{
    QList <QVariant> tableData;
    table->cacheTable();

    if((headerParents.size()>0) && (table->rowHeaders.size()>0) && (table->columnHeaders.size()>0))
    {
        tableData.append(headerParents);
        tableData.append(table->rowHeaders);
        tableData.append(table->columnHeaders);

        QVariant dataBuff;

        for (int i = 0; i < table->columnsCache.size(); i++){
            dataBuff.setValue(table->columnsCache.at(i));
            tableData.append(dataBuff);

        }
    }

    if(tableData.size() > 0){
        activeTestItem->setData(2,Qt::UserRole, tableData);
        statusBar()->showMessage(tr("Data saved."), 3000);
        hasChanges = false;
        emit saved();

    }else{
        statusBar()->showMessage(tr("Nothing saved."), 3000);

    }
}
//----------------------------------------------------------------------------------------------------

void crossCheckerWindow::resetCache()
{
    table->columnsCache.clear();
    QStringList vals;

    for(int n = 0; n < table->rowHeaders.size(); n++){
        vals.append("");
    }

    for(int n = 0; n < table->columnHeaders.size(); n++){
        table->columnsCache.append(vals);
    }
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

        table->rowHeaders = designData.at(1).toStringList();
        table->columnHeaders = designData.at(2).toStringList();

        for (int i = 3; i < designData.size(); i++){
            table->columnsCache.append(designData.at(i).toStringList());

        }

        table->loadFromCache();
    }

}
//----------------------------------------------------------------------------------------------------

int crossCheckerWindow::instance = 0;
QTreeWidgetItem* crossCheckerWindow::clickedItem;

//----------------------------------------------------------------------------------------------------

void crossCheckerWindow::moveColumnLeft()
{
    table->moveColumnLeft();
    hasChanges = true;
}
//----------------------------------------------------------------------------------------------------

void crossCheckerWindow::moveColumnRight()
{
    table->moveColumnRight();
    hasChanges = true;
}
//----------------------------------------------------------------------------------------------------

void crossCheckerWindow::coverSelected()
{
    table->cacheTable();
    QList<int> selIdxs = table->getSelectedIndexes(dataTable::selectionColumn);
    QList<QStringList> colsBuff;

    for (int i = 0; i < selIdxs.size(); i++){
        colsBuff.append(table->columnsCache.at(selIdxs.at(i)));

    }

    applyCoverageTo(&colsBuff);

    for (int i = 0; i < selIdxs.size(); i++){
        table->columnsCache.replace(selIdxs.at(i),colsBuff.at(i));

    }

    table->loadFromCache();
    hasChanges = true;
}
//----------------------------------------------------------------------------------------------------

void crossCheckerWindow::coverAll()
{
    table->cacheTable();
    applyCoverageTo(&table->columnsCache);
    table->loadFromCache();
    hasChanges = true;

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
            xmlWriter->writeAttribute("colspan",QString::number(table->columnHeaders.count() + 2));
            xmlWriter->writeCharacters(headerParents.at(1));
        xmlWriter->writeEndElement();
    xmlWriter->writeEndElement();

    xmlWriter->writeStartElement("tr");
    xmlWriter->writeTextElement("th","");

    for (int i = 0; i < table->columnHeaders.size(); i++){
        xmlWriter->writeTextElement("th",table->columnHeaders.at(i));

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
                xmlWriter->writeCharacters(table->rowHeaders.at(i));
            xmlWriter->writeEndElement();

        for (int j = 0; j < table->columnsCache.size(); j++){
            colBuff = table->columnsCache.at(j);

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

void crossCheckerWindow::closeEvent(QCloseEvent *event)
{
    bool doClose = false;

    if (hasChanges) {

        QMessageBox msg(QMessageBox::Warning, STRING_CROSS_CHECKER, STRING_DESIGN_HAS_UNSAVED_CHANGES, QMessageBox::Save | QMessageBox::Ignore | QMessageBox::Cancel);
        int decission = msg.exec();

        if (decission == QMessageBox::Save ){
            saveChecker();
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

    table->cacheTable();

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
    tb->addSeparator();
    tb->addAction(pickColumnHeadersAction);
    tb->addAction(pickRowHeadersAction);
    tb->addAction(pickVariablesAction);
    tb->addAction(addColumnAction);
    tb->addSeparator();
    tb->addAction(coverAllAction);
    tb->addAction(coverSelectedAction);
    tb->addAction(moveColumnLeftAction);
    tb->addAction(moveColumnRightAction);
    tb->addAction(deleteSelectedColumnsAction);
    tb->addSeparator();
    tb->addWidget(rowsParentLabel);
    tb->addSeparator();
    tb->addWidget(columnsParentLabel);

}
//----------------------------------------------------------------------------------------------------

void crossCheckerWindow::deleteSelectedColumns()
{
    table->deleteSelectedColumns();
    hasChanges = true;

}
//----------------------------------------------------------------------------------------------------

void crossCheckerWindow::showHelp()
{
    QMessageBox::about(this,tr("Cross checker"), tr("The <b>cross checker</b> window allows you to cross relate values."\
                                                    "You can pick variables from clicked items in the tree and arrange them "\
                                                    "as headers for rows and column headers acting as coordinates for the values you "\
                                                    "enter in each cell."\
                                                    "You can also just pick variables and add columns manually to cross relate with "\
                                                    "values you choose for each column. This is useful for creating <b>Traceability matrixes</b>."\
                                                    "<br><br><b>Example:</b>"\
                                                    "<br><br>  1. Click a item in the tree."\
                                                     "<br>  2. Click Variables and select <b>Pick variables</b> option."\
                                                     "<br>  3. Click Add Column icon to add a column and give it a name."\
                                                     "<br>  4. Repeat the step 3 for as many columns you need to add."));

}
//----------------------------------------------------------------------------------------------------

void crossCheckerWindow::clearTable()
{
    table->clear();
    hasChanges = true;
}
//----------------------------------------------------------------------------------------------------

void crossCheckerWindow::createActions()
{
    icons = new iconsCatalog;

    saveCheckerAction = new QAction(icons->saveIcon, tr("Save"),this);
    connect(saveCheckerAction,SIGNAL(triggered()),this,SLOT(saveChecker()));

    exportCheckerAction = new QAction(icons->htmlIcon, tr("Export html"),this);
    connect(exportCheckerAction,SIGNAL(triggered()),this,SLOT(exportChecker()));

    pickColumnHeadersAction = new QAction(icons->pickColumnHeadersIcon, tr("Get column headers"),this);
    connect(pickColumnHeadersAction,SIGNAL(triggered()),this,SLOT(pickColumnHeadersFromTree()));

    pickRowHeadersAction = new QAction(icons->pickRowHeadersIcon, tr("Get row headers"),this);
    connect(pickRowHeadersAction,SIGNAL(triggered()),this,SLOT(pickRowHeadersFromTree()));

    showHelpAction = new QAction(tr("Help"),this);
    connect(showHelpAction,SIGNAL(triggered()),this,SLOT(showHelp()));

    deleteSelectedColumnsAction= new QAction(icons->deleteIcon, tr("Delete selected columns"),this);
    connect(deleteSelectedColumnsAction,SIGNAL(triggered()),this,SLOT(deleteSelectedColumns()));

    clearTableAction = new QAction(QIcon(":/icons/clearIcon.png"),tr("Clear table"),this);
    connect(clearTableAction,SIGNAL(triggered()),this,SLOT(clearTable()));

    coverSelectedAction = new QAction(icons->fillIcon, tr("Cover selected"),this);
    connect(coverSelectedAction,SIGNAL(triggered()),this,SLOT(coverSelected()));

    moveColumnLeftAction = new QAction(icons->leftIcon, tr("Move left"),this);
    connect(moveColumnLeftAction,SIGNAL(triggered()),this,SLOT(moveColumnLeft()));

    moveColumnRightAction = new QAction(icons->rightIcon, tr("Move right"),this);
    connect(moveColumnRightAction,SIGNAL(triggered()),this,SLOT(moveColumnRight()));

    coverAllAction = new QAction(icons->coverIcon, tr("Cover all"),this);
    connect(coverAllAction,SIGNAL(triggered()),this,SLOT(coverAll()));

    pickVariablesAction =  new QAction(QIcon(":/icons/pickVariablesIcon.png"), tr("Pick variables"),this);
    connect(pickVariablesAction,SIGNAL(triggered()),this,SLOT(pickVariables()));

    addColumnAction = new QAction(QIcon(":/icons/addColumnIcon.png"),tr("Add a column"),this);
    connect(addColumnAction,SIGNAL(triggered()),this,SLOT(addColumn()));

}
//----------------------------------------------------------------------------------------------------

void crossCheckerWindow::createMenus()
{
    QMenuBar *mb = this->menuBar();

    QMenu *saveMenu = mb->addMenu(tr("Table"));
        saveMenu->addAction(saveCheckerAction);
        saveMenu->addAction(exportCheckerAction);
        saveMenu->addAction(clearTableAction);

    QMenu *variablesMenu = mb->addMenu(tr("Variables"));
        variablesMenu->addAction(pickRowHeadersAction);
        variablesMenu->addAction(pickColumnHeadersAction);
        variablesMenu->addSeparator();
        variablesMenu->addAction(pickVariablesAction);
        variablesMenu->addSeparator();
        variablesMenu->addAction(coverAllAction);
        variablesMenu->addAction(coverSelectedAction);

    QMenu *columnsMenu = mb->addMenu(tr("Columns"));
        columnsMenu->addAction(addColumnAction);
        columnsMenu->addAction(deleteSelectedColumnsAction);
        columnsMenu->addAction(moveColumnLeftAction);
        columnsMenu->addAction(moveColumnRightAction);

    QMenu *helpMenu = mb->addMenu(tr("Help"));
        helpMenu->addAction(showHelpAction);

}
//----------------------------------------------------------------------------------------------------

crossCheckerWindow::~crossCheckerWindow()
{

}


