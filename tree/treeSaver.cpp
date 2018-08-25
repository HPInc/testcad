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

#include "treeSaver.h"

treeSaver::treeSaver(QTreeWidget *treeWidget, QWidget *parent)
    : treeWidget(treeWidget)
{
    parentWidget = parent;

}
//----------------------------------------------------------------------------------------------------

void treeSaver::addCrossCheckDesignToStream(QTreeWidgetItem *item)
{
    QList <QVariant> designData = item->data(2,Qt::UserRole).toList();

    if (designData.size()>0){
        QVariantList headerParents = designData.at(0).toList();

        if(headerParents.size()==2){
            xmlWriter->writeStartElement(TAG_TYPE_TEST_CROSSCHECK_PARENTS);
                xmlWriter->writeAttribute(TAG_ATTRIBUTE_CROSSCHECK_ROW_PARENT,headerParents.at(0).toString());
                xmlWriter->writeAttribute(TAG_ATTRIBUTE_CROSSCHECK_COLUMN_PARENT,headerParents.at(1).toString());
            xmlWriter->writeEndElement();
        }

        QVariantList rowHeaders = designData.at(1).toList();
        QVariantList columnHeaders = designData.at(2).toList();
        int rowCount = 0;

        for (int i = 3; i < designData.size(); i++){

            if(columnHeaders.size()>0 && rowHeaders.size()>0 ){
                xmlWriter->writeStartElement(TAG_TYPE_TEST_CROSSCHECK_COLUMN);
                xmlWriter->writeAttribute(TAG_ATTRIBUTE_CROSSCHECK_COLUMN_HEADER, columnHeaders.at(i - 3).toString());
                QVariantList columnBuff = designData.at(i).toList();

                for (int j = 0; j < columnBuff.size(); j++){
                    xmlWriter->writeTextElement(TAG_TYPE_TEST_CROSSCHECK_CELL, columnBuff.at(j).toString());

                }

                xmlWriter->writeEndElement();
                if (i == 3) rowCount = columnBuff.size();
            }
        }

        for (int i = 0; i < rowCount; i++){
            xmlWriter->writeTextElement(TAG_TYPE_TEST_CROSSCHECK_ROW_HEADER, rowHeaders.at(i).toString());

        }
    }
}
//----------------------------------------------------------------------------------------------------

void treeSaver::addCollectionDesignToStream(QTreeWidgetItem *item)
{
    QList <QVariant> designData = item->data(2,Qt::UserRole).toList();

    if (designData.size()>0){
        QVariantList columnHeaders = designData.at(0).toList();

        for (int i = 1; i < designData.size(); i++){
            xmlWriter->writeStartElement(TAG_TYPE_TEST_COLLECTION_COLUMN);
            xmlWriter->writeAttribute(TAG_ATTRIBUTE_TEST_COLLECTION_COLUMN_HEADER, columnHeaders.at(i - 1).toString());
            QVariantList columnBuff = designData.at(i).toList();

            for (int j = 0; j < columnBuff.size(); j++){
                xmlWriter->writeStartElement(TAG_TYPE_TEST_COLLECTION_TABLE_COLUMN_CELL);
                xmlWriter->writeAttribute(TAG_ATTRIBUTE_TEST_COLLECTION_CELL_VALUE, columnBuff.at(j).toString());
                xmlWriter->writeEndElement();

            }

            xmlWriter->writeEndElement();
        }
    }

}
//----------------------------------------------------------------------------------------------------

void treeSaver::addCombinationDesignToStream(QTreeWidgetItem *item)
{
    QList <QVariant> designData = item->data(2,Qt::UserRole).toList();

    if (designData.size()>0){
        QVariantList columnHeaders = designData.at(0).toList();

        for (int i = 1; i < designData.size(); i++){
            xmlWriter->writeStartElement(TAG_TYPE_TEST_COMBINATION_COLUMN);
            xmlWriter->writeAttribute(TAG_ATTRIBUTE_TEST_COMBINATION_COLUMN_HEADER, columnHeaders.at(i - 1).toString());
            QVariantList columnBuff = designData.at(i).toList();

            for (int j = 0; j < columnBuff.size(); j++){
                xmlWriter->writeStartElement(TAG_TYPE_TEST_COMBINATION_TABLE_COLUMN_CELL);
                xmlWriter->writeAttribute(TAG_ATTRIBUTE_TEST_COMBINATION_CELL_VALUE, columnBuff.at(j).toString());
                xmlWriter->writeEndElement();

            }

            xmlWriter->writeEndElement();
        }
    }

}
//----------------------------------------------------------------------------------------------------

