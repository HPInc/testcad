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

#include "diagram.h"

diagram::diagram(QGraphicsView *view, int lastID)
{
    this->nextID = lastID + 1;
    this->newConnector = 0;
    this->currentStatus = diagramStatus::connectionEstablished;
    viewer = view;

}
//--------------------------------------------------------------------------------------------------------------------------------

void diagram::findAndHighlight(QString textToFind)
{
    diagramStructure connections;
    QList<QGraphicsItem*>::iterator it;
    QList<QGraphicsItem*> allItems = this->items();
    QGraphicsItem *item;
    stateShape *shp;
    connector *cnn;
    QString strBuff;

    for(it = allItems.begin(); it != allItems.end(); ++it){
        item = *it;
        cnn = dynamic_cast<connector*>(item);

        if(cnn){
            strBuff = cnn->properties.text;

            if (strBuff.contains(textToFind,Qt::CaseInsensitive)){
                cnn->setHighlighted(true);

            }else{
                cnn->setHighlighted(false);
            }

        }else{
            shp = dynamic_cast<stateShape*>(item);

            if(shp){
                strBuff = shp->properties.text;

                if (strBuff.contains(textToFind,Qt::CaseInsensitive)){
                    shp->fillColor = QColor(255,255,0);

                }else{
                    shp->fillColor = QColor(255,255,255);

                }
            }
        }
    }
}
//--------------------------------------------------------------------------------------------------------------------------------

void diagram::loadFromStructure(diagramStructure structure)
{
    shapeProperties shProps;
    connectorProperties cnProps;
    nextID = 0;
    QPointF topLeft = QPointF(0,0);
    QPointF bottomRight = QPointF(0,0);
    int margin = 50;

    for (int i = 0; i < structure.shapesInfo.size(); i++){
        shProps = structure.shapesInfo.at(i);
        stateShape* newShape = new stateShape(shProps);
        this->addItem(newShape);

        if (shProps.position.x() < topLeft.x())
            topLeft.setX(shProps.position.x());

        if (shProps.position.y() < topLeft.y())
            topLeft.setY(shProps.position.y());

        if (shProps.position.x() > bottomRight.x())
            bottomRight.setX(shProps.position.x());

        if (shProps.position.y() > bottomRight.y())
            bottomRight.setY(shProps.position.y());

        if ((shProps.ID > nextID) || (shProps.ID == nextID)){
            nextID = shProps.ID + 1;

        }
    }

    for (int i = 0; i < structure.connectorsInfo.size(); i++){
        cnProps = structure.connectorsInfo.at(i);
        newConnector = new connector(cnProps);
        this->addItem(newConnector);
        newConnector = 0;
        currentStatus = diagramStatus::connectionEstablished;

        if ((cnProps.ID > nextID) || (cnProps.ID == nextID)){
            nextID = cnProps.ID + 1;

        }
    }

    topLeft.setX(topLeft.x() - margin);
    topLeft.setY(topLeft.y() - margin);
    bottomRight.setX(bottomRight.x() + 2*margin);
    bottomRight.setY(bottomRight.y() + 2*margin);

    fitViewerTo(QRectF(topLeft,bottomRight));

}
//--------------------------------------------------------------------------------------------------------------------------------

void diagram::fitViewerTo(QRectF fitArea)
{
    this->setSceneRect(fitArea);
    this->update(fitArea);
    viewer->fitInView(fitArea,Qt::KeepAspectRatio);

}
//--------------------------------------------------------------------------------------------------------------------------------

void diagram::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    mousePosition = mouseEvent->scenePos();
    redrawDiagram();
    update(this->itemsBoundingRect());
    QGraphicsScene::mouseMoveEvent(mouseEvent);

}
//--------------------------------------------------------------------------------------------------------------------------------

