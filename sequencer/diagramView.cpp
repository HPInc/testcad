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

#include "diagramView.h"

diagramView::diagramView(int lastID)
{
    controlKeyIsPressed = false;
    Diagram = new diagram(this, lastID);
    mousePosition.setX(0);
    mousePosition.setY(0);
    mouseKeyIsPressed = false;
    this->setScene(Diagram);
    this->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

}
//--------------------------------------------------------------------------------------------------------------------------------

void diagramView::wheelEvent(QWheelEvent *event)
{
    if(controlKeyIsPressed){
        QRectF newRect = Diagram->itemsBoundingRect();
        qreal dx = newRect.width() * 0.5 / 2;
        qreal dy = newRect.height() * 0.5 / 2;

        if (event->delta() < 0){
            this->scale(0.95, 0.95);
            newRect.adjust(dx, dy, -dx, -dy);

        }else{
            this->scale(1.05, 1.05);
            newRect.adjust(-dx, -dy, dx, dy);

        }

        Diagram->setSceneRect(newRect);
    }

    QGraphicsView::wheelEvent(event);
}
//--------------------------------------------------------------------------------------------------------------------------------

void diagramView::mouseMoveEvent(QMouseEvent *event)
{
    mousePosition = event->pos();

    if(userIsMoving()){
        this->setDragMode(QGraphicsView::NoDrag);
        this->setCursor(QCursor(Qt::ClosedHandCursor));

    }else if(Diagram->selectedItems().size() == 0){
        this->setDragMode(QGraphicsView::ScrollHandDrag);

    }else{
        this->setDragMode(QGraphicsView::NoDrag);
        this->setCursor(QCursor(Qt::ArrowCursor));

    }

    QGraphicsView::mouseMoveEvent(event);
}
//--------------------------------------------------------------------------------------------------------------------------------

diagramView::~diagramView()
{
    Diagram->clearDiagram();
    delete Diagram;
}
//--------------------------------------------------------------------------------------------------------------------------------

bool diagramView::userIsMoving()
{
    if (((Diagram->selectedItems().size() > 0)) &&
            (mouseKeyIsPressed)){
        return true;

    }else{
        return false;

    }
}
//--------------------------------------------------------------------------------------------------------------------------------

void diagramView::mousePressEvent(QMouseEvent *event)
{
    mouseClickedPoint = event->pos();
    mouseKeyIsPressed = true;

    if(event->button()==Qt::LeftButton){
        this->repaint();

    }

    QGraphicsView::mousePressEvent(event);
}
//--------------------------------------------------------------------------------------------------------------------------------

void diagramView::mouseReleaseEvent(QMouseEvent *event)
{
    mouseKeyIsPressed = false;
    mouseReleasedPoint = event->pos();

    QGraphicsView::mouseReleaseEvent(event);
}
//--------------------------------------------------------------------------------------------------------------------------------

void diagramView::keyPressEvent(QKeyEvent *event)
{
    if (event->key()==Qt::Key_Control){
        this->controlKeyIsPressed = true;

        if (mousePosition.isNull()){
            mousePosition = Diagram->itemsBoundingRect().center().toPoint();

        }

    }else{
        this->controlKeyIsPressed = false;

    }
    QGraphicsView::keyPressEvent(event);
}
//--------------------------------------------------------------------------------------------------------------------------------

void diagramView::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key()==Qt::Key_Control){
        this->controlKeyIsPressed = false;

    }
    QGraphicsView::keyReleaseEvent(event);
}

