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

#include "connector.h"
#include "testCAD.h"
#include <QtWidgets>
#include <QTransform>

connector::connector(QPointF startPoint, QPointF endPoint, int ID, QString text)
{  
    properties.ID = ID;
    properties.startPoint = startPoint;
    properties.endPoint = endPoint;
    properties.text = text;
    properties.textBoxPosition = QPointF(0,0);
    properties.bezierFromPosition =  startPoint;
    properties.bezierToPosition = endPoint;

    setup();

    bezierFrom = new bezierController(this->mapFromScene(startPoint), this);
    bezierTo = new bezierController(this->mapFromScene(endPoint), this);
    txtBox = new floatingTextBox(properties.text, QPointF(0,0), this);
    txtBox->setVisible(false);
    bezierTo->setVisible(false);

}
//--------------------------------------------------------------------------------------------------------------------------------

connector::connector(connectorProperties values)
{
    properties = values;

    setup();

    bezierFrom = new bezierController(properties.bezierFromPosition, this);
    bezierTo = new bezierController(properties.bezierToPosition, this);
    txtBox = new floatingTextBox(properties.text, properties.textBoxPosition, this);
    txtBox->setVisible(true);
    bezierTo->setVisible(true);

}
//--------------------------------------------------------------------------------------------------------------------------------

void connector::setup()
{
    this->setPos(properties.startPoint);
    setFlags( ItemIsSelectable | ItemSendsGeometryChanges | ItemIsFocusable);

}
//--------------------------------------------------------------------------------------------------------------------------------

connector::~connector()
{
    delete txtBox;
    delete bezierFrom;
    delete bezierTo;

}
//--------------------------------------------------------------------------------------------------------------------------------

void connector::glueTo(QPointF endPoint)
{
    properties.endPoint = endPoint;

    bezierTo->setPos(QPointF((properties.endPoint.x()- properties.startPoint.x())/1.2,
                             (properties.endPoint.y()- properties.startPoint.y())/1.2));

    bezierFrom->setPos(QPointF((properties.endPoint.x()- properties.startPoint.x())/5,
                             (properties.endPoint.y()- properties.startPoint.y())/5));

    bezierTo->setVisible(true);

    txtBox->setVisible(true);
    txtBox->setPos(QPointF((properties.endPoint.x()- properties.startPoint.x())/2,
                          (properties.endPoint.y()- properties.startPoint.y())/2));

}
//--------------------------------------------------------------------------------------------------------------------------------

QPainterPath connector::shape() const
{
    return newOutlinePath;

}
//--------------------------------------------------------------------------------------------------------------------------------

QRectF connector::boundingRect() const
{
    return newBoundingRect;

}
//--------------------------------------------------------------------------------------------------------------------------------

void connector::drawConnector(QPainter *painter, const QStyleOptionGraphicsItem *option)
{
    QPen pen;
    pen.setWidth(1);

    if ((option->state & QStyle::State_Selected) ||
        (bezierFrom->isSelected()) ||
        (bezierTo->isSelected()))
    {
        pen.setWidth(2);
        pen.setStyle(Qt::DotLine);
        bezierFrom->setVisible(true);
        bezierTo->setVisible(true);
        pen.setColor(QColor(0,0,0));

    }else{
        pen.setStyle(Qt::SolidLine);
        bezierFrom->setVisible(false);
        bezierTo->setVisible(false);
        pen.setColor(QColor(0,0,0));

    }

    if (this->data(0) == "textBoxSelected"){
        pen.setColor(QColor(0,0,255));

    }

    painter->setPen(pen);
    painter->setBrush(Qt::NoBrush);

    QTransform rotator;
    rotator.rotate(connectionAngle);

    QPainterPath connectorPath;
    connectorPath.moveTo(0,0);
    connectorPath.cubicTo(this->mapFromItem(bezierFrom, bezierFrom->pos()),bezierTo->pos(),rotator.map(QPointF(length, 0)));

    painter->drawPath(connectorPath);

    QPainterPathStroker *stroker = new QPainterPathStroker();
    stroker->setWidth(10);
    newOutlinePath = stroker->createStroke(connectorPath);
    newBoundingRect = newOutlinePath.boundingRect();
    newBoundingRect.setSize(QSizeF(newBoundingRect.width() - 20,
                                   newBoundingRect.height() - 20));
    newBoundingRect.moveBottomRight(QPointF(newBoundingRect.bottomRight().x() + 10,
                                            newBoundingRect.bottomRight().y() + 10));

    pen.setWidth(1);
    drawArrowHead(-connectorPath.angleAtPercent(0.95),painter);

}
//--------------------------------------------------------------------------------------------------------------------------------

void connector::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);

    calculateConnectorGeometry();
    drawConnector(painter,option);
    properties.bezierFromPosition = bezierFrom->pos();
    properties.bezierToPosition = bezierTo->pos();
    properties.text = txtBox->text();
    properties.textBoxPosition = txtBox->pos();

}
//--------------------------------------------------------------------------------------------------------------------------------

void connector::calculateConnectorGeometry()
{
    this->setPos(properties.startPoint);
    this->length = Trigonometry::getDistance(properties.startPoint, properties.endPoint );
    this->connectionAngle = Trigonometry::getDegreesClockWise(properties.startPoint, properties.endPoint );

}
//--------------------------------------------------------------------------------------------------------------------------------

void connector::drawArrowHead(qreal angle, QPainter *painter)
{
    qreal dx = length;
    qreal dy = 0;

    QBrush connBrush(QColor(0,0,0),Qt::SolidPattern);
    painter->setBrush(connBrush);

    QTransform positioner;
    positioner.rotate(connectionAngle);
    positioner.translate(dx,dy);
    positioner.rotate(-connectionAngle+angle);

    //The point 0,0 is the arrow tip points right, defining points counterclock wise
    QVector<QPoint> arrowPoints;
    QPoint mappedPoint;
    mappedPoint = positioner.map(QPoint(0,0));
    arrowPoints.append(mappedPoint);
    mappedPoint = positioner.map(QPoint(-ARROW_TIP_SIZE,-ARROW_TIP_SIZE / 2));
    arrowPoints.append(mappedPoint);
    mappedPoint = positioner.map(QPoint(-ARROW_TIP_SIZE + ARROW_TIP_SIZE / 3,0));
    arrowPoints.append(mappedPoint);
    mappedPoint = positioner.map(QPoint(-ARROW_TIP_SIZE, ARROW_TIP_SIZE / 2));
    arrowPoints.append(mappedPoint);

    painter->drawPolygon(arrowPoints);

}
//--------------------------------------------------------------------------------------------------------------------------------


