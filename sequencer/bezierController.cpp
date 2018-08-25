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

#include "bezierController.h"

bezierController::bezierController(QPointF initialPosition, QGraphicsItem *parent): QGraphicsItem(parent)
{
    this->setParentItem(parent);
    //this->setPos(this->mapFromScene(initialPosition));
    this->setPos(initialPosition);

    setFlags( ItemIsSelectable | ItemIsMovable | ItemSendsGeometryChanges | ItemIsFocusable);

}
//--------------------------------------------------------------------------------------------------------------------------------

void bezierController::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    QPen pen;
    pen.setWidth(2);
    pen.setColor(QColor(0,255,0));
    painter->setPen(pen);
    painter->setBrush(Qt::NoBrush);

    painter->drawEllipse(QPoint(0,0),MARKER_SIZE/2,MARKER_SIZE/2);

}
//--------------------------------------------------------------------------------------------------------------------------------

QRectF bezierController::boundingRect() const
{
    return QRectF(-MARKER_SIZE/2,-MARKER_SIZE/2, MARKER_SIZE, MARKER_SIZE);

}


