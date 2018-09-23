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

class treeAnalyzer : public QObject
{
    Q_OBJECT

public:
    treeAnalyzer(QTreeWidget *tree);
    void showDiffWith(QHash<QString, QString> diffHashTable);
    void setClickedItem(QTreeWidgetItem *item);

private:
    int longestStatusLength();
    QString align(QString stringToFormat);
    QString getPathOf(QTreeWidgetItem *treeItem);
    QString getBranchesFrom(QTreeWidgetItem *treeItem, QString indent);
    QTreeWidgetItem *clickedItem;
    QTreeWidget *treeWidget;
    statsWindow *statisticsTable;

public slots:
    void copyPathToClipBoard();
    void copyBranchToClipboard();
    void copyVariableStatisticsToClipboard();
    void showStatistics();

};

#endif // TREEANALYZER_H
