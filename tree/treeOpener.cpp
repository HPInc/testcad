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

#include "treeOpener.h"

treeOpener::treeOpener(QTreeWidget *treeWidget, QWidget *parent)
        : treeWidget(treeWidget)
{
    parentWidget = parent;
    rootTagFound = false;
    editor = new treeEditor(treeWidget);
    editor->activeItem = 0;

}
//----------------------------------------------------------------------------------------------------

QFile* treeOpener::openFileDialog()
{
    QFile *file = new QFile(filePath);

    if (!file->open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(parentWidget, QObject::tr(STRING_TESTCAD),
                             QObject::tr(STRING_CANNOT_READ)
                             .arg(filePath)
                             .arg(file->errorString()));
        return 0;
    }

    return file;
}
//----------------------------------------------------------------------------------------------------

void treeOpener::processStart(callPurpose purpose)
{
    if ((xmlReader->name().toString() == TAG_TYPE_COMPONENT) ||
                 (xmlReader->name().toString() == TAG_TYPE_MODULE) ||
                 (xmlReader->name().toString() == TAG_TYPE_ENTRY) ||
                 (xmlReader->name().toString() == TAG_TYPE_TEST_GROUP) ||
                 (xmlReader->name().toString() == TAG_TYPE_TEST_COLLECTION) ||
                 (xmlReader->name().toString() == TAG_TYPE_TEST_COMBINATION) ||
                 (xmlReader->name().toString() == TAG_TYPE_TEST_SEQUENCE) ||
                 (xmlReader->name().toString() == TAG_TYPE_TEST_CROSSCHECK) ){

        QString statusValue = xmlReader->attributes().value(TAG_ATTRIBUTE_STATUS).toString();
        QString nameValue = xmlReader->attributes().value(TAG_ATTRIBUTE_NAME).toString();

        if ( purpose == treeOpener::toLoad){
            editor->activeItem = editor->addNewItemAs(nameValue, xmlReader->name().toString(), statusValue);
            treeWidget->setItemExpanded(editor->activeItem, !(xmlReader->attributes().value(TAG_ATTRIBUTE_FOLDED).toString() == STRING_YES));

        }

        if ( purpose == treeOpener::toCompare){
            xPathParts.append(nameValue);
            QString xPath = getXPath(xPathParts);
            diffHashTable.insert(xPath, statusValue);

        }
    }

    if ( purpose == treeOpener::toLoad){

        if(xmlReader->name().toString() == TAG_COMMENT){

            if ( editor->activeItem != 0 )
                    editor->activeItem->setText(2, xmlReader->readElementText());
        }else if(xmlReader->name().toString() == TAG_TYPE_TEST_COLLECTION_COLUMN){
            tableColumnHeaders.append(xmlReader->attributes().value(TAG_ATTRIBUTE_TEST_COLLECTION_COLUMN_HEADER).toString());

        }else if(xmlReader->name().toString() == TAG_TYPE_TEST_COLLECTION_TABLE_COLUMN_CELL){
            tableColumn.append(xmlReader->attributes().value(TAG_ATTRIBUTE_TEST_COLLECTION_CELL_VALUE).toString());

        }else if(xmlReader->name().toString() == TAG_TYPE_TEST_COMBINATION_COLUMN){
            tableColumnHeaders.append(xmlReader->attributes().value(TAG_ATTRIBUTE_TEST_COMBINATION_COLUMN_HEADER).toString());

        }else if(xmlReader->name().toString() == TAG_TYPE_TEST_COMBINATION_TABLE_COLUMN_CELL){
            tableColumn.append(xmlReader->attributes().value(TAG_ATTRIBUTE_TEST_COMBINATION_CELL_VALUE).toString());

        }else if(xmlReader->name().toString() == TAG_TYPE_TEST_SEQUENCE_STATE){
            loadStateProperties();

        }else if(xmlReader->name().toString() == TAG_TYPE_TEST_SEQUENCE_TRANSITION){
            loadTransitionProperties();

        }else if(xmlReader->name().toString() == TAG_TYPE_TEXT){
            lastTextBuffer = xmlReader->readElementText();

        }else if(xmlReader->name().toString() == TAG_TYPE_TEST_CROSSCHECK_PARENTS){
            tableHeaderParents.clear();
            tableHeaderParents.append(xmlReader->attributes().value(TAG_ATTRIBUTE_CROSSCHECK_ROW_PARENT).toString());
            tableHeaderParents.append(xmlReader->attributes().value(TAG_ATTRIBUTE_CROSSCHECK_COLUMN_PARENT).toString());

        }else if(xmlReader->name().toString() == TAG_TYPE_TEST_CROSSCHECK_COLUMN){
            tableColumnHeaders.append(xmlReader->attributes().value(TAG_ATTRIBUTE_CROSSCHECK_COLUMN_HEADER).toString());

        }else if(xmlReader->name().toString() == TAG_TYPE_TEST_CROSSCHECK_CELL){
            tableColumn.append(xmlReader->readElementText());

        }else if(xmlReader->name().toString() == TAG_TYPE_TEST_CROSSCHECK_ROW_HEADER){
            tableRowHeaders.append(xmlReader->readElementText());
        }
    }
}
//----------------------------------------------------------------------------------------------------