void diagram::redrawDiagram()
{
    QList<QGraphicsItem*> sceneItems = this->items();
    QList<QGraphicsItem*>::iterator shapesIterator;
    QGraphicsItem* item;
    stateShape* shapeItem;

    for (shapesIterator = sceneItems.begin(); shapesIterator != sceneItems.end(); ++shapesIterator){
        item = *shapesIterator;
        shapeItem = dynamic_cast<stateShape*> (item);

        if (shapeItem){
            redrawConnectors(shapeItem->properties);

        }
    }

}
//--------------------------------------------------------------------------------------------------------------------------------

bool diagram::saveImage(QString imagePath)
{
    if (imagePath != ""){
        this->clearSelection();
        this->setSceneRect(this->itemsBoundingRect());
        QImage img(this->sceneRect().size().toSize(), QImage::Format_RGB32);
        img.fill(Qt::white);

        QPainter painter(&img);
        this->render(&painter);

        if (img.save(imagePath)){
            return true;
        }
    }

    return false;
}
//--------------------------------------------------------------------------------------------------------------------------------

bool diagram::isValid()
{
    diagramRules rules = checkRules();

    if(rules.hasInitialState &&
            rules.hasUniqueInitialState &&
            rules.initialStateHasNoInboundTransition &&
            rules.initialStateHasOutboundTransition &&
            rules.initialStateHasUniqueOutboundTransition &&
            rules.hasIntermediateState &&
            rules.allStatesHaveInboundTransition &&
            rules.allStatesHaveOutboundTransition &&
            rules.hasFinalState &&
            rules.hasUniqueFinalState &&
            rules.finalStateHasInboundTransitions &&
            rules.finalStateHasNoOutboundTransition &&
            rules.allElementsHaveText){

        return true;

    }else{
        QMessageBox msg(QMessageBox::Critical, STRING_DESIGN, getRulesErrors(rules), QMessageBox::Ok);
        msg.exec();

    }

    return false;
}
//--------------------------------------------------------------------------------------------------------------------------------

QString diagram::getRulesErrors(diagramRules rules)
{
    QString output(STRING_DIAGRAM_ERRORS);

    if (!rules.hasInitialState)
            output.append(QString(STRING_ERROR_NO_INITIAL_STATE));

    else {
        if (!rules.initialStateHasNoInboundTransition)
                output.append(QString(STRING_ERROR_INITIAL_STATE_HAS_INBOUND_TRANSITION));

        if (!rules.initialStateHasOutboundTransition)
                output.append(QString(STRING_ERROR_INITIAL_STATE_HAS_NO_OUTBOUND_TRANSITION));

        if (!rules.initialStateHasUniqueOutboundTransition)
                output.append(QString(STRING_ERROR_INITIAL_STATE_HAS_NO_UNIQUE_OUTBOUND_TRANSITION));

        if (!rules.hasUniqueInitialState)
                output.append(QString(STRING_ERROR_MULTIPLE_INITIAL_STATES));
    }

    if (!rules.hasIntermediateState)
            output.append(QString(STRING_ERROR_NO_INTERMEDIATE_STATE));

    else {
        if (!rules.allStatesHaveInboundTransition)
                output.append(QString(STRING_ERROR_INTERMEDIATE_STATE_WITHOUT_INBOUND_TRANSITION));

        if (!rules.allStatesHaveOutboundTransition)
                output.append(QString(STRING_ERROR_INTERMEDIATE_STATE_WITHOUT_OUTBOUND_TRANSITION));
    }

    if (!rules.hasFinalState)
            output.append(QString(STRING_ERROR_NO_FINAL_STATE));
    else {
        if (!rules.hasUniqueFinalState)
                output.append(QString(STRING_ERROR_MULTIPLE_FINAL_STATES));

        if (!rules.finalStateHasInboundTransitions)
                output.append(QString(STRING_ERROR_FINAL_STATE_WITHOUT_INBOUND_TRANSITION));

        if (!rules.finalStateHasNoOutboundTransition)
                output.append(QString(STRING_ERROR_FINAL_STATE_WITH_OUTBOUND_TRANSITION));

        if (!rules.allElementsHaveText)
                output.append(QString(STRING_ERROR_ELEMENT_WITHOUT_TEXT));
    }

    output.append("</ul>");

    return output;
}
//--------------------------------------------------------------------------------------------------------------------------------

