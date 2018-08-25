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

#include <QtWidgets>
#include "treeEditor.h"
#include "iconsCatalog.h"
#include "testCAD.h"

treeEditor::treeEditor(QTreeWidget *tree)
{
    icons = new iconsCatalog();
    treeWidget = tree;
    activeItem = 0;
    clickedItem = 0;
    clonedItem = 0;
    childSnapshot = 0;
    defaultStatus = STRING_STATUS_PENDING;

}
//----------------------------------------------------------------------------------------------------

void treeEditor::markChildrenOf(QTreeWidgetItem *item){

    for (int i = 0; i < item->childCount(); i++){
        item->child(i)->setData(1,Qt::UserRole, item->data(1,Qt::UserRole));
        icons->assignIcon(item->child(i));

        if (item->child(i)->childCount() > 0)
            markChildrenOf(item->child(i));
    }
}
//----------------------------------------------------------------------------------------------------

//Changes the status of the parents up to advice a review
void treeEditor::setParentsToReview(QTreeWidgetItem *item){

    if (dynamic_cast<QTreeWidgetItem*>(item->parent())){
        item->parent()->setData(1,Qt::UserRole,STRING_STATUS_REVIEW);
        icons->assignIcon(item->parent());
        setParentsToReview(item->parent());
    }
}
//----------------------------------------------------------------------------------------------------

void treeEditor::markAs(QString status){

    if (clickedItem != 0){
        takeSnapshot();//Store for eventual undo operation

        clickedItem->setData(1,Qt::UserRole,status);
        icons->assignIcon(clickedItem);

        if ((status == STRING_STATUS_PENDING)||
                (status == STRING_STATUS_VALIDATED)||
                (status == STRING_STATUS_FAILED)||
                (status == STRING_STATUS_UNSUPPORTED)){
            markChildrenOf(clickedItem); //Those status imply all children are affected

        }

        setParentsToReview(clickedItem); //Any change in the child should put the parent to review
    }
}
//----------------------------------------------------------------------------------------------------

bool treeEditor::isValidTreePaste()
{
    if((getItemType(clickedItem) == TAG_TYPE_COMPONENT ) &&
            ((getItemType(clonedItem) == TAG_TYPE_MODULE) ||
             (getItemType(clonedItem) == TAG_TYPE_ENTRY))){
        return true; //Components can only take cloned modules and entries

    }else if (((getItemType(clickedItem) == TAG_TYPE_MODULE) ||
              (getItemType(clickedItem) == TAG_TYPE_ENTRY)) &&
                ((getItemType(clonedItem) == TAG_TYPE_MODULE) ||
                 (getItemType(clonedItem) == TAG_TYPE_ENTRY))){
        return true; //Entries and modules can take only entries or modules

    }

    return false;
}
//----------------------------------------------------------------------------------------------------

bool treeEditor::isValidTestPaste()
{
    if((getItemType(clickedItem) == TAG_TYPE_TEST_GROUP ) &&
            ((getItemType(clonedItem) == TAG_TYPE_TEST_COLLECTION) ||
             (getItemType(clonedItem) == TAG_TYPE_TEST_COMBINATION) ||
             (getItemType(clonedItem) == TAG_TYPE_TEST_SEQUENCE) ||
             (getItemType(clonedItem) == TAG_TYPE_TEST_CROSSCHECK))){
        return true; //Test Group can take cloned combinations and sequences

    }else if ((getItemType(clickedItem) == TAG_TYPE_TEST_COLLECTION) ||
              (getItemType(clickedItem) == TAG_TYPE_TEST_COMBINATION) ||
              (getItemType(clickedItem) == TAG_TYPE_TEST_SEQUENCE) ||
              (getItemType(clickedItem) == TAG_TYPE_TEST_CROSSCHECK)){
        return false; //Test items cannot take anything

    }

    return false;
}
//----------------------------------------------------------------------------------------------------

void treeEditor::paste()
{
    if( ( clickedItem ) && ( clonedItem ) ){

        if(isValidTreePaste() || isValidTestPaste()){
            clickedItem->addChild(clonedItem);

            if ((getItemType(clickedItem) == TAG_TYPE_MODULE) || (getItemType(clickedItem) == TAG_TYPE_ENTRY)){
                updateParentOf(clonedItem);

            }
        }

        if(getItemType(clonedItem) == TAG_TYPE_COMPONENT){
            treeWidget->addTopLevelItem(clonedItem);

        }

        clickedItem = 0;
        clonedItem = 0;
        activeItem = 0;
    }
}
//----------------------------------------------------------------------------------------------------

