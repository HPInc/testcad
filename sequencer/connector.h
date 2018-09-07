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

#ifndef CONNECTOR_H
#define CONNECTOR_H

#include <QColor>
#include <QGraphicsItem>
#include <QKeyEvent>
#include <QPainterPathStroker>
#include "trigonometry.h"
#include "bezierController.h"
#include "floatingTextBox.h"

#define ARROW_TIP_SIZE 15

class connectorProperties
{
public:
    int ID;
    int fromID;
    int toID;
    QString text;
    QPointF textBoxPosition;
    int fromHookPointIndex;
    int toHookPointIndex;
    QPointF bezierFromPosition;
    QPointF bezierToPosition;
    QPointF startPoint;
    QPointF endPoint;

};

class connector : public QGraphicsItem
{

public:
    connector(QPointF startPoint, QPointF endPoint, int ID, QString text);
    connector(connectorProperties values);
    ~connector();
    void glueTo(QPointF endPoint);
    QRectF boundingRect() const Q_DECL_OVERRIDE;
    QPainterPath shape() const Q_DECL_OVERRIDE;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) Q_DECL_OVERRIDE;
    connectorProperties properties;
    void setHighlighted(bool doHighLight);

private:
    void setup();
    qreal connectionAngle;
    int length;
    QRectF newBoundingRect;
    QPainterPath newOutlinePath;

    void calculateConnectorGeometry();
    void drawConnector(QPainter *painter, const QStyleOptionGraphicsItem *option);
    void drawArrowHead(qreal angle, QPainter *painter);

    floatingTextBox* txtBox;
    bezierController* bezierFrom;
    bezierController* bezierTo;

};

#endif // CONNECTOR_H