diagramRules diagram::checkRules()
{
    diagramRules rules;
    structureCache = getStructure();

    for (int i = 0; i < structureCache.shapesInfo.size(); i++){

        if (structureCache.shapesInfo.at(i).text.isEmpty())
            rules.allElementsHaveText = false;

        if (structureCache.shapesInfo.at(i).shapeType == shapeTypes::initialState){
            rules.hasInitialState = true;
            rules.initialStatesCount++;

            if (countInboundTransitions(structureCache.shapesInfo.at(i).ID) > 0)
                rules.initialStateHasNoInboundTransition = false;

            if (countOutboundTransitions(structureCache.shapesInfo.at(i).ID) > 0)
                rules.initialStateHasOutboundTransition = true;

            if (countOutboundTransitions(structureCache.shapesInfo.at(i).ID) > 1)
                rules.initialStateHasUniqueOutboundTransition = false;

        }else if (structureCache.shapesInfo.at(i).shapeType == shapeTypes::finalState){
            rules.hasFinalState = true;
            rules.finalStatesCount++;

            if (countInboundTransitions(structureCache.shapesInfo.at(i).ID) > 0)
                rules.finalStateHasInboundTransitions = true;

            if (countOutboundTransitions(structureCache.shapesInfo.at(i).ID) > 0)
                rules.finalStateHasNoOutboundTransition = false;

        }else{
            rules.hasIntermediateState = true;

            if (countInboundTransitions(structureCache.shapesInfo.at(i).ID) == 0)
                rules.allStatesHaveInboundTransition = false;

            if (countOutboundTransitions(structureCache.shapesInfo.at(i).ID) == 0)
                rules.allStatesHaveOutboundTransition = false;

        }

    }

    for (int i = 0; i < structureCache.connectorsInfo.size(); i++){

        if (structureCache.connectorsInfo.at(i).text.isEmpty())
            rules.allElementsHaveText = false;

    }

    if (rules.initialStatesCount == 1)
        rules.hasUniqueInitialState = true;

    if (rules.finalStatesCount == 1)
        rules.hasUniqueFinalState = true;

    return rules;
}
//--------------------------------------------------------------------------------------------------------------------------------

int diagram::countInboundTransitions(int stateID)
{
    int out = 0;

    for (int i = 0; i < structureCache.connectorsInfo.size(); i++){

        if ((structureCache.connectorsInfo.at(i).toID == stateID) &&
            (structureCache.connectorsInfo.at(i).fromID != stateID)){
            out++;

        }
    }

    return out;
}
//--------------------------------------------------------------------------------------------------------------------------------

int diagram::countOutboundTransitions(int stateID)
{
    int out = 0;

    for (int i = 0; i < structureCache.connectorsInfo.size(); i++){

        if ((structureCache.connectorsInfo.at(i).fromID == stateID) &&
            (structureCache.connectorsInfo.at(i).toID != stateID)){
            out++;

        }
    }

    return out;
}
//--------------------------------------------------------------------------------------------------------------------------------

stateShape* diagram::getShapeByID(int shapeID)
{
    QList<QGraphicsItem*>::iterator it;
    QList<QGraphicsItem*> allItems = this->items();
    QGraphicsItem *item;
    stateShape *shp;

    for(it = allItems.begin(); it != allItems.end(); ++it){
        item = *it;
        shp = dynamic_cast<stateShape*>(item);

        if(shp){
            if (shp->properties.ID == shapeID){
                return shp;

            }
        }
    }

    return 0;
}
//--------------------------------------------------------------------------------------------------------------------------------

