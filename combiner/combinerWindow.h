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

#ifndef COMBINERWINDOW_H
#define COMBINERWINDOW_H

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

class SelectionData{

public:
    QList<QStringList> selectedColumns;
    QList<int> selectedIndexes;
    QStringList selectionHeaders;
    double RowsMultiplied;

};

class CollectionData{

public:
    QStringList variables;
    QList<QStringList> valuesByVariable;

};

class combinerWindow : public QMainWindow
{
    Q_OBJECT

private:
    QList<QTreeWidgetItem *> getVariableItemsFrom(QTreeWidgetItem *treeItem);
    CollectionData getCollectionFrom(QTreeWidgetItem *treeItem);
    bool isValidVariable(QTreeWidgetItem *treeItem);
    QList<QStringList> columnsCache;
    QList<QStringList> clickedItemColumns();
    QStringList columnHeaders;
    void loadStoredData();
    QList<int> getSelectedIndexes();
    SelectionData getSelectionData();
    int getRandom(int upper);
    void createToolBar();
    iconsCatalog *icons;
    void createActions();
    void createMenus();
    void writeTableToCache();  
    void closeEvent(QCloseEvent *event);
    bool hasChanges;
    int selectedColumn;

    QXmlStreamWriter *xmlWriter;
    void writeHtmlStyle();
    void writeHtmlH1(int combinationsCount);
    void writeColumnHeaders();
    void writeDataRows();
    bool writeHtmlTo(QString filePath);
    QLabel *andWithLabel;
    QList<bool> andRows;

public:
    explicit combinerWindow(QWidget *parent = 0, QTreeWidgetItem *testItem = 0);
    void updateTable();

    QAction *andAction;
    QAction *combineAction;
    QAction *clearTableAction;
    QAction *clearHighlightsAction;
    QAction *deleteSelectedColumnsAction;
    QAction *exportCombinationAction;
    QAction *fillDownAction;
    QAction *pickVariablesAction;
    QAction *pickCollectionAction;
    QAction *moveColumnLeftAction;
    QAction *moveColumnRightAction;
    QAction *removeCoveredAction;
    QAction *randomizeAction;
    QAction *shiftUpAction;
    QAction *shiftDownAction;
    QAction *saveCombinationAction;
    QAction *showHelpAction;

    QTreeWidgetItem *activeTestItem;
    static QTreeWidgetItem *clickedItem;
    dataTable *table;
    ~combinerWindow();

signals:
    void saved();

public slots:
    void andWithSelected();
    void calculateOnSelection(const QModelIndex & index);
    void clearHighlights();
    void combineSelected();
    void clearTable();
    void deleteSelectedColumns();
    void exportCombination();
    void fillDownSelected();
    void moveColumnLeft();
    void moveColumnRight();
    void removeCovered();
    void pickFromTree();
    void pickCollection();
    void randomizeSelected();
    void storeCombination();
    void showHelp();
    void shiftSelectedUp();
    void shiftSelectedDown();

};

#endif // COMBINERWINDOW_H
