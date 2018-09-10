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
    this->setAttribute(Qt::WA_DeleteOnClose);
    createActions();
    createMenus();
    createToolBar();
    statusBar()->show();
    loadStatistics(treeStatistics);

}
//----------------------------------------------------------------------------------------------------

void statsWindow::moveRowUp()
{
    table->moveRowUp();
}
//----------------------------------------------------------------------------------------------------

void statsWindow::moveRowDown()
{
    table->moveRowDown();
}
//----------------------------------------------------------------------------------------------------

void statsWindow::deleteSelectedRows()
{
    table->deleteSelectedRows();
}
//----------------------------------------------------------------------------------------------------

void statsWindow::loadStatistics(QList<statistics> *treeStatistics)
{
    QList<QStringList> valuesByRow;
    QStringList rowVals;
    int colCount;

    setStatsHeaders();
    table->rowHeaders.clear();

    for (int i = 0; i < treeStatistics->size(); i++){
        table->rowHeaders.append(treeStatistics->at(i).componentName);
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

    table->columnsCache.clear();

    for (int i = 0; i < colCount; i++){
       QStringList colVals;

       for(int j = 0; j < valuesByRow.size(); j++){
           rowVals = valuesByRow.at(j);
           colVals.append(rowVals.at(i));

       }

       table->columnsCache.append(colVals);

    }

    table->loadFromCache();

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

    for (int i = 0; i < table->columnHeaders.size(); i++){
        xmlWriter->writeTextElement("th",table->columnHeaders.at(i));

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
        xmlWriter->writeCharacters(table->rowHeaders.at(i));
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

bool statsWindow::writeHtmlTo(QString filePath)
{
    if (filePath.isEmpty())
        return false;

    QFile file(filePath);

    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this,STRING_TESTCAD,
                             QObject::tr("Cannot write file...")
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

void statsWindow::closeEvent(QCloseEvent *event)
{
    statsWindow::instance = 0;
    event->accept();

}
//--------------------------------------------------------------------------------------------------------------------------------

int statsWindow::instance = 0;

//----------------------------------------------------------------------------------------------------

void statsWindow::exportStats()
{
    QString filePath = QFileDialog::getSaveFileName(this, QObject::tr("Save statistics"),
                                         QDir::currentPath(),
                                         QObject::tr(STRING_FILETYPE_HTML));

    table->cacheTable();

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

void statsWindow::createToolBar()
{
    QToolBar *tb = this->addToolBar(tr("Tools"));
    tb->setMovable(false);
    tb->addAction(exportStatsAction);
    tb->addSeparator();
    tb->addAction(moveUpAction);
    tb->addAction(moveDownAction);
    tb->addSeparator();
    tb->addAction(deleteSelectedRowsAction);

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

    moveUpAction = new QAction(icons->upIcon, tr("Move up"),this);
    connect(moveUpAction,SIGNAL(triggered()),this,SLOT(moveRowUp()));

    moveDownAction = new QAction(icons->downIcon, tr("Move down"),this);
    connect(moveDownAction,SIGNAL(triggered()),this,SLOT(moveRowDown()));
}
//----------------------------------------------------------------------------------------------------

void statsWindow::createMenus()
{
    QMenuBar *mb = this->menuBar();

    QMenu *columnsMenu = mb->addMenu(tr("Rows"));
        columnsMenu->addAction(deleteSelectedRowsAction);
        columnsMenu->addAction(moveUpAction);
        columnsMenu->addAction(moveDownAction);

    QMenu *helpMenu = mb->addMenu(tr("Help"));
        helpMenu->addAction(showHelpAction);

}
//----------------------------------------------------------------------------------------------------

void statsWindow::setStatsHeaders()
{
    table->columnHeaders.clear();
    table->columnHeaders.append("Variables");
    table->columnHeaders.append("Combinations");
    table->columnHeaders.append("Pending");
    table->columnHeaders.append("Pending %");
    table->columnHeaders.append("Planned");
    table->columnHeaders.append("Planned %");
    table->columnHeaders.append("Review");
    table->columnHeaders.append("Review %");
    table->columnHeaders.append("Validated");
    table->columnHeaders.append("Validated %");
    table->columnHeaders.append("Failed");
    table->columnHeaders.append("Failed %");
    table->columnHeaders.append("Unsupported");
    table->columnHeaders.append("Unsupported %");
    table->columnHeaders.append("Modules");
    table->columnHeaders.append("Entries");
}
//----------------------------------------------------------------------------------------------------

statsWindow::~statsWindow()
{

}