void treeSaver::addSequenceDesignToStream(QTreeWidgetItem *item)
{
    diagramStructure diagram = sequencerWindow::structureFromItem(item);

    if (diagram.shapesInfo.size()>0){
        shapeProperties shPs;
        connectorProperties cnnPs;

        for (int i = 0; i < diagram.shapesInfo.size(); i++){
            shPs = diagram.shapesInfo.at(i);
            xmlWriter->writeStartElement(TAG_TYPE_TEST_SEQUENCE_STATE);
            xmlWriter->writeAttribute(TAG_ATTRIBUTE_TEST_SEQUENCE_ID, QString::number(shPs.ID));
            xmlWriter->writeAttribute(TAG_ATTRIBUTE_TEST_SEQUENCE_STATE_XPOSITION,QString::number(shPs.position.x()));
            xmlWriter->writeAttribute(TAG_ATTRIBUTE_TEST_SEQUENCE_STATE_YPOSITION,QString::number(shPs.position.y()));
            xmlWriter->writeAttribute(TAG_ATTRIBUTE_TEST_SEQUENCE_STATE_TYPE,QString::number(shPs.shapeType));
            xmlWriter->writeAttribute(TAG_ATTRIBUTE_TEST_XTEXTBOXPOSITION,QString::number(shPs.textBoxPosition.x()));
            xmlWriter->writeAttribute(TAG_ATTRIBUTE_TEST_YTEXTBOXPOSITION,QString::number(shPs.textBoxPosition.y()));
            xmlWriter->writeTextElement(TAG_TYPE_TEXT,shPs.text);
            xmlWriter->writeEndElement();
        }

        for (int i = 0; i < diagram.connectorsInfo.size(); i++){
            cnnPs = diagram.connectorsInfo.at(i);
            xmlWriter->writeStartElement(TAG_TYPE_TEST_SEQUENCE_TRANSITION);
            xmlWriter->writeAttribute(TAG_ATTRIBUTE_TEST_SEQUENCE_ID, QString::number(cnnPs.ID));
            xmlWriter->writeAttribute(TAG_ATTRIBUTE_TEST_SEQUENCE_TRANSITION_XSTART_POINT, QString::number(cnnPs.startPoint.x()));
            xmlWriter->writeAttribute(TAG_ATTRIBUTE_TEST_SEQUENCE_TRANSITION_YSTART_POINT, QString::number(cnnPs.startPoint.y()));
            xmlWriter->writeAttribute(TAG_ATTRIBUTE_TEST_SEQUENCE_TRANSITION_XEND_POINT, QString::number(cnnPs.endPoint.x()));
            xmlWriter->writeAttribute(TAG_ATTRIBUTE_TEST_SEQUENCE_TRANSITION_YEND_POINT, QString::number(cnnPs.endPoint.y()));
            xmlWriter->writeAttribute(TAG_ATTRIBUTE_TEST_SEQUENCE_TRANSITION_XBEZIER_FROM, QString::number(cnnPs.bezierFromPosition.x()));
            xmlWriter->writeAttribute(TAG_ATTRIBUTE_TEST_SEQUENCE_TRANSITION_YBEZIER_FROM, QString::number(cnnPs.bezierFromPosition.y()));
            xmlWriter->writeAttribute(TAG_ATTRIBUTE_TEST_SEQUENCE_TRANSITION_XBEZIER_TO, QString::number(cnnPs.bezierToPosition.x()));
            xmlWriter->writeAttribute(TAG_ATTRIBUTE_TEST_SEQUENCE_TRANSITION_YBEZIER_TO, QString::number(cnnPs.bezierToPosition.y()));
            xmlWriter->writeAttribute(TAG_ATTRIBUTE_TEST_SEQUENCE_TRANSITION_FROM_HOOK_INDEX, QString::number(cnnPs.fromHookPointIndex));
            xmlWriter->writeAttribute(TAG_ATTRIBUTE_TEST_SEQUENCE_TRANSITION_TO_HOOK_INDEX, QString::number(cnnPs.toHookPointIndex));
            xmlWriter->writeAttribute(TAG_ATTRIBUTE_TEST_SEQUENCE_TRANSITION_FROM_ID, QString::number(cnnPs.fromID));
            xmlWriter->writeAttribute(TAG_ATTRIBUTE_TEST_SEQUENCE_TRANSITION_TO_ID, QString::number(cnnPs.toID));
            xmlWriter->writeAttribute(TAG_ATTRIBUTE_TEST_XTEXTBOXPOSITION, QString::number(cnnPs.textBoxPosition.x()));
            xmlWriter->writeAttribute(TAG_ATTRIBUTE_TEST_YTEXTBOXPOSITION, QString::number(cnnPs.textBoxPosition.y()));
            xmlWriter->writeTextElement(TAG_TYPE_TEXT, cnnPs.text);
            xmlWriter->writeEndElement();
        }
    }
}
//----------------------------------------------------------------------------------------------------