void treeEditor::addComponent()
{
    activeItem = 0;
    QString defaultName = "New component";
    activeItem = addNewItemAs(defaultName, TAG_TYPE_COMPONENT, defaultStatus);

}
//----------------------------------------------------------------------------------------------------

void treeEditor::addTestGroup()
{
    activeItem = 0;
    QString defaultName = "New test group";
    activeItem = addNewItemAs(defaultName, TAG_TYPE_TEST_GROUP, defaultStatus);
}
//----------------------------------------------------------------------------------------------------

void treeEditor::addTestCrossChecker()
{
    QString defaultName = "New cross checker";

    if(getItemType(activeItem) == TAG_TYPE_TEST_GROUP){
        addNewItemAs(defaultName, TAG_TYPE_TEST_CROSSCHECK, defaultStatus);

    }
}
//----------------------------------------------------------------------------------------------------

void treeEditor::addTestCollection()
{
    QString defaultName = "New collection";

    if(getItemType(activeItem) == TAG_TYPE_TEST_GROUP){
        addNewItemAs(defaultName, TAG_TYPE_TEST_COLLECTION, defaultStatus);

    }
}
//----------------------------------------------------------------------------------------------------

void treeEditor::addTestCombination()
{
    QString defaultName = "New combination";

    if(getItemType(activeItem) == TAG_TYPE_TEST_GROUP){
        addNewItemAs(defaultName, TAG_TYPE_TEST_COMBINATION, defaultStatus);

    }
}
//----------------------------------------------------------------------------------------------------

void treeEditor::addTestSequence()
{
    QString defaultName = "New sequence";

    if(getItemType(activeItem) == TAG_TYPE_TEST_GROUP){
        addNewItemAs(defaultName, TAG_TYPE_TEST_SEQUENCE, defaultStatus);

    }
}
//----------------------------------------------------------------------------------------------------

void treeEditor::addMultiple()
{
    QInputDialog multiDialog;
    multiDialog.setFixedSize(350,600);
    multiDialog.setInputMode(QInputDialog::TextInput);
    multiDialog.setOption(QInputDialog::UsePlainTextEditForTextInput,true);
    multiDialog.setWindowTitle(tr(STRING_ACTION_ADD));
    multiDialog.setLabelText(tr(ADD_MULTI_DIALOG_LABEL));
    multiDialog.setFont(QFont("Arial",10));
    multiDialog.exec();

    QString entries = multiDialog.textValue();
    QStringList itemNames = entries.split("\n", QString::SkipEmptyParts);
    QList<QString>::iterator it;

    for (it = itemNames.begin(); it < itemNames.end(); ++it){
            activeItem = clickedItem;
            activeItem = addNewItemAs(*it,TAG_TYPE_ENTRY, defaultStatus);

    }
}
//----------------------------------------------------------------------------------------------------

void treeEditor::search()
{
    QInputDialog searchDialog;
    searchDialog.setFixedSize(512,256);
    searchDialog.setWindowTitle(QObject::tr(SEARCH_DIALOG_TITLE));
    searchDialog.setLabelText(QObject::tr(SEARCH_DIALOG_LABEL));
    searchDialog.exec();

    if (!searchDialog.textValue().isEmpty()){
        if (treeWidget->topLevelItemCount()>0){
            treeWidget->collapseAll();
            clearHighlightsIn(treeWidget);

            for (int i = 0; i < treeWidget->topLevelItemCount(); i++){
                QTreeWidgetItem *item = treeWidget->topLevelItem(i);
                expandIf(item, "", searchDialog.textValue());

            }
        }
    }
}
//----------------------------------------------------------------------------------------------------

void treeEditor::moveClickedTopItem(treeEditor::direction movingDirection)
{
    int clickedItemIndex = treeWidget->indexOfTopLevelItem(clickedItem);

    if(movingDirection == treeEditor::Upwards){
        if(clickedItemIndex > 0){
            QTreeWidgetItem* itemToMove = treeWidget->takeTopLevelItem(clickedItemIndex);
            treeWidget->insertTopLevelItem(clickedItemIndex - 1, itemToMove);
        }

    }else if(movingDirection == treeEditor::Downwards){
        if(clickedItemIndex < treeWidget->topLevelItemCount()-1){
            QTreeWidgetItem* itemToMove = treeWidget->takeTopLevelItem(clickedItemIndex);
            treeWidget->insertTopLevelItem(clickedItemIndex + 1, itemToMove);
        }
    }
}

