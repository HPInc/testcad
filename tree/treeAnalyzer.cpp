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

QList<statistics> treeAnalyzer::getStatisticsList()
{
    QList<statistics> treeStatistics;
    int sumOfChildren;
    int pendingCount;
    int plannedCount;
    int reviewCount;
    int validatedCount;
    int failedCount;
    int unsupportedCount;
    variableStatistics vStats;

    treeItemChildrenCount itemsCounted;

    for(int i = 0; i< treeWidget->topLevelItemCount(); i++){
        statistics statBuff;
        statBuff.componentName = treeWidget->topLevelItem(i)->text(0);
        itemsCounted = childrenCountOf(treeWidget->topLevelItem(i));
        sumOfChildren = itemsCounted.modulesCount + itemsCounted.entriesCount;
        pendingCount = countOf(treeWidget->topLevelItem(i),STRING_STATUS_PENDING);
        plannedCount = countOf(treeWidget->topLevelItem(i),STRING_STATUS_PLANNED);
        reviewCount = countOf(treeWidget->topLevelItem(i),STRING_STATUS_REVIEW);
        validatedCount = countOf(treeWidget->topLevelItem(i),STRING_STATUS_VALIDATED);
        failedCount = countOf(treeWidget->topLevelItem(i),STRING_STATUS_FAILED);
        unsupportedCount = countOf(treeWidget->topLevelItem(i),STRING_STATUS_UNSUPPORTED);

        statBuff.modulesCount = QString::number(itemsCounted.modulesCount);
        statBuff.entriesCount = QString::number(itemsCounted.entriesCount);

        statBuff.pending = QString::number(pendingCount);
        statBuff.pendingPercent = percentOf(pendingCount, sumOfChildren);
        statBuff.planned = QString::number(plannedCount);
        statBuff.plannedPercent = percentOf(plannedCount, sumOfChildren);
        statBuff.review = QString::number(reviewCount);
        statBuff.reviewPercent = percentOf(reviewCount, sumOfChildren);
        statBuff.validated = QString::number(validatedCount);
        statBuff.validatedPercent = percentOf(validatedCount, sumOfChildren);
        statBuff.failed = QString::number(failedCount);
        statBuff.failedPercent = percentOf(failedCount, sumOfChildren);
        statBuff.unsupported = QString::number(unsupportedCount);
        statBuff.unsupportedPercent = percentOf(unsupportedCount, sumOfChildren);

        vStats = getVariableStatistics(treeWidget->topLevelItem(i));
        statBuff.variablesCount = QString::number(vStats.variablesCount);
        statBuff.fullCombinationSize = QString::number(vStats.possibleCombinations);

        treeStatistics.append(statBuff);
    }

    return treeStatistics;
}
//----------------------------------------------------------------------------------------------------

void treeAnalyzer::showStatistics()
{
    QList<statistics> treeStatistics = getStatisticsList();

    if (statsWindow::instance == 0){
        statisticsTable = new statsWindow(0, &treeStatistics);
        statsWindow::instance = 1;

    }else{
        statisticsTable->loadStatistics(&treeStatistics);

    }

    statisticsTable->show();

}
//----------------------------------------------------------------------------------------------------

treeItemChildrenCount treeAnalyzer::childrenCountOf(QTreeWidgetItem *treeItem)
{
    treeItemChildrenCount itCount;

    for(int i = 0; i < treeItem->childCount(); i++){
        if (treeItem->child(i)->data(0, Qt::UserRole) == TAG_TYPE_MODULE){
            itCount.modulesCount++;

        }else if(treeItem->child(i)->data(0, Qt::UserRole) == TAG_TYPE_ENTRY){
            itCount.entriesCount++;

        }

        treeItemChildrenCount buff = childrenCountOf(treeItem->child(i));
        itCount.modulesCount = itCount.modulesCount + buff.modulesCount;
        itCount.entriesCount = itCount.entriesCount + buff.entriesCount;
    }

    return itCount;
}
//----------------------------------------------------------------------------------------------------