bool treeSaver::exportTreeToImage()
{
    QString imageFileName = QFileDialog::getSaveFileName(parentWidget, QObject::tr(STRING_SAVE_TESTCAD_FILE),
                                         QDir::currentPath(),
                                         QObject::tr(STRING_PNG_IMAGE_FILE));
    if (imageFileName.isEmpty())
        return false;

    if (imageFileName.indexOf("png",0,Qt::CaseInsensitive) < 2){
        imageFileName = imageFileName + ".png";

    }

    QPixmap pixmap(treeWidget->size());
    treeWidget->render(&pixmap);

    return pixmap.save(imageFileName,"PNG");
}
//----------------------------------------------------------------------------------------------------

bool treeSaver::saveTreeAs()
{
    filePath = QFileDialog::getSaveFileName(parentWidget, QObject::tr(STRING_SAVE_TESTCAD_FILE),
                                         QDir::currentPath(),
                                         QObject::tr(STRING_FILETYPE_TESTCAD));
    if (filePath.isEmpty())
        return false;

    return saveTreeTo(filePath);
}
//----------------------------------------------------------------------------------------------------

bool treeSaver::saveTreeTo(QString fileName)
{
    if (fileName.isEmpty())
        return false;

    if (!fileName.endsWith(TESTCAD_FILE_EXTENSION)){
        fileName = fileName + TESTCAD_FILE_EXTENSION;

    }

    QFile file(fileName);

    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(parentWidget,STRING_TESTCAD,
                             QObject::tr(STRING_CANNOT_WRITE_FILE)
                             .arg(fileName)
                             .arg(file.errorString()));

    }else{

        if(writeTreeTo(&file)){
            filePath = fileName;
            return true;

        }

    }

    return false;

}
//----------------------------------------------------------------------------------------------------

bool treeSaver::writeTreeTo(QIODevice *device)
{
    xmlWriter = new QXmlStreamWriter(device);
    xmlWriter->setAutoFormatting(true);
    xmlWriter->writeStartDocument();
    xmlWriter->writeStartElement(TAG_SYSTEM);
    xmlWriter->writeAttribute("version", STRING_VERSION_NUMBER);

    for (int i = 0; i < treeWidget->topLevelItemCount(); i++){
        addItemToStream(treeWidget->topLevelItem(i));

    }

    xmlWriter->writeEndElement();
    xmlWriter->writeEndDocument();
    bool hasError = xmlWriter->hasError();
    delete xmlWriter;

    return !hasError;
}
//----------------------------------------------------------------------------------------------------

void treeSaver::addItemToStream(QTreeWidgetItem *item)
{
    QString tagName = item->data(0, Qt::UserRole).toString();
    xmlWriter->writeStartElement(tagName);
    xmlWriter->writeAttribute(TAG_ATTRIBUTE_NAME, item->text(0));

    QString statusValue = item->data(1, Qt::UserRole).toString();
    xmlWriter->writeAttribute(TAG_ATTRIBUTE_STATUS, statusValue);

    bool isFolded = !treeWidget->isItemExpanded(item);
    xmlWriter->writeAttribute(TAG_ATTRIBUTE_FOLDED, (isFolded ? STRING_YES : STRING_NO));
    xmlWriter->writeTextElement(TAG_COMMENT,item->text(2));

    if (tagName == TAG_TYPE_TEST_COLLECTION){
        addCollectionDesignToStream(item);

    }else if (tagName == TAG_TYPE_TEST_COMBINATION){
        addCombinationDesignToStream(item);

    }else if (tagName == TAG_TYPE_TEST_SEQUENCE){
        addSequenceDesignToStream(item);

    }else if (tagName == TAG_TYPE_TEST_CROSSCHECK){
        addCrossCheckDesignToStream(item);

    }

    for (int i = 0; i < item->childCount(); i++){
        addItemToStream(item->child(i));

    }

    xmlWriter->writeEndElement();
}

