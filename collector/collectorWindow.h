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

#ifndef COLLECTORWINDOW_H
#define COLLECTORWINDOW_H

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

class collectorWindow : public QMainWindow
{
    Q_OBJECT

private:
    void loadStoredData();
    void createToolBar();
    iconsCatalog *icons;
    void createActions();
    void createMenus();
    void closeEvent(QCloseEvent *event);
    bool hasChanges;

    QXmlStreamWriter *xmlWriter;
    void writeHtmlStyle();
    void writeHtmlH1(int collectionsCount);
    void writeColumnHeaders();
    void writeDataRows();
    bool writeHtmlTo(QString filePath);

public:
    explicit collectorWindow(QWidget *parent = 0, QTreeWidgetItem *testItem = 0);
    static int instance;

    QAction *addColumnAction;
    QAction *addRowAction;
    QAction *saveCollectionAction;
    QAction *pickVariablesAction;
    QAction *pickCollectionAction;
    QAction *showHelpAction;
    QAction *clearTableAction;
    QAction *deleteSelectedColumnsAction;
    QAction *moveColumnLeftAction;
    QAction *moveColumnRightAction;
    QAction *exportCollectionAction;

    QTreeWidgetItem *activeTestItem;
    static QTreeWidgetItem *clickedItem;
    dataTable *table;
    ~collectorWindow();

signals:
    void saved();

public slots:
    void addColumn();
    void addRow();
    void pickFromTree();
    void pickCollection();
    void storeCollection();
    void showHelp();
    void clearTable();
    void deleteSelectedColumns();
    void moveColumnLeft();
    void moveColumnRight();
    void exportCollection();

};

#endif // COLLECTORWINDOW_H
