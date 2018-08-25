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

#include "textBox.h"

textBox::textBox(QString &text, QPointF position, QGraphicsItem * parent): QGraphicsTextItem(text, parent)
{
    setFlags(ItemIsSelectable | ItemIsFocusable);
    setTextInteractionFlags(Qt::TextEditable);
    setParentItem(parent);
    this->setPos(position);

    QFont labelFont;
    labelFont.setFamily("Courier");
    labelFont.setPixelSize(LABEL_PIXELS);
    setFont(labelFont);

}
//--------------------------------------------------------------------------------------------------------------------------------

void textBox::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0)
{
    if(option->state & QStyle::State_MouseOver){
        QPen pen;
        pen.setWidth(2);
        pen.setColor(QColor(0,255,0));
        painter->setPen(pen);
        painter->setBrush(Qt::NoBrush);
        painter->drawRect(this->boundingRect());

    }

    QGraphicsTextItem::paint(painter, option, widget);

}
//--------------------------------------------------------------------------------------------------------------------------------

QString textBox::text()
{
    return this->toPlainText();

}

