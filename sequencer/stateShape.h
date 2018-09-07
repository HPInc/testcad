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

#ifndef STATESHAPE_H
#define STATESHAPE_H

#include <QColor>
#include <QGraphicsItem>
#include <QKeyEvent>
#include <QtWidgets>
#include "testCAD.h"
#include "textBox.h"
#include "floatingTextBox.h"

enum shapeTypes{
    State,
    initialState,
    finalState
};

class shapeProperties
{
public:
    int ID;
    QString text;
    QPointF textBoxPosition;
    QPointF position;
    int shapeType;
    QPointF hookPoints[4];
    QPointF selectedHookPoint;
    int selectedHookPointIndex;

};

class stateShape : public QGraphicsItem
{

public:
    stateShape(QPointF position, int ID, QString text, int shapeType);
    stateShape(shapeProperties values);
    ~stateShape();

    QRectF boundingRect() const Q_DECL_OVERRIDE;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) Q_DECL_OVERRIDE;
    int getNearestHookPointIndex();
    shapeProperties properties;
    QColor fillColor;
    void highlight();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;

public slots:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

private:
    void construct();
    int shapeHeight;
    int shapeWidth;
    int halfMarkerSize;

    void drawHookMarkers(QPainter *painter, const QStyleOptionGraphicsItem *option);
    QPointF mousePositionInLocalCoordinates;
    QPen chooseShapePenWhen(const QStyleOptionGraphicsItem *option);
    void calculateHookCenters();
    bool isColliding();

    textBox* shapeText;
    floatingTextBox* shapeFloatingText;

    void sizeToText();

};

#endif // STATESHAPE_H
