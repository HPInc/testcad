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

#ifndef TREEANALYZER_H
#define TREEANALYZER_H

#include <QObject>
#include <QHash>
#include "treeEditor.h"
#include "testCAD.h"
#include "iconsCatalog.h"
#include "statsWindow/statsWindow.h"

class QTreeWidget;
class QTreeWidgetItem;
class QString;

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

};

class treeAnalyzer : public QObject
{
    Q_OBJECT

public:
    treeAnalyzer(QTreeWidget *tree);
    QList<statistics> getStatisticsList();
    void showDiffWith(QHash<QString, QString> diffHashTable);
    void setClickedItem(QTreeWidgetItem *item);

private:
    treeItemChildrenCount childrenCountOf(QTreeWidgetItem *treeItem);
    statsWindow *statisticsTable;
    int countOf(QTreeWidgetItem *treeItem, QString status);
    QString percentOf(int count, int fromTotal);
    int longestStatusLength();
    QString align(QString stringToFormat);

    QString getPathOf(QTreeWidgetItem *treeItem);
    QString getBranchesFrom(QTreeWidgetItem *treeItem, QString indent);
    QTreeWidgetItem *clickedItem;
    QTreeWidget *treeWidget;
    QList<QTreeWidgetItem*> getVariableItemsFrom(QTreeWidgetItem *treeItem);
    bool isValidVariable(QTreeWidgetItem *treeItem);
    variableStatistics getVariableStatistics(QTreeWidgetItem *treeItem);

public slots:
    void copyPathToClipBoard();
    void showStatistics();
    void copyBranchToClipboard();
    void copyVariableStatisticsToClipboard();

};

#endif // TREEANALYZER_H