void treeOpener::processEnd(callPurpose purpose)
{

    if ( purpose == treeOpener::toLoad){

        if (xmlReader->name().toString() == TAG_TYPE_COMPONENT ||
                       xmlReader->name().toString() == TAG_TYPE_TEST_GROUP ){
            editor->activeItem = 0;

        }else if (xmlReader->name().toString() == TAG_TYPE_MODULE ||
                    xmlReader->name().toString() == TAG_TYPE_ENTRY){
            editor->activeItem = editor->activeItem->parent();

        }else if(xmlReader->name().toString() == TAG_TYPE_TEST_COLLECTION){
            designData.prepend(tableColumnHeaders);
            editor->activeItem->setData(2,Qt::UserRole, designData);
            tableColumnHeaders.clear();
            designData.clear();
            editor->activeItem = editor->activeItem->parent();

        }else if(xmlReader->name().toString() == TAG_TYPE_TEST_COMBINATION){
            designData.prepend(tableColumnHeaders);
            editor->activeItem->setData(2,Qt::UserRole,designData);
            tableColumnHeaders.clear();
            designData.clear();
            editor->activeItem = editor->activeItem->parent();

        }else if(xmlReader->name().toString() == TAG_TYPE_TEST_SEQUENCE){
            setActiveItemSequenceDesign();
            editor->activeItem = editor->activeItem->parent();

        }else if(xmlReader->name().toString() == TAG_TYPE_TEST_CROSSCHECK){

            if ((tableColumnHeaders.size()>0) && (tableRowHeaders.size()>0) && (tableHeaderParents.size()>0)){
                designData.prepend(tableColumnHeaders);
                designData.prepend(tableRowHeaders);
                designData.prepend(tableHeaderParents);
                editor->activeItem->setData(2,Qt::UserRole,designData);
                tableRowHeaders.clear();
                tableColumnHeaders.clear();
                designData.clear();
            }

            editor->activeItem = editor->activeItem->parent();

        }else if (xmlReader->name().toString() == TAG_TYPE_TEST_COLLECTION_COLUMN){
            designData.append(tableColumn);
            tableColumn.clear();

        }else if (xmlReader->name().toString() == TAG_TYPE_TEST_COMBINATION_COLUMN){
            designData.append(tableColumn);
            tableColumn.clear();

        }else if(xmlReader->name().toString() == TAG_TYPE_TEST_SEQUENCE_STATE){
            shapePropertiesBuffer.text = lastTextBuffer;
            addToDiagramPropertiesBuffer(shapePropertiesBuffer);

        }else if(xmlReader->name().toString() == TAG_TYPE_TEST_SEQUENCE_TRANSITION){
            connectorPropertiesBuffer.text = lastTextBuffer;
            addToDiagramPropertiesBuffer(connectorPropertiesBuffer);

        }else if(xmlReader->name().toString() == TAG_TYPE_TEST_CROSSCHECK_COLUMN){
            designData.append(tableColumn);
            tableColumn.clear();

        }

    }else if ( purpose == treeOpener::toCompare){

        if (isComparableTag(xmlReader->name().toString())){
                    xPathParts.removeLast();
        }

    }
}
//----------------------------------------------------------------------------------------------------

