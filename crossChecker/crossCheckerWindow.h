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

#ifndef CROSSCHECKERWINDOW_H
#define CROSSCHECKERWINDOW_H

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

class crossCheckerWindow : public QMainWindow
{
    Q_OBJECT

private:
    QStringList getSelectionChildren();
    QStringList columnHeaders;
    QStringList rowHeaders;
    QList<QStringList> columnsCache;
    QList<QStringList> clickedItemColumns();
    QStringList headerParents;
    QLabel *rowsParentLabel;
    QLabel *columnsParentLabel;
    QLabel *andWithLabel;

    enum headerFor{
        rows,
        columns,
    };

    void setHeaderFor(int tablePart);
    void resetCache();
    void loadHeaderParent(int element, QString parentText);
    void loadStoredData();
    QList<int> getSelectedIndexes();
    void createToolBar();
    iconsCatalog *icons;
    void createActions();
    void createMenus();
    void writeTableToCache();  
    void closeEvent(QCloseEvent *event);
    bool hasChanges;
    void applyCoverageTo(QList<QStringList> *columnsList);

    QXmlStreamWriter *xmlWriter;

    void writeHtmlStyle();
    void writeHtmlH1();
    void writeColumnHeaders();
    void writeDataRows();
    bool writeHtmlTo(QString filePath);
    int selectedColumn;

public:
    explicit crossCheckerWindow(QWidget *parent = 0, QTreeWidgetItem *testItem = 0);
    static int instance;
    void updateTable();

    QAction *shiftUpAction;
    QAction *shiftDownAction;
    QAction *coverAllAction;
    QAction *pickRowHeadersAction;
    QAction *pickColumnHeadersAction;
    QAction *saveCheckerAction;
    QAction *coverSelectedAction;
    QAction *showHelpAction;
    QAction *clearTableAction;
    QAction *deleteSelectedColumnsAction;
    QAction *moveColumnLeftAction;
    QAction *moveColumnRightAction;
    QAction *exportCheckerAction;
    QAction *clearHighlightsAction;
    QAction *andAction;

    QTreeWidgetItem *activeTestItem;
    static QTreeWidgetItem *clickedItem;
    dataTable *table;
    ~crossCheckerWindow();

signals:
    void saved();

public slots:
    void shiftSelectedUp();
    void shiftSelectedDown();
    void coverAll();
    void pickColumnHeadersFromTree();
    void pickRowHeadersFromTree();
    void coverSelected();
    void storeChecker();
    void showHelp();
    void clearTable();
    void deleteSelectedColumns();
    void moveColumnLeft();
    void moveColumnRight();
    void exportChecker();
    void andWithSelected();
    void clearHighlights();
};

#endif // CROSSCHECKERWINDOW_H
