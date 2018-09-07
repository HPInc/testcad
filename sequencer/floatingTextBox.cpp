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

#include "floatingTextBox.h"

floatingTextBox::floatingTextBox(QString text, QPointF initialPosition, QGraphicsItem *parent)
{
    this->setParentItem(parent);
    setFlags(ItemIsSelectable | ItemIsMovable | ItemIsFocusable);

    this->setPos(initialPosition);
    tBox = new textBox(text, QPointF(0,0), this);

    highLight = false;

}
//--------------------------------------------------------------------------------------------------------------------------------

void floatingTextBox::setHighLighted(bool doHighLight)
{
    this->highLight = doHighLight;
}
//--------------------------------------------------------------------------------------------------------------------------------

void floatingTextBox::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    QVariant flagValue = "";

    if(option->state & QStyle::State_MouseOver){
        this->scene()->clearSelection();
        this->setSelected(true);

    }

    if((option->state & QStyle::State_Selected)||(option->state & QStyle::State_MouseOver)){
        QPen pen;
        pen.setWidth(2);
        pen.setColor(QColor(0,255,0));
        painter->setPen(pen);
        painter->setBrush(Qt::NoBrush);
        painter->drawRect(QRectF(-MARKER_SIZE, -MARKER_SIZE, MARKER_SIZE, MARKER_SIZE));
        flagValue = "textBoxSelected";

    }

    if(option->state & QStyle::State_MouseOver){
        highLight = false;

    }

    if (highLight){
        painter->setBrush(QColor(255,255,0));
        painter->drawRect(tBox->boundingRect());

    }

    parentItem()->setData(0, flagValue);

}
//--------------------------------------------------------------------------------------------------------------------------------

QRectF floatingTextBox::boundingRect() const
{
    QRectF labelBoxRect = tBox->boundingRect();
    return QRectF(QRectF(- MARKER_SIZE, - MARKER_SIZE, MARKER_SIZE + labelBoxRect.width(), MARKER_SIZE+labelBoxRect.height()));

}
//--------------------------------------------------------------------------------------------------------------------------------

QString floatingTextBox::text()
{
    return this->tBox->text();

}