bool treeOpener::isComparableTag(QString tagValue)
{
    if (tagValue == QString(TAG_TYPE_COMPONENT) ||
             tagValue == QString(TAG_TYPE_MODULE) ||
             tagValue == QString(TAG_TYPE_ENTRY) ||
             tagValue == QString(TAG_TYPE_TEST_GROUP) ||
             tagValue == QString(TAG_TYPE_TEST_COLLECTION) ||
             tagValue == QString(TAG_TYPE_TEST_COMBINATION) ||
             tagValue == QString(TAG_TYPE_TEST_SEQUENCE) ||
             tagValue == QString(TAG_TYPE_TEST_CROSSCHECK)){
                return true;

    }else{
        return false;

    }
}
//----------------------------------------------------------------------------------------------------

bool treeOpener::openSelected(callPurpose purpose)
{

    QFile *file = openFileDialog();

    if (file != 0 ){
        fileChosen = file;
        xmlReader = new QXmlStreamReader(file);

        while(!xmlReader->atEnd()){
            processElement(xmlReader->readNext(), purpose);

        }

        if (xmlReader->hasError()){
            QMessageBox::information(treeWidget->window(),STRING_TESTCAD,xmlReader->errorString());

        }else{
            return true;

        }

    }

    return false;
}
//----------------------------------------------------------------------------------------------------

bool treeOpener::fileOpened()
{
    if(!selectFile())
            return false;

    return openSelected(treeOpener::toLoad);
}
//----------------------------------------------------------------------------------------------------

void treeOpener::processElement(int tokenType, callPurpose purpose)
{

    if (tokenType == QXmlStreamReader::StartElement){

        if(xmlReader->name() == TAG_SYSTEM){
            rootTagFound = true;

        }else{

            if (rootTagFound){
                 processStart(purpose);

            }
        }
    }

    if (tokenType == QXmlStreamReader::EndElement){
        processEnd(purpose);

    }

}
//----------------------------------------------------------------------------------------------------

void treeOpener::loadStateProperties()
{
    shapePropertiesBuffer.ID = xmlReader->attributes().value(TAG_ATTRIBUTE_TEST_SEQUENCE_ID).toInt();
    shapePropertiesBuffer.position.setX(xmlReader->attributes().value(TAG_ATTRIBUTE_TEST_SEQUENCE_STATE_XPOSITION).toFloat());
    shapePropertiesBuffer.position.setY(xmlReader->attributes().value(TAG_ATTRIBUTE_TEST_SEQUENCE_STATE_YPOSITION).toFloat());
    shapePropertiesBuffer.shapeType = xmlReader->attributes().value(TAG_ATTRIBUTE_TEST_SEQUENCE_STATE_TYPE).toInt();
    shapePropertiesBuffer.textBoxPosition.setX(xmlReader->attributes().value(TAG_ATTRIBUTE_TEST_XTEXTBOXPOSITION).toFloat());
    shapePropertiesBuffer.textBoxPosition.setY(xmlReader->attributes().value(TAG_ATTRIBUTE_TEST_YTEXTBOXPOSITION).toFloat());
}
//----------------------------------------------------------------------------------------------------

