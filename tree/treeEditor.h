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

#ifndef TREEEDITOR_H
#define TREEEDITOR_H

#include <QObject>
#include <QtWidgets>

class iconsCatalog;

class treeEditor : public QObject
{
    Q_OBJECT
public:
    explicit treeEditor(QTreeWidget *tree = 0);

    QTreeWidget *treeWidget;
    QTreeWidgetItem *activeItem;
    QTreeWidgetItem *addNewItemAs(QString &itemName, const QString &itemType, QString &statusValue);
    static void expandParentsOf(QTreeWidgetItem *item);
    static void highlightItem(QTreeWidgetItem *item);
    static void clearHighlightsIn(QTreeWidget *tree);
    QString getItemType(QTreeWidgetItem *item);
    void setClickedItem(QTreeWidgetItem *item);

private:

    enum direction{
        Upwards,
        Downwards,
    };

     iconsCatalog *icons;
     QString itemName;
     QString defaultStatus;

     QTreeWidgetItem *clickedItem;
     QTreeWidgetItem *parentSnapshot;
     QTreeWidgetItem *childSnapshot;
     int clickedItemIndex;
     QTreeWidgetItem *clonedItem;

     void setItemTypeTo(QTreeWidgetItem *itemToSet, QString itemType);
     void markAs(QString status);
     void setParentsToReview(QTreeWidgetItem *item);
     void markChildrenOf(QTreeWidgetItem *item);
     void takeSnapshot();
     void restoreFromSnapshot();
     void expandIf(QTreeWidgetItem *item, QString hasStatus, QString hasText);
     void showOnlyWithStatus(QString status);
     void move(direction movingDirection);
     void moveClickedTopItem(treeEditor::direction movingDirection);
     void moveClickedSubItem(QTreeWidgetItem *parentItem, treeEditor::direction movingDirection);
     static void paintDiffColumnOf(QTreeWidgetItem *item);
     void updateParentOf(QTreeWidgetItem *item);
     void collapseItem(QTreeWidgetItem *item);
     void expandItem(QTreeWidgetItem *item);
     bool isValidTestPaste();
     bool isValidTreePaste();
signals:

public slots:

    void markAsPending();
    void markAsPlanned();
    void markAsValidated();
    void markAsReview();
    void markAsFailed();
    void markAsUnsupported();

    void copy();
    void cut();
    void paste();

    void collapseAll();
    void expandAll();
    void collapseSelected();
    void expandSelected();
    void showMissing();
    void showPlanned();
    void showPending();
    void showReview();
    void showValidated();
    void showUnsupported();
    void search();

    void addComponent();
    void addMultiple();
    void removeItem();
    void undo();
    void moveUp();
    void moveDown();

    void addTestGroup();
    void addTestCollection();
    void addTestCombination();
    void addTestSequence();
    void addTestCrossChecker();

    void preventDiffColumnEdition(QTreeWidgetItem *item, int column);


};

#endif // TREEEDITOR_H
