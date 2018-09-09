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

#include "dataTable.h"

dataTable::dataTable()
{
    model = new tableModel();
}
//----------------------------------------------------------------------------------------------------

void dataTable::deleteSelectedColumns()
{
    QList<int> selectedIdexes = getSelectedIndexes();

    for (int i = 0; i < selectedIdexes.size(); i++){
        columnHeaders.removeAt(selectedIdexes.at( selectedIdexes.size() - 1 - i ) );
        columnsCache.removeAt(selectedIdexes.at( selectedIdexes.size() - 1 - i ));

    }

    loadFromCache();
}
//----------------------------------------------------------------------------------------------------

void dataTable::loadFromCollection(QTreeWidgetItem *treeItem)
{
    cacheTable();

    if ((treeItem != 0) &&
        (treeItem->data(0,Qt::UserRole).toString() == TAG_TYPE_TEST_COLLECTION)){
        CollectionData collBuffer = getCollectionDataFrom(treeItem);

        if (!collBuffer.variables.isEmpty()){
            clear();

            for (int i = 0; i < collBuffer.variables.size(); i++){
                columnHeaders.append(collBuffer.variables.at(i));
                columnsCache.append(collBuffer.valuesByVariable.at(i));

            }

            setColumnHeaders(columnHeaders);

            for (int i = 0; i < columnsCache.size(); i++){
                setColumnValues(i, columnsCache.at( i ));

            }

            update();
        }
    }
}
//----------------------------------------------------------------------------------------------------

CollectionData dataTable::getCollectionDataFrom(QTreeWidgetItem *treeItem){
    CollectionData collData;
    QStringList vars;
    QList<QStringList> vals;
    QStringList valBuff;
    QList <QVariant> designData = treeItem->data(2,Qt::UserRole).toList();

    if (designData.size() > 0){
        QVariantList headersStored = designData.at(0).toList();

        for (int i = 1; i < designData.size(); i++){
            vars.append(headersStored.at(i - 1).toString());
            QVariantList storedColumnBuff = designData.at(i).toList();

            for (int j = 0; j < storedColumnBuff.size(); j++){
                valBuff.append(storedColumnBuff.at( j ).toString());

            }

            vals.append(valBuff);
            valBuff.clear();
        }

        collData.variables = vars;
        collData.valuesByVariable = vals;
    }

    return collData;
}
//----------------------------------------------------------------------------------------------------

void dataTable::loadFromItem(QTreeWidgetItem *treeItem)
{
    QTreeWidgetItem *item;
    QList<QTreeWidgetItem*> varBuffer;
    QStringList columnBuffer;
    QList<QTreeWidgetItem*> variables;

    cacheTable();

    if (treeItem != 0){
        varBuffer = getVariableItemsFrom(treeItem);
        variables.append(varBuffer);
        clear();

        for (int i = 0; i < variables.size(); i++){
            item = variables.at(i);
            columnHeaders.append(item->text(0));

            for (int j=0; j < item->childCount(); j++){
                columnBuffer.append(item->child(j)->text(0));

            }

            columnsCache.append(columnBuffer);
            columnBuffer.clear();
        }

        for (int i = 0; i < columnsCache.size(); i++){
            setColumnValues(i, columnsCache.at( i ));

        }

        setColumnHeaders(columnHeaders);
        update();
    }
}
//----------------------------------------------------------------------------------------------------

QList<QTreeWidgetItem*> dataTable::getVariableItemsFrom(QTreeWidgetItem *treeItem)
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

bool dataTable::isValidVariable(QTreeWidgetItem *treeItem)
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

void dataTable::moveColumn(int polarity)
{
    QList<int> selectedIdexes = getSelectedIndexes();
    QString headerToMove;
    QStringList columnToMove;
    int nextPosIdx;
    cacheTable();

    if (polarity < 0){
        polarity = -1;
    }else{
        polarity = 1;
    }

    for (int i = 0; i < selectedIdexes.size(); i++){
        headerToMove = columnHeaders.at(selectedIdexes.at(i));
        columnToMove = columnsCache.at(selectedIdexes.at(i));
        columnHeaders.removeAt(selectedIdexes.at(i));
        columnsCache.removeAt(selectedIdexes.at(i));
        nextPosIdx = selectedIdexes.at(i) + polarity;

        if (nextPosIdx > columnsCache.size()){
            columnHeaders.prepend(headerToMove);
            columnsCache.prepend(columnToMove);
            nextPosIdx = 0;

        }else if (nextPosIdx < 0){
            columnHeaders.append(headerToMove);
            columnsCache.append(columnToMove);
            nextPosIdx = columnsCache.size() - 1;

        }else{
            columnHeaders.insert(nextPosIdx,headerToMove);
            columnsCache.insert(nextPosIdx,columnToMove);

        }
    }

    loadFromCache();
    selectColumn(nextPosIdx);
}
//----------------------------------------------------------------------------------------------------

