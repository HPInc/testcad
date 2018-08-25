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

#include "tableModel.h"

tableModel::tableModel()
{

}
//----------------------------------------------------------------------------------------------------

QVariant tableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.column() >=  valuesByColumn.size() || index.row() >=  valuesByColumn.at(index.column()).size() || index.row() < 0)
        return QVariant();

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        QStringList sl  = valuesByColumn.at(index.column());
        return sl.at(index.row());

    }

    if (role == Qt::BackgroundRole) {

        if (backgroundColorByColumn.size() > 0 &&
                index.column() <=  backgroundColorByColumn.size()-1 &&
                index.row() <=  backgroundColorByColumn.at(index.column()).size()-1){

            QList< QColor > colors = backgroundColorByColumn.at(index.column());
            return colors.at(index.row());

        }
     }

    return QVariant();
}
//----------------------------------------------------------------------------------------------------

bool tableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole) {
        QStringList sl = valuesByColumn.at(index.column());

        if ((index.row() < sl.size() - 1)||
           (index.row() == sl.size() - 1)){

            sl.replace(index.row(),value.toString());

        }else{
            sl.append(value.toString());

        }

        valuesByColumn.replace(index.column(),sl);
        emit(dataChanged(index, index));
        return true;

    }else{
        return false;

    }

    return false;
}
//----------------------------------------------------------------------------------------------------

QVariant tableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        return columnHeaders.at(section);

    }else if(orientation == Qt::Vertical){

        if (section < rowHeaders.size()){
            return rowHeaders.at(section);

        }

        return QString::number(section + 1);

    }

    return QVariant();
}
//----------------------------------------------------------------------------------------------------

Qt::ItemFlags tableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}
//----------------------------------------------------------------------------------------------------

int tableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    int higherRowCount = 0;

    for (int i = 0; i < valuesByColumn.size(); i++ ){
        QStringList sl = valuesByColumn.at(i);

        if (sl.size() > higherRowCount)
            higherRowCount = sl.size();

    }

    return higherRowCount;
}
//----------------------------------------------------------------------------------------------------

int tableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return columnHeaders.size();
}



