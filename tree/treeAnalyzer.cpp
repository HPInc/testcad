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

#include "treeAnalyzer.h"

#include <QObject>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QApplication>
#include <QClipboard>
#include <QInputDialog>

treeAnalyzer::treeAnalyzer(QTreeWidget *tree)
{
    treeWidget = tree;
    clickedItem = 0;

}
//----------------------------------------------------------------------------------------------------

void treeAnalyzer::showStatistics()
{
    statisticsTable = new statsWindow(0, treeWidget);
    statisticsTable->show();

}
//----------------------------------------------------------------------------------------------------

void treeAnalyzer::copyVariableStatisticsToClipboard()
{
    QClipboard *cb = QApplication::clipboard();
    statsWindow *statsTable = new statsWindow(0, treeWidget);
    variableStatistics vs = statsTable->getVariableStatistics(clickedItem);

    delete statsTable;

    QString statText = tr("Variables count: ") + QString::number(vs.variablesCount) + "\n" +
            tr("Values count: ") + QString::number(vs.valuesCount) + "\n" +
            tr("Higher values count: ") + QString::number(vs.higherValuesCount) + "\n" +
            tr("Possible combinations: ") + QString::number(vs.possibleCombinations) + "\n\n" +
            tr("Variables and values: ") + "\n\n" + vs.variablesTree;

    cb->setText(statText,QClipboard::Clipboard);

}
//----------------------------------------------------------------------------------------------------

void treeAnalyzer::showDiffWith(QHash<QString, QString> diffHashTable)
{
    QTreeWidgetItemIterator it(treeWidget);
    iconsCatalog icons;
    QVariant itemType;
    QVariant itemStatusData;
    QString itemXPath;
    QIcon diffIcon;
    QString diffStatus;

    treeWidget->collapseAll();
    treeEditor::clearHighlightsIn(treeWidget);

    while(*it){
        itemXPath = getPathOf((*it));
        itemType = (*it)->data(0, Qt::UserRole);
        itemStatusData = (*it)->data(1, Qt::UserRole);

        diffStatus = diffHashTable.value(itemXPath);

        if(!diffStatus.isEmpty()){
            diffIcon = icons.getIconFor(itemType.toString(), diffStatus);

        }else{
            diffIcon = icons.getIconFor("","");

        }

        if(itemStatusData != "" ){

            if (QString::compare(itemStatusData.toString(), diffStatus,Qt::CaseSensitive) != 0 ){
                (*it)->setExpanded(true);
                treeEditor::expandParentsOf((*it));
                treeEditor::highlightItem((*it));
                (*it)->setIcon(1, diffIcon);

            }

        }

        ++it;
    }
}
//----------------------------------------------------------------------------------------------------

QString treeAnalyzer::getBranchesFrom(QTreeWidgetItem *treeItem, QString indent)
{
    QString branches;
    QString indents;

    if (treeItem->childCount() > 0){
        indents = indent + "\t";

        for(int i = 0; i < treeItem->childCount(); i++){
            branches = branches + indents + treeItem->child(i)->text(0) + "\r\n";
            branches = branches + getBranchesFrom(treeItem->child(i),indents);

        }

    }

    return branches;
}
//----------------------------------------------------------------------------------------------------

void treeAnalyzer::copyBranchToClipboard()
{
    if (clickedItem != 0){
        QString branches = clickedItem->text(0) + "\r\n" + getBranchesFrom(clickedItem, "");
        QClipboard *cb = QApplication::clipboard();
        cb->setText(branches,QClipboard::Clipboard);

    }

}
//----------------------------------------------------------------------------------------------------

int treeAnalyzer::longestStatusLength()
{
    int widestWidth = QString(STRING_TOTAL_COUNT).length();

    if(widestWidth < QString(STRING_STATUS_PENDING).length() ){
        widestWidth = QString(STRING_STATUS_PENDING).length();
    }
    if(widestWidth < QString(STRING_STATUS_PLANNED).length() ){
        widestWidth = QString(STRING_STATUS_PLANNED).length();
    }
    if(widestWidth < QString(STRING_STATUS_REVIEW).length() ){
        widestWidth = QString(STRING_STATUS_REVIEW).length();
    }
    if(widestWidth < QString(STRING_STATUS_VALIDATED).length() ){
        widestWidth = QString(STRING_STATUS_VALIDATED).length();
    }
    if(widestWidth < QString(STRING_STATUS_FAILED).length() ){
        widestWidth = QString(STRING_STATUS_FAILED).length();
    }

    return widestWidth;
}
//----------------------------------------------------------------------------------------------------

QString treeAnalyzer::align(QString stringToFormat)
{
    bool wentOk;
    stringToFormat.toInt(&wentOk,10);

    if(wentOk){//Is a numeric value
        return stringToFormat.leftJustified(4,' ',false);

    }else{
        return stringToFormat.rightJustified(longestStatusLength(),' ',false);

    }
}
//----------------------------------------------------------------------------------------------------

QString treeAnalyzer::getPathOf(QTreeWidgetItem *treeItem)
{
    QString pathString = treeItem->text(0);

    if(dynamic_cast<QTreeWidgetItem*>(treeItem->parent())){
        QTreeWidgetItem *parentBuffer = treeItem->parent();

        while (parentBuffer){
            pathString = parentBuffer->text(0) + "/" + pathString;
            parentBuffer = parentBuffer->parent();

        }
    }
    return pathString;
}
//----------------------------------------------------------------------------------------------------

void treeAnalyzer::setClickedItem(QTreeWidgetItem *item)
{
    clickedItem = item;
}
//----------------------------------------------------------------------------------------------------

void treeAnalyzer::copyPathToClipBoard()
{
    if (clickedItem != 0){
        QString itemPath = getPathOf(clickedItem);
        QClipboard *cb = QApplication::clipboard();
        cb->setText(itemPath,QClipboard::Clipboard);

    }
}