void dataTable::loadFromData(QList<QVariant> tableData)
{
    QStringList columnBuff;

    if (tableData.size() > 0){
        QVariantList headersStored = tableData.at(0).toList();

        for (int i = 1; i < tableData.size(); i++){
            columnHeaders.append(headersStored.at(i - 1).toString());
            QVariantList storedColumnBuff = tableData.at(i).toList();

            for (int j = 0; j < storedColumnBuff.size(); j++){
                columnBuff.append(storedColumnBuff.at( j ).toString());

            }

            columnsCache.append(columnBuff);
            columnBuff.clear();

        }

        loadFromCache();
    }
}
//----------------------------------------------------------------------------------------------------

void dataTable::moveColumnRight()
{
    moveColumn(1);
}
//----------------------------------------------------------------------------------------------------

void dataTable::moveColumnLeft()
{
    moveColumn(-1);
}
//----------------------------------------------------------------------------------------------------

QList<int> dataTable::getSelectedIndexes()
{
    QList<int> selectedIdexes;

    if (model->hasIndex(0,0)){
        QModelIndex index;
        QModelIndexList indexes = currentSelection->selectedColumns();

        foreach (index, indexes){
            selectedIdexes.append(index.column());

        }
    }

    return selectedIdexes;
}
//----------------------------------------------------------------------------------------------------

void dataTable::cacheTable()
{
    columnHeaders = getColumnHeaders();
    rowHeaders = getRowHeaders();
    columnsCache = getColumnValues();
    QStringList colWithBlanks;
    QStringList colWithoutBlanks;

    for (int i = 0; i < columnsCache.size(); i++){
        colWithBlanks = columnsCache.at(i);

        for (int j = 0; j < colWithBlanks.size(); j++){

            if (!colWithBlanks.at(j).isEmpty()){
                colWithoutBlanks.append(colWithBlanks.at(j));

            }
        }

        columnsCache.replace(i, colWithoutBlanks);
        colWithoutBlanks.clear();
    }

}
//----------------------------------------------------------------------------------------------------

void dataTable::loadFromCache()
{
    clear();
    setColumnHeaders(columnHeaders);
    setRowHeaders(rowHeaders);
    setColumnValues(columnsCache);
    update();

}
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------

void dataTable::andByCells(QList<QStringList> valuesByColumn)
{
    QStringList refCol;
    QStringList colBuff;
    int n;

    if((valuesByColumn.size()>0) && (model->valuesByColumn.size()>0)){

        for(int i=0; i<model->valuesByColumn.size(); i++){
            colBuff = model->valuesByColumn.at(i);

            if(i <= valuesByColumn.size()-1){
                refCol = valuesByColumn.at(i);
                n = 0;

                while((n <= colBuff.size()-1 ) && (n <= refCol.size()-1 )){

                    if (refCol.at(n) == colBuff.at(n)){
                        this->setCellBackground(n, i, Qt::yellow);

                    }

                    n++;
                }
            }
        }
    }

}
//----------------------------------------------------------------------------------------------------

QList<bool> dataTable::andByRows(QList<QStringList> valuesByColumn)
{
    QStringList refRow;
    QStringList rowBuff;
    int cellsHit;
    QList<bool> rowHitMask;
    QList<bool> andMask;

    if((valuesByColumn.size()>0) && (model->valuesByColumn.size()>0)){

        for(int j=0; j<this->rowCount(); j++){
            rowBuff = this->getRow(j);
            andMask.append(false);

            for (int i=0; i<valuesByColumn.at(0).size(); i++){
                refRow = this->getRowFrom(i,valuesByColumn);
                rowHitMask.clear();
                cellsHit = 0;

                for(int n=0; n<rowBuff.size(); n++){
                    rowHitMask.append(false);

                    for(int m=0; m<refRow.size(); m++){

                        if (refRow.at(m) == rowBuff.at(n)){
                            cellsHit++;
                            rowHitMask.replace(n,true);
                            break;
                        }
                    }
                }

                if((cellsHit == refRow.size()) || (cellsHit == rowBuff.size())){

                    for(int k=0; k<rowBuff.size(); k++){

                        if(rowHitMask.at(k) == true){
                            this->setCellBackground(j,k, Qt::yellow);
                            andMask.replace(j,true);

                        }
                    }
                }
            }
        }
    }

    return andMask;
}
//----------------------------------------------------------------------------------------------------

