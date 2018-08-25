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

#ifndef DATATABLE_H
#define DATATABLE_H
#include <QTableView>
#include <QList>
#include <QAction>
#include <QTreeWidgetItem>
#include "dataTable/tableModel.h"

class dataTable : public QTableView
{
public:
    dataTable();
    tableModel *model;
    void clear();
    void update();
    void setColumnHeaders(QStringList columnHeaders);
    void setRowHeaders(QStringList rowHeaders);
    QStringList getColumnHeaders();
    QStringList getRowHeaders();
    void setColumnValues(int columnIndex, QStringList columnValues);
    void setColumnValues(QList<QStringList> allColumns);
    QList<QStringList> getColumnValues();
    int rowCount();
    int columnCount();
    QItemSelectionModel *currentSelection;
    QStringList getRowFrom(int rowIndex, QList<QStringList> sourceColumns);
    QStringList getRow(int rowIndex);
    QList< QList< QColor > >getTableBackground();
    void clearTableBackground();
    void setCellBackground(int cellRow, int cellColumn, QColor cellColor);
    QList<bool> andByRows(QList<QStringList> valuesByColumn);
    void andByCells(QList<QStringList> valuesByColumn);

private:
    QList< QColor > getColumnBackground(int columnIndex);

};

#endif // DATATABLE_H