void treeOpener::loadTransitionProperties()
{
    connectorPropertiesBuffer.ID = xmlReader->attributes().value(TAG_ATTRIBUTE_TEST_SEQUENCE_ID).toInt();
    connectorPropertiesBuffer.startPoint.setX(xmlReader->attributes().value(TAG_ATTRIBUTE_TEST_SEQUENCE_TRANSITION_XSTART_POINT).toFloat());
    connectorPropertiesBuffer.startPoint.setY(xmlReader->attributes().value(TAG_ATTRIBUTE_TEST_SEQUENCE_TRANSITION_YSTART_POINT).toFloat());
    connectorPropertiesBuffer.endPoint.setX(xmlReader->attributes().value(TAG_ATTRIBUTE_TEST_SEQUENCE_TRANSITION_XEND_POINT).toFloat());
    connectorPropertiesBuffer.endPoint.setY(xmlReader->attributes().value(TAG_ATTRIBUTE_TEST_SEQUENCE_TRANSITION_YEND_POINT).toFloat());
    connectorPropertiesBuffer.fromHookPointIndex = xmlReader->attributes().value(TAG_ATTRIBUTE_TEST_SEQUENCE_TRANSITION_FROM_HOOK_INDEX).toInt();
    connectorPropertiesBuffer.toHookPointIndex = xmlReader->attributes().value(TAG_ATTRIBUTE_TEST_SEQUENCE_TRANSITION_TO_HOOK_INDEX).toInt();
    connectorPropertiesBuffer.bezierFromPosition.setX(xmlReader->attributes().value(TAG_ATTRIBUTE_TEST_SEQUENCE_TRANSITION_XBEZIER_FROM).toFloat());
    connectorPropertiesBuffer.bezierFromPosition.setY(xmlReader->attributes().value(TAG_ATTRIBUTE_TEST_SEQUENCE_TRANSITION_YBEZIER_FROM).toFloat());
    connectorPropertiesBuffer.bezierToPosition.setX(xmlReader->attributes().value(TAG_ATTRIBUTE_TEST_SEQUENCE_TRANSITION_XBEZIER_TO).toFloat());
    connectorPropertiesBuffer.bezierToPosition.setY(xmlReader->attributes().value(TAG_ATTRIBUTE_TEST_SEQUENCE_TRANSITION_YBEZIER_TO).toFloat());
    connectorPropertiesBuffer.fromID = xmlReader->attributes().value(TAG_ATTRIBUTE_TEST_SEQUENCE_TRANSITION_FROM_ID).toInt();
    connectorPropertiesBuffer.toID = xmlReader->attributes().value(TAG_ATTRIBUTE_TEST_SEQUENCE_TRANSITION_TO_ID).toInt();
    connectorPropertiesBuffer.textBoxPosition.setX(xmlReader->attributes().value(TAG_ATTRIBUTE_TEST_XTEXTBOXPOSITION).toFloat());
    connectorPropertiesBuffer.textBoxPosition.setY(xmlReader->attributes().value(TAG_ATTRIBUTE_TEST_YTEXTBOXPOSITION).toFloat());
}
//----------------------------------------------------------------------------------------------------

void treeOpener::addToDiagramPropertiesBuffer(shapeProperties stateProperties)
{
    QVariant sP = sequencerWindow::propertiesToVariant(stateProperties);
    allShapesProperties.append(sP);

}
//----------------------------------------------------------------------------------------------------

void treeOpener::addToDiagramPropertiesBuffer(connectorProperties transitionProperties)
{
    allConnectorsProperties.append(sequencerWindow::propertiesToVariant(transitionProperties));

}
//----------------------------------------------------------------------------------------------------

void treeOpener::setActiveItemSequenceDesign()
{
    QList<QVariant> sequenceStructure;
    sequenceStructure.append(QVariant::fromValue(allShapesProperties));
    sequenceStructure.append(QVariant::fromValue(allConnectorsProperties));

    editor->activeItem->setData(2,Qt::UserRole,sequenceStructure);

    allShapesProperties.clear();
    allConnectorsProperties.clear();
}
//----------------------------------------------------------------------------------------------------

bool treeOpener::selectFile()
{
    filePath = QFileDialog::getOpenFileName(parentWidget, QObject::tr(STRING_OPEN_DIALOG_TITLE),
                                         QDir::currentPath(),
                                         QObject::tr(STRING_FILETYPE_TESTCAD));
    if (filePath.isEmpty()){
        return false;
    }

    return true;
}
//----------------------------------------------------------------------------------------------------

bool treeOpener::selectToCompare()
{
    if(!selectFile())
            return false;

    return openSelected(treeOpener::toCompare);
}
//----------------------------------------------------------------------------------------------------

QString treeOpener::getXPath(QList<QString> xPathParts)
{
    QString buffer;
    QList<QString>::iterator it;

    for(it = xPathParts.begin(); it != xPathParts.end(); ++it){
        buffer = buffer + (*it);

        if(it < xPathParts.end() - 1){
            buffer = buffer +"/";

        }
    }

    return buffer;
}
