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

#ifndef STATSWINDOW_H
#define STATSWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QTreeWidgetItem>
#include <QMessageBox>
#include <QXmlStreamWriter>
#include <QByteArray>
#include <QTime>
#include "testCAD.h"
#include "dataTable/dataTable.h"
#include "iconsCatalog.h"

class statistics{

public:
    QString componentName;
    QString modulesCount;
    QString entriesCount;
    QString pending;
    QString pendingPercent;
    QString planned;
    QString plannedPercent;
    QString review;
    QString reviewPercent;
    QString validated;
    QString validatedPercent;
    QString failed;
    QString failedPercent;
    QString unsupported;
    QString unsupportedPercent;
    QString variablesCount;
    QString fullCombinationSize;

};

class treeItemChildrenCount{

public:
    int modulesCount = 0;
    int entriesCount = 0;

};

class variableStatistics{

public:
    int variablesCount = 0;
    int valuesCount = 0;
    int higherValuesCount = 0;
    double possibleCombinations = 0;
    QString variablesTree = "";
    QStringList variables;

};

class statsWindow : public QMainWindow
{
    Q_OBJECT

private:
    void createToolBar();
    iconsCatalog *icons;
    void createActions();
    void createMenus();
    void setStatsHeaders();

    QXmlStreamWriter *xmlWriter;

    void writeHtmlStyle();
    void writeHtmlH1(int componentsCount);
    void writeColumnHeaders();
    void writeDataRows();
    bool writeHtmlTo(QString filePath);

    void closeEvent(QCloseEvent *event);

    treeItemChildrenCount childrenCountOf(QTreeWidgetItem *treeItem);
    int countOf(QTreeWidgetItem *treeItem, QString status);
    QString percentOf(int count, int fromTotal);
    QTreeWidget *tree;

public:
    statsWindow(QWidget *parent = 0, QTreeWidget *tree = 0);
    variableStatistics getVariableStatistics(QTreeWidgetItem *treeItem);
    QList<statistics> getStatistics();
    void loadTable();
    static int instance;
    QAction *moveUpAction;
    QAction *moveDownAction;
    QAction *showHelpAction;
    QAction *deleteSelectedRowsAction;
    QAction *exportStatsAction;
    dataTable *table;
    ~statsWindow();

signals:
    void saved();

public slots:
    void moveRowUp();
    void moveRowDown();
    void showHelp();
    void deleteSelectedRows();
    void exportStats();

};

#endif // STATSWINDOW_H