//----------------------------------------------------------------------------------------------------

void treeEditor::moveClickedSubItem(QTreeWidgetItem *parentItem, treeEditor::direction movingDirection)
{
    int clickedItemIndex = parentItem->indexOfChild(clickedItem);
    QList<QTreeWidgetItem*> itemListBuffer = parentItem->takeChildren();
    QList<QTreeWidgetItem*>::iterator i;
    int listIndex = 0;

    for (i = itemListBuffer.begin();i!=itemListBuffer.end();++i){
        if(movingDirection == treeEditor::Upwards){
            if (listIndex == clickedItemIndex - 1){
                parentItem->addChild(itemListBuffer.at(clickedItemIndex));
                parentItem->addChild(itemListBuffer.at(listIndex));
                ++i;

            }else{
                parentItem->addChild(*i);

            }
        }else if(movingDirection == treeEditor::Downwards){
            if ((listIndex == clickedItemIndex) && (clickedItemIndex < itemListBuffer.count() - 1)){
                    parentItem->addChild(itemListBuffer.at(listIndex + 1));
                    parentItem->addChild(itemListBuffer.at(clickedItemIndex));
                    ++i;

            }else{
                parentItem->addChild(*i);

            }
        }
        listIndex++;
    }
}
//----------------------------------------------------------------------------------------------------

void treeEditor::paintDiffColumnOf(QTreeWidgetItem *item)
{
    QBrush diffBrush;
    diffBrush.setColor(QColor(235,235,235));
    diffBrush.setStyle(Qt::SolidPattern);
    item->setBackground(1, diffBrush);

}
//----------------------------------------------------------------------------------------------------

void treeEditor::updateParentOf(QTreeWidgetItem *item)
{
    QVariant parentItemType = item->parent()->data(0, Qt::UserRole);

    if(parentItemType.toString() == TAG_TYPE_ENTRY){
        setItemTypeTo(item->parent(),TAG_TYPE_MODULE);
        icons->assignIcon(item->parent());

    }
}

//----------------------------------------------------------------------------------------------------

void treeEditor::move(treeEditor::direction movingDirection)
{
    if((clickedItem != 0) && (treeWidget->topLevelItemCount() > 0)){
        QTreeWidgetItem* parentItem;
        parentItem = clickedItem->parent();

        if(dynamic_cast<QTreeWidgetItem*>(parentItem)){
            moveClickedSubItem(parentItem, movingDirection);

        }else{
            moveClickedTopItem(movingDirection); //The parent is not QTreeWidgetItem then it is the treeWidget itself

        }
    }
}
//----------------------------------------------------------------------------------------------------

void treeEditor::moveDown()
{
    move(treeEditor::Downwards);

}
//----------------------------------------------------------------------------------------------------

void treeEditor::preventDiffColumnEdition(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column);
    item->setText(1,"");

}
//----------------------------------------------------------------------------------------------------

void treeEditor::moveUp()
{
    move(treeEditor::Upwards);

}
//----------------------------------------------------------------------------------------------------

void treeEditor::cut()
{
    if(clickedItem){
        clonedItem = clickedItem->clone();
        delete clickedItem;
        clickedItem = 0;
    }
}
//----------------------------------------------------------------------------------------------------

void treeEditor::copy()
{
    if(clickedItem){
        clonedItem = clickedItem->clone();
        clickedItem = 0;

    }
}
//----------------------------------------------------------------------------------------------------

void treeEditor::expandParentsOf(QTreeWidgetItem *item)
{

    if (dynamic_cast<QTreeWidgetItem*>(item->parent())){
        item->parent()->setExpanded(true);
        expandParentsOf(item->parent());
    }
}
//----------------------------------------------------------------------------------------------------

void treeEditor::highlightItem(QTreeWidgetItem *item)
{
    QBrush highlight;
    highlight.setColor(Qt::yellow);
    highlight.setStyle(Qt::SolidPattern);

    item->setBackground(0,highlight);
    item->setBackground(1,highlight);
    item->setBackground(2,highlight);
}
//----------------------------------------------------------------------------------------------------