diagramStructure diagram::getStructure()
{
    diagramStructure connections;
    QList<QGraphicsItem*>::iterator it;
    QList<QGraphicsItem*> allItems = this->items();
    QGraphicsItem *item;
    stateShape *shp;
    connector *cnn;

    for(it = allItems.begin(); it != allItems.end(); ++it){
        item = *it;
        cnn = dynamic_cast<connector*>(item);

        if(cnn){
            connections.connectorsInfo.append(cnn->properties);

        }else{
            shp = dynamic_cast<stateShape*>(item);

            if (shp){
                connections.shapesInfo.append(shp->properties);

            }
        }
    }

    return connections;
}
//--------------------------------------------------------------------------------------------------------------------------------

void diagram::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    setDiagramConnectionStatus();
    lastClickPoint = mouseEvent->scenePos();
    hasChanged = true;
    QGraphicsScene::mouseReleaseEvent(mouseEvent);

}
//--------------------------------------------------------------------------------------------------------------------------------

void diagram::addShape(int shapeType, QPointF insertionPoint)
{
    stateShape* newshape = new stateShape(insertionPoint, nextID, STRING_DEFAULT_LABEL_STATE, shapeType);
    newshape->properties.ID = nextID;
    nextID++;
    this->addItem(newshape);
    hasChanged = true;

}
//--------------------------------------------------------------------------------------------------------------------------------

void diagram::connectStates()
{
    this->currentStatus = diagramStatus::connectionRequested;

}
//--------------------------------------------------------------------------------------------------------------------------------

void diagram::addInitialState()
{
    this->addShape(shapeTypes::initialState, lastClickPoint);

}
//--------------------------------------------------------------------------------------------------------------------------------

void diagram::addState()
{
    this->addShape(shapeTypes::State, lastClickPoint);

}
//--------------------------------------------------------------------------------------------------------------------------------

void diagram::addFinalState()
{
    this->addShape(shapeTypes::finalState, lastClickPoint);

}
//--------------------------------------------------------------------------------------------------------------------------------

void diagram::clearDiagram()
{
    QList<QGraphicsItem*>::iterator it;
    QList<QGraphicsItem*> allItems = this->items();
    QGraphicsItem *item;

    for(it = allItems.begin(); it != allItems.end(); ++it){
        item = *it;
        this->removeItem(item);

    }

}
//--------------------------------------------------------------------------------------------------------------------------------

void diagram::deleteSelected()
{
    QList<QGraphicsItem*>::iterator selectedIterator;
    QList<QGraphicsItem*> selecteItems = this->selectedItems();
    QGraphicsItem *item;
    stateShape *selectedShape;
    connector *selectedConnector;

    if (newConnector != 0){
        this->removeItem(newConnector);

    }else{

        for(selectedIterator = selecteItems.begin(); selectedIterator != selecteItems.end(); ++selectedIterator){
            item = *selectedIterator;
            selectedShape = dynamic_cast<stateShape*>(item);

            if(selectedShape){
                deleteAssociatedConnectors(selectedShape->properties);
                this->removeItem(item);

            }else{
                selectedConnector = dynamic_cast<connector*>(item);

                if(selectedConnector){
                    this->removeItem(item);

                }
            }
        }

        hasChanged = true;
    }
}
//--------------------------------------------------------------------------------------------------------------------------------

void diagram::deleteAssociatedConnectors(shapeProperties sProperties)
{
    QList<QGraphicsItem*> sceneItems = this->items();
    QList<QGraphicsItem*>::iterator connectorsIterator;
    QGraphicsItem* item;
    connector* connectorItem;

    for (connectorsIterator = sceneItems.begin(); connectorsIterator != sceneItems.end(); ++connectorsIterator){
        item = *connectorsIterator;
        connectorItem = dynamic_cast<connector*> (item);

        if (connectorItem){

            if ((connectorItem->properties.fromID == sProperties.ID) ||
                    (connectorItem->properties.toID == sProperties.ID)){
                currentStatus = diagramStatus::connectionEstablished; //Force state avoid further status processing
                this->removeItem(item);
                newConnector = 0;

            }
        }
    }
}
//--------------------------------------------------------------------------------------------------------------------------------

