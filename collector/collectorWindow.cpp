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
    table->skipEmptyCells = true;
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
    collectorWindow::instance = 1;

}
//----------------------------------------------------------------------------------------------------

void collectorWindow::addColumn()
{
    table->addColumn();
    hasChanges = true;
}
//----------------------------------------------------------------------------------------------------

void collectorWindow::addRow()
{
    table->addRow();
    hasChanges = true;
}
//----------------------------------------------------------------------------------------------------

void collectorWindow::loadStoredData()
{
    table->loadFromData(activeTestItem->data(2,Qt::UserRole).toList());
}
//----------------------------------------------------------------------------------------------------

void collectorWindow::pickCollection()
{
    table->loadFromCollection(collectorWindow::clickedItem);
    hasChanges = true;
}
//----------------------------------------------------------------------------------------------------

void collectorWindow::deleteSelectedColumns()
{
    table->deleteSelectedColumns();
    hasChanges = true;
}
//----------------------------------------------------------------------------------------------------

void collectorWindow::pickFromTree()
{
    table->loadFromItem(collectorWindow::clickedItem);
    hasChanges = true;
}
//----------------------------------------------------------------------------------------------------

void collectorWindow::moveColumnLeft()
{
    table->moveColumnLeft();
    hasChanges = true;
}
//----------------------------------------------------------------------------------------------------

void collectorWindow::moveColumnRight()
{
    table->moveColumnRight();
    hasChanges = true;
}
//----------------------------------------------------------------------------------------------------

int collectorWindow::instance = 0;
QTreeWidgetItem* collectorWindow::clickedItem = 0;

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

    for (int i = 0; i < table->columnHeaders.size(); i++){
        xmlWriter->writeTextElement("th",table->columnHeaders.at(i));

    }

    xmlWriter->writeEndElement();
}
//----------------------------------------------------------------------------------------------------

void collectorWindow::writeDataRows()
{
    QStringList colBuff;
    table->loadFromCache();

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

    table->cacheTable();

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
    tb->addAction(pickCollectionAction);
    tb->addAction(addColumnAction);
    tb->addAction(addRowAction);
    tb->addSeparator();
    tb->addAction(moveColumnLeftAction);
    tb->addAction(moveColumnRightAction);
    tb->addSeparator();
    tb->addAction(deleteSelectedColumnsAction);

}
//----------------------------------------------------------------------------------------------------

void collectorWindow::storeCollection()
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
    hasChanges = true;

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

    pickCollectionAction = new QAction(icons->pickCollectionIcon, tr("Pick selected collection"),this);
    connect(pickCollectionAction,SIGNAL(triggered()),this,SLOT(pickCollection()));

    showHelpAction = new QAction(tr("Help"),this);
    connect(showHelpAction,SIGNAL(triggered()),this,SLOT(showHelp()));

    deleteSelectedColumnsAction= new QAction(icons->deleteIcon, tr("Delete selected columns"),this);
    connect(deleteSelectedColumnsAction,SIGNAL(triggered()),this,SLOT(deleteSelectedColumns()));

    clearTableAction = new QAction(QIcon(":/icons/clearIcon.png"),tr("Clear table"),this);
    connect(clearTableAction,SIGNAL(triggered()),this,SLOT(clearTable()));

    moveColumnLeftAction = new QAction(icons->leftIcon, tr("Move column left"),this);
    connect(moveColumnLeftAction,SIGNAL(triggered()),this,SLOT(moveColumnLeft()));

    moveColumnRightAction = new QAction(icons->rightIcon, tr("Move column right"),this);
    connect(moveColumnRightAction,SIGNAL(triggered()),this,SLOT(moveColumnRight()));

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
        variablesMenu->addAction(pickCollectionAction);
        variablesMenu->addSeparator();

    QMenu *columnsMenu = mb->addMenu(tr("Columns"));
        columnsMenu->addAction(deleteSelectedColumnsAction);
        columnsMenu->addAction(moveColumnLeftAction);
        columnsMenu->addAction(moveColumnRightAction);

    QMenu *helpMenu = mb->addMenu(tr("Help"));
        helpMenu->addAction(showHelpAction);

}
//----------------------------------------------------------------------------------------------------

collectorWindow::~collectorWindow()
{

}