void treeEditor::clearHighlightsIn(QTreeWidget *tree)
{
    QTreeWidgetItemIterator it(tree);

    while(*it){
        (*it)->setBackground(0,Qt::NoBrush);
        (*it)->setBackground(2,Qt::NoBrush);
        paintDiffColumnOf((*it));
        ++it;
    }
}
//----------------------------------------------------------------------------------------------------

void treeEditor::setItemTypeTo(QTreeWidgetItem *itemToSet, QString itemType)
{
    itemToSet->setData(0, Qt::UserRole, itemType);

}
//----------------------------------------------------------------------------------------------------

QString treeEditor::getItemType(QTreeWidgetItem *item)
{
    if (item != 0){
         return item->data(0,Qt::UserRole).toString();

    }

    return "";
}
//----------------------------------------------------------------------------------------------------

void treeEditor::expandIf(QTreeWidgetItem *item, QString hasStatus, QString hasText){

    for (int i = 0; i < item->childCount(); i++){
        QTreeWidgetItem *childItem = item->child(i);

        if (!hasStatus.isEmpty()){
            QString itemStatus = childItem->data(1,Qt::UserRole).toString();
            if (itemStatus == hasStatus){
                childItem->setExpanded(true);
                expandParentsOf(childItem);

            }
        }

        if (!hasText.isEmpty()){
            QString itemText = childItem->text(0);

            if (itemText.contains(hasText,Qt::CaseInsensitive)){
                childItem->setExpanded(true);
                highlightItem(childItem);
                expandParentsOf(childItem);

            }

            itemText = childItem->text(2);

            if (itemText.contains(hasText,Qt::CaseInsensitive)){
                childItem->setExpanded(true);
                highlightItem(childItem);
                expandParentsOf(childItem);

            }
        }
        expandIf(childItem, hasStatus, hasText);
    }
}
//----------------------------------------------------------------------------------------------------

void treeEditor::collapseItem(QTreeWidgetItem *item)
{
    if(item!=0)
    {
        item->setExpanded(false);

        for (int i = 0; i < item->childCount(); i++){
            QTreeWidgetItem *childItem = item->child(i);
            childItem->setExpanded(false);
            collapseItem(childItem);

        }
    }

}
//----------------------------------------------------------------------------------------------------

void treeEditor::expandItem(QTreeWidgetItem *item)
{
    if(item!=0)
    {
        item->setExpanded(false);
        item->setExpanded(true);

        for (int i = 0; i < item->childCount(); i++){
            QTreeWidgetItem *childItem = item->child(i);
            childItem->setExpanded(true);
            expandItem(childItem);

        }
    }
}
//----------------------------------------------------------------------------------------------------

void treeEditor::showOnlyWithStatus(QString status){
    treeWidget->collapseAll();

    for (int i = 0; i < treeWidget->topLevelItemCount(); i++)
    {
        QTreeWidgetItem *item = treeWidget->topLevelItem(i);
        expandIf(item, status, "");
    }
}
//----------------------------------------------------------------------------------------------------

void treeEditor::takeSnapshot(){

    if (clickedItem->parent()){
        parentSnapshot = clickedItem->parent();
        clickedItemIndex = parentSnapshot->indexOfChild(clickedItem);

    }else{
        parentSnapshot = 0;
        clickedItemIndex = treeWidget->indexOfTopLevelItem(clickedItem);

    }
    childSnapshot = clickedItem->clone();
}
//----------------------------------------------------------------------------------------------------

void treeEditor::restoreFromSnapshot(){

    if(childSnapshot){
        if(parentSnapshot){
            parentSnapshot->insertChild(clickedItemIndex,childSnapshot);

        }else{
            treeWidget->insertTopLevelItem(clickedItemIndex,childSnapshot);

        }

        childSnapshot->setExpanded(true);
        parentSnapshot = 0;
        childSnapshot = 0;
    }
}
//----------------------------------------------------------------------------------------------------

