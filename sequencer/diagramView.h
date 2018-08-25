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

#ifndef DIAGRAMVIEW_H
#define DIAGRAMVIEW_H

#include <QGraphicsView>
#include <QWheelEvent>
#include "diagram.h"

class diagramView : public QGraphicsView
{
    Q_OBJECT

public:
    explicit diagramView(int lastID = 0);
    ~diagramView();
    diagram *Diagram;

private:
    bool controlKeyIsPressed;
    bool mouseKeyIsPressed;
    bool userIsMoving();
    QPoint mousePosition;
    QPoint mouseClickedPoint;
    QPoint mouseReleasedPoint;
    void wheelEvent(QWheelEvent* event) Q_DECL_OVERRIDE;
    void keyPressEvent(QKeyEvent * event)  Q_DECL_OVERRIDE;
    void keyReleaseEvent(QKeyEvent * event)  Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent * event)  Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent * event)  Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent * event)  Q_DECL_OVERRIDE;

};

#endif // DIAGRAMVIEW_H