int treeAnalyzer::countOf(QTreeWidgetItem *treeItem, QString status)
{
    int itemCount = 0;

    for(int i=0; i < treeItem->childCount(); i++){
        if (!status.isEmpty()){
            if (treeItem->child(i)->data(1, Qt::UserRole) == status){
                itemCount++;

            }
        }else{
            itemCount++;

        }

        itemCount = itemCount + countOf(treeItem->child(i),status);
    }

    return itemCount;
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

variableStatistics treeAnalyzer::getVariableStatistics(QTreeWidgetItem *treeItem)
{
    variableStatistics output;
    QString varTree = "";
    long variablesCount;
    long valuesCount = 0;
    long highestValCount = 0;
    double possibleCombinations = 1;
    int valuesBuffer;

    if (treeItem != 0){
        QList<QTreeWidgetItem*> itemsList = getVariableItemsFrom(treeItem);
        QTreeWidgetItem *item;
        variablesCount = itemsList.count();

        for (int i=0; i < itemsList.count(); i++){
            valuesBuffer = 0;
            item = itemsList.at(i);
            varTree = varTree + item->text(0) + "\n";

            for (int n=0; n < item->childCount(); n++){
                varTree = varTree + "\t" + item->child(n)->text(0) + "\n";
                valuesCount++;
                valuesBuffer++;

                if(valuesBuffer > highestValCount){
                    highestValCount = valuesBuffer;

                }

            }

            possibleCombinations = possibleCombinations * valuesBuffer;

        }

        output.variablesCount=variablesCount;
        output.valuesCount = valuesCount;
        output.higherValuesCount = highestValCount;
        output.possibleCombinations = possibleCombinations;
        output.variablesTree = varTree;

     }

    return output;

}
//----------------------------------------------------------------------------------------------------

bool treeAnalyzer::isValidVariable(QTreeWidgetItem *treeItem)
{
    bool isValid = true;

    if(treeItem->childCount()>1){

        for(int i = 0; i < treeItem->childCount(); i++){

            if (treeItem->child(i)->childCount() > 0){
                isValid = false;
                break;

            }

        }

    }else{
       isValid = false;

    }

    return isValid;
}
//----------------------------------------------------------------------------------------------------

QList<QTreeWidgetItem*> treeAnalyzer::getVariableItemsFrom(QTreeWidgetItem *treeItem)
{
    QList<QTreeWidgetItem*> lastParents;
    QList<QTreeWidgetItem*> lastPBuff;

    if (treeItem->childCount() > 1){

        for(int i = 0; i < treeItem->childCount(); i++){

            if(isValidVariable(treeItem)){
                lastParents.append(treeItem);
                break;

            }else{
                lastPBuff = getVariableItemsFrom(treeItem->child(i));

            }

            for (int n = 0; n < lastPBuff.count(); n++){
                lastParents.append(lastPBuff.at(n));

            }

        }

    }

    return lastParents;
}
//----------------------------------------------------------------------------------------------------

void treeAnalyzer::copyVariableStatisticsToClipboard()
{
    QClipboard *cb = QApplication::clipboard();

    variableStatistics vs = getVariableStatistics(clickedItem);
    QString statText = tr("Variables count: ") + QString::number(vs.variablesCount) + "\n" +
            tr("Values count: ") + QString::number(vs.valuesCount) + "\n" +
            tr("Higher values count: ") + QString::number(vs.higherValuesCount) + "\n" +
            tr("Possible combinations: ") + QString::number(vs.possibleCombinations) + "\n\n" +
            tr("Variables and values: ") + "\n\n" + vs.variablesTree;

    cb->setText(statText,QClipboard::Clipboard);

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

QString treeAnalyzer::percentOf(int count, int fromTotal)
{
    double percent = 0;
    QString output = "%1 %";

    if (fromTotal > 0){
        percent = (double)count/(double)fromTotal * 100;
    }

    return output.arg(percent,1,'f',1);
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