QStringList dataTable::getRowFrom(int rowIndex, QList<QStringList> sourceColumns)
{
    QStringList colBuff;
    QStringList rowBuff;

    for (int i = 0; i < sourceColumns.size(); i++){
        colBuff = sourceColumns.at(i);
        rowBuff.append(colBuff.at(rowIndex));

    }

    return rowBuff;
}
//----------------------------------------------------------------------------------------------------

QStringList dataTable::getRow(int rowIndex)
{
    return getRowFrom(rowIndex, model->valuesByColumn);
}
//----------------------------------------------------------------------------------------------------

void dataTable::setCellBackground(int cellRow, int cellColumn, QColor cellColor)
{
    QList< QList< QColor > > tableBackground = getTableBackground();
    QList< QColor > colBackground = tableBackground.at(cellColumn);
    colBackground.replace(cellRow, cellColor);
    tableBackground.replace(cellColumn,colBackground);
    model->backgroundColorByColumn = tableBackground;
    this->setModel(NULL);
    this->setModel(model);

}
//----------------------------------------------------------------------------------------------------

QList< QColor > dataTable::getColumnBackground(int columnIndex)
{
    QList< QColor > columnBackground;

    while (model->backgroundColorByColumn.size() > model->columnHeaders.size())
        model->backgroundColorByColumn.removeLast();

    if(columnIndex <= model->columnHeaders.size()-1){

        if(columnIndex <= model->backgroundColorByColumn.size()-1)
            columnBackground = model->backgroundColorByColumn.at(columnIndex);

        while (columnBackground.size() < rowCount()){
            columnBackground.append(Qt::transparent);

        }
    }

    return columnBackground;
}
//----------------------------------------------------------------------------------------------------

void dataTable::clearTableBackground()
{
    QList< QColor > columnColors;
    QList< QList< QColor > > colorsByColumn;

    for(int i = 0; i < model->rowHeaders.size(); i++)
        columnColors.append(Qt::transparent);

    for(int i = 0; i < model->columnHeaders.size(); i++)
        colorsByColumn.append(columnColors);

    model->backgroundColorByColumn = colorsByColumn;
    this->setModel(NULL);
    this->setModel(model);
}
//----------------------------------------------------------------------------------------------------

QList< QList< QColor > > dataTable::getTableBackground()
{
    QList< QList< QColor > > colorsByColumn;

    for(int i = 0; i < model->columnHeaders.size(); i++)
        colorsByColumn.append(getColumnBackground(i));

    model->backgroundColorByColumn = colorsByColumn;

    return colorsByColumn;
}
//----------------------------------------------------------------------------------------------------

void dataTable::setColumnValues(int columnIndex, QStringList columnValues)
{
    if (columnIndex < (model->valuesByColumn.size()-1) ){
        model->valuesByColumn.replace(columnIndex,columnValues);

    }else{

        while(columnIndex > (model->valuesByColumn.size()-1)){
            model->valuesByColumn.append(QStringList(""));

            if(columnIndex == (model->valuesByColumn.size()-1)){
                model->valuesByColumn.replace(columnIndex,columnValues);

            }
        }
    }
}
//----------------------------------------------------------------------------------------------------

void dataTable::setColumnValues(QList<QStringList> allColumns)
{
   for (int i = 0; i < allColumns.size(); i++){
       QStringList colBuff = allColumns.at(i);
       setColumnValues(i, colBuff);

   }
}
//----------------------------------------------------------------------------------------------------

QStringList dataTable::getColumnHeaders()
{
    return model->columnHeaders;
}
//----------------------------------------------------------------------------------------------------

QStringList dataTable::getRowHeaders()
{
    return model->rowHeaders;
}
//----------------------------------------------------------------------------------------------------

QList<QStringList> dataTable::getColumnValues()
{
    return model->valuesByColumn;
}
//----------------------------------------------------------------------------------------------------

void dataTable::clear()
{
    delete model;
    model = new tableModel();

}
//----------------------------------------------------------------------------------------------------

void dataTable::update()
{
    this->setModel(model);
    currentSelection = this->selectionModel();

}
//----------------------------------------------------------------------------------------------------

void dataTable::setColumnHeaders(QStringList columnHeaders)
{
    model->columnHeaders = columnHeaders;

}
//----------------------------------------------------------------------------------------------------

void dataTable::setRowHeaders(QStringList rowHeaders)
{
    model->rowHeaders = rowHeaders;

}
//----------------------------------------------------------------------------------------------------

int dataTable::rowCount()
{
    if (model->valuesByColumn.size() > 0){
        return model->rowCount();

    }

    return 0;

}
//----------------------------------------------------------------------------------------------------

int dataTable::columnCount()
{
    if (model->valuesByColumn.size() > 0){
        return model->columnCount();

    }

    return 0;
}

