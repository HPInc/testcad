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