void diagram::setDiagramConnectionStatus()
{
    stateShape* shapeItem = this->getSelectedShape();

    if(shapeItem){

        if (this->currentStatus == diagramStatus::connectionRequested){

            if (shapeItem->properties.selectedHookPointIndex != -1){
                beginConnection(shapeItem->properties);
                currentStatus = diagramStatus::connectionInProgress;

            }

        }else if (this->currentStatus == diagramStatus::connectionInProgress){

            if (shapeItem->properties.selectedHookPointIndex != -1){

                if (glueConnection(shapeItem->properties)){
                    currentStatus = diagramStatus::connectionEstablished;
                    hasChanged = true;

                }
            }
        }
    }
}
//--------------------------------------------------------------------------------------------------------------------------------

void diagram::beginConnection(shapeProperties fromshape)
{
    QPointF startPoint;
    startPoint = fromshape.selectedHookPoint;
    QPointF endPoint;
    endPoint = this->mousePosition;

    newConnector = new connector(startPoint, endPoint, nextID, STRING_DEFAULT_LABEL_EVENT);
    newConnector->properties.fromID = fromshape.ID;
    newConnector->properties.fromHookPointIndex = fromshape.selectedHookPointIndex;
    newConnector->properties.toID = -1;
    newConnector->properties.toHookPointIndex = fromshape.selectedHookPointIndex + 2;//Force the excentricity calculation value

    this->addItem(newConnector);
    nextID++;

}
//--------------------------------------------------------------------------------------------------------------------------------

bool diagram::glueConnection(shapeProperties toshape)
{
    QPointF endPoint;
    endPoint = toshape.selectedHookPoint;
    bool isGlued = false;

    if(newConnector != 0){

        if ((newConnector->properties.fromID != toshape.ID) ||
            ((newConnector->properties.fromID == toshape.ID) && //Ensure to choose two different hookPoints
             (newConnector->properties.fromHookPointIndex != toshape.selectedHookPointIndex)))
        {
            newConnector->properties.toID = toshape.ID;

            if(newConnector->properties.toID > 0){ //Prevent from storing an invalid toshape.ID (investigating why it happens)
                newConnector->properties.toHookPointIndex = toshape.selectedHookPointIndex;
                newConnector->glueTo(endPoint);
                newConnector = 0;
                isGlued = true;

            }
        }
    }
    return isGlued;
}
//--------------------------------------------------------------------------------------------------------------------------------

void diagram::redrawConnectors(shapeProperties sProperties)
{
    QList<QGraphicsItem*> sceneItems = this->items();
    QList<QGraphicsItem*>::iterator connectorsIterator;
    QGraphicsItem* item;
    connector* connectorItem;

    for (connectorsIterator = sceneItems.begin();
         connectorsIterator != sceneItems.end();
         ++connectorsIterator){

        item = *connectorsIterator;
        connectorItem = dynamic_cast<connector*> (item);

        if (connectorItem){

            if (connectorItem->properties.fromID == sProperties.ID){
                connectorItem->properties.startPoint = sProperties.hookPoints[connectorItem->properties.fromHookPointIndex];

            }

            if (connectorItem->properties.toID == sProperties.ID){
                connectorItem->properties.endPoint = sProperties.hookPoints[connectorItem->properties.toHookPointIndex];

            }

            if (connectorItem->properties.toID == -1){
                connectorItem->properties.endPoint = this->mousePosition;

            }
        }
    }
}
//--------------------------------------------------------------------------------------------------------------------------------

stateShape* diagram::getSelectedShape()
{
    QList<QGraphicsItem*> sceneSelectedItems = this->selectedItems();
    QList<QGraphicsItem*>::iterator selectedItemsIterator;
    QGraphicsItem* selectedItem;
    stateShape* selectedShape;

    for (selectedItemsIterator = sceneSelectedItems.begin();
         selectedItemsIterator != sceneSelectedItems.end();
         ++selectedItemsIterator){

        selectedItem = *selectedItemsIterator;
        selectedShape = dynamic_cast<stateShape*> (selectedItem);

        if (selectedShape){
            return selectedShape;

        }
    }
    return NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------

diagram::~diagram()
{

}