void treeEditor::removeItem(){

    if(clickedItem){
        takeSnapshot();

        if (clickedItem->parent() != 0){

            if (clickedItem->parent()->childCount() == 1 &&
                    getItemType(clickedItem->parent()) != TAG_TYPE_COMPONENT &&
                    getItemType(clickedItem->parent()) != TAG_TYPE_TEST_GROUP )

            {
                setItemTypeTo(clickedItem->parent(), TAG_TYPE_ENTRY);
                icons->assignIcon(clickedItem->parent());

            }

        }

        delete clickedItem;
        clickedItem = 0;
        activeItem = 0;
    }
}
//----------------------------------------------------------------------------------------------------

void treeEditor::undo(){

    if((clickedItem != 0)&&(childSnapshot)){
        delete clickedItem;
        clickedItem = 0;
    }

    restoreFromSnapshot();

}
//----------------------------------------------------------------------------------------------------

void treeEditor::collapseAll()
{
    if (treeWidget->children().count()>0){
        treeWidget->collapseAll();

    }

}
//----------------------------------------------------------------------------------------------------

void treeEditor::expandAll()
{
    if (treeWidget->children().count()>0){
        treeWidget->expandAll();

    }

}
//----------------------------------------------------------------------------------------------------

void treeEditor::collapseSelected()
{
    collapseItem(clickedItem);

}
//----------------------------------------------------------------------------------------------------

void treeEditor::expandSelected()
{
    expandItem(clickedItem);

}
//----------------------------------------------------------------------------------------------------

void treeEditor::showPending(){
    showOnlyWithStatus(STRING_STATUS_PENDING);

}
//----------------------------------------------------------------------------------------------------

void treeEditor::showMissing(){
    showOnlyWithStatus(STRING_STATUS_FAILED);

}
//----------------------------------------------------------------------------------------------------

void treeEditor::showPlanned(){
    showOnlyWithStatus(STRING_STATUS_PLANNED);

}
//----------------------------------------------------------------------------------------------------

void treeEditor::showReview(){
    showOnlyWithStatus(STRING_STATUS_REVIEW);

}
//----------------------------------------------------------------------------------------------------

void treeEditor::showValidated(){
    showOnlyWithStatus(STRING_STATUS_VALIDATED);

}
//----------------------------------------------------------------------------------------------------

void treeEditor::showUnsupported(){
    showOnlyWithStatus(STRING_STATUS_UNSUPPORTED);

}
//----------------------------------------------------------------------------------------------------

QTreeWidgetItem* treeEditor::addNewItemAs(QString &itemName, const QString &itemType, QString &statusValue)
{
    QTreeWidgetItem *newItem;

    if (activeItem != 0){
        newItem = new QTreeWidgetItem(activeItem);
        setItemTypeTo(newItem, itemType);

        if((itemType != TAG_TYPE_TEST_COLLECTION) &&
                (itemType != TAG_TYPE_TEST_COMBINATION) &&
                (itemType != TAG_TYPE_TEST_SEQUENCE) &&
                (itemType != TAG_TYPE_TEST_CROSSCHECK)){
            updateParentOf(newItem);

        }

    }else{
        newItem = new QTreeWidgetItem(treeWidget);
        setItemTypeTo(newItem, itemType);

    }

    newItem->setData(1, Qt::UserRole, statusValue);
    icons->assignIcon(newItem);
    newItem->setText(0,itemName);
    newItem->setText(2,"...");

    paintDiffColumnOf(newItem);

    return newItem;
}
//----------------------------------------------------------------------------------------------------

void treeEditor::setClickedItem(QTreeWidgetItem *item){
    clickedItem = item;
    activeItem = item;

}
//----------------------------------------------------------------------------------------------------

void treeEditor::markAsPending(){
    markAs(STRING_STATUS_PENDING);
}
//----------------------------------------------------------------------------------------------------

void treeEditor::markAsPlanned(){
    markAs(STRING_STATUS_PLANNED);
}
//----------------------------------------------------------------------------------------------------

void treeEditor::markAsValidated(){
    markAs(STRING_STATUS_VALIDATED);
}
//----------------------------------------------------------------------------------------------------

void treeEditor::markAsReview(){
    markAs(STRING_STATUS_REVIEW);
}
//----------------------------------------------------------------------------------------------------

void treeEditor::markAsFailed(){
    markAs(STRING_STATUS_FAILED);
}
//----------------------------------------------------------------------------------------------------

void treeEditor::markAsUnsupported(){
    markAs(STRING_STATUS_UNSUPPORTED);
}
//----------------------------------------------------------------------------------------------------




