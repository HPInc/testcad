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

 #include "stateShape.h"

stateShape::stateShape(QPointF position, int ID, QString text, int shapeType)
{
    properties.ID = ID;
    properties.text = text;
    properties.position = position;
    properties.shapeType = shapeType;
    properties.textBoxPosition = QPointF(MARKER_SIZE * 6, MARKER_SIZE * 6);

    construct();

}
//--------------------------------------------------------------------------------------------------------------------------------

void stateShape::highlight()
{

}
//--------------------------------------------------------------------------------------------------------------------------------

stateShape::stateShape(shapeProperties values)
{
    properties = values;

    construct();
}
//--------------------------------------------------------------------------------------------------------------------------------

//Hook Points calculation for connectorProperties location
void stateShape::calculateHookCenters()
{
    QPointF PointN = QPointF(this->scenePos().rx() + shapeWidth/2, this->scenePos().ry());//N
    QPointF PointE = QPointF(this->scenePos().rx(), this->scenePos().ry() + shapeHeight/2);//E
    QPointF PointS = QPointF(this->scenePos().rx() + shapeWidth/2, this->scenePos().ry() + shapeHeight );//S
    QPointF PointW = QPointF(this->scenePos().rx() + shapeWidth, this->scenePos().ry() + shapeHeight/2);//W

    properties.hookPoints[0] = PointN;
    properties.hookPoints[1] = PointE;
    properties.hookPoints[2] = PointS;
    properties.hookPoints[3] = PointW;

}
//--------------------------------------------------------------------------------------------------------------------------------

void stateShape::construct()
{
    fillColor = QColor(255,255,255);
    this->setPos(properties.position);

    if(properties.shapeType == shapeTypes::State)
    {
        shapeText = new textBox(properties.text, QPointF(MARKER_SIZE, MARKER_SIZE), this);
        shapeFloatingText = 0;
        properties.textBoxPosition = QPointF(0,0);

    }else if((properties.shapeType == shapeTypes::initialState) || (properties.shapeType == shapeTypes::finalState)){
        shapeFloatingText = new floatingTextBox(properties.text, properties.textBoxPosition, this);
        shapeText = 0;

    }

    halfMarkerSize = MARKER_SIZE/2;
    setFlags( ItemIsSelectable | ItemIsMovable | ItemSendsGeometryChanges | ItemIsFocusable );
    setAcceptHoverEvents(true);
    calculateHookCenters();
}

//--------------------------------------------------------------------------------------------------------------------------------

stateShape::~stateShape()
{
    delete shapeText;
    delete shapeFloatingText;

}
//--------------------------------------------------------------------------------------------------------------------------------

void stateShape::drawHookMarkers(QPainter *painter, const QStyleOptionGraphicsItem *option)
{
    if(option->state & QStyle::State_MouseOver){
        QPen pen;
        pen.setWidth(2);
        pen.setColor(QColor(0,255,0));
        painter->setPen(pen);
        painter->setBrush(Qt::NoBrush);

        //Draw the hookPoints
        painter->drawRect(shapeWidth/2 - halfMarkerSize, - halfMarkerSize, MARKER_SIZE, MARKER_SIZE);//N
        painter->drawRect(- halfMarkerSize, shapeHeight/2 - halfMarkerSize, MARKER_SIZE, MARKER_SIZE);//W
        painter->drawRect(shapeWidth/2 - halfMarkerSize, shapeHeight - halfMarkerSize, MARKER_SIZE, MARKER_SIZE);//S
        painter->drawRect(shapeWidth - halfMarkerSize, shapeHeight/2 - halfMarkerSize, MARKER_SIZE, MARKER_SIZE);//E

    }
}
//--------------------------------------------------------------------------------------------------------------------------------

//Choose the nearest index for EndPoint based on the mouse click
int stateShape::getNearestHookPointIndex()
{
    QPointF endPointCoordinates;

    for(int index = 0; index < 4; index++){
        endPointCoordinates =  properties.hookPoints[index];
        int deltaX = qFabs(this->mousePositionInLocalCoordinates.x() - endPointCoordinates.x() + this->pos().rx());
        int deltaY = qFabs(this->mousePositionInLocalCoordinates.y() - endPointCoordinates.y() + this->pos().ry());

        if (((deltaX < halfMarkerSize) || (deltaX == halfMarkerSize)) &&
                ((deltaY < halfMarkerSize) || (deltaY == halfMarkerSize))){//Within the endpoint marker area
            this->properties.selectedHookPoint = properties.hookPoints[index];
            return index;

        }
    }
    return -1;
}
//--------------------------------------------------------------------------------------------------------------------------------

void stateShape::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    this->mousePositionInLocalCoordinates = event->pos();
    properties.selectedHookPointIndex = getNearestHookPointIndex();
    QGraphicsItem::mousePressEvent(event);

}
//--------------------------------------------------------------------------------------------------------------------------------

void stateShape::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);

    painter->setBrush(QBrush(fillColor));
    painter->setPen(chooseShapePenWhen(option));

    //Choose what shape to draw based on the diagram type
    if(properties.shapeType == shapeTypes::State){
            properties.text = shapeText->text();
            sizeToText();
            painter->drawRoundedRect(QRect(0,0,shapeWidth, shapeHeight),MARKER_SIZE,MARKER_SIZE);
    }else{
        properties.text = shapeFloatingText->text();
        properties.textBoxPosition = shapeFloatingText->pos();
        shapeWidth = MARKER_SIZE * 3 + 3 * MARKER_SIZE;
        shapeHeight = MARKER_SIZE * 3 + 3 * MARKER_SIZE;
        painter->drawEllipse(QPointF(MARKER_SIZE * 3, MARKER_SIZE * 3), MARKER_SIZE * 3, MARKER_SIZE * 3);

        if(properties.shapeType == shapeTypes::finalState){
            painter->drawEllipse(QPointF(MARKER_SIZE * 3, MARKER_SIZE * 3), MARKER_SIZE * 2.5, MARKER_SIZE * 2.5);

        }
    }

    calculateHookCenters();
    drawHookMarkers(painter, option);
}
//--------------------------------------------------------------------------------------------------------------------------------

QPen stateShape::chooseShapePenWhen(const QStyleOptionGraphicsItem *option)
{
    QPen pen;
    pen.setWidth(1);
    pen.setColor(QColor(0,0,0));

    if (option->state & QStyle::State_Selected){
        pen.setWidth(3);

    }

    if (option->state & QStyle::State_MouseOver){
        fillColor = QColor(255,255,255);

    }

    return pen;
}
//--------------------------------------------------------------------------------------------------------------------------------

void stateShape::sizeToText()
{
    shapeWidth = shapeText->boundingRect().width() + 2 * MARKER_SIZE;
    shapeHeight = shapeText->boundingRect().height() + 2 * MARKER_SIZE;

    this->itemChange(ItemPositionHasChanged, 0);//Just trick the scene to redraw the connector

}
//--------------------------------------------------------------------------------------------------------------------------------

QVariant stateShape::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemPositionHasChanged) {
        calculateHookCenters();
        properties.position = this->pos();

    }

    return QGraphicsItem::itemChange(change, value);
}
//--------------------------------------------------------------------------------------------------------------------------------

//Avoid stateShape overlapping each other
bool stateShape::isColliding()
{
    bool collissionExist = false;
    QList<QGraphicsItem*>::iterator collidingItemsIterator;
    QList<QGraphicsItem*> collidingItems = this->collidingItems();
    stateShape *shapesIterator;
    QGraphicsItem *item;

    for(collidingItemsIterator = collidingItems.begin(); collidingItemsIterator != collidingItems.end(); ++collidingItemsIterator){
        item = *collidingItemsIterator;
        shapesIterator = dynamic_cast<stateShape*>(item);

        if(shapesIterator){
            collissionExist = true;
            break;

        }
    }

    return collissionExist;
}
//--------------------------------------------------------------------------------------------------------------------------------

QRectF stateShape::boundingRect() const
{
    return QRectF(-halfMarkerSize, -halfMarkerSize,
                  shapeWidth + 2 * MARKER_SIZE,
                  shapeHeight + 2 * MARKER_SIZE);

}
