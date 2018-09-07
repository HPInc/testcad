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

#ifndef DIAGRAM_H
#define DIAGRAM_H

#include <QObject>
#include <QPointF>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QAction>
#include <QKeyEvent>
#include <QMainWindow>
#include "connector.h"
#include "stateShape.h"

enum diagramStatus{
    connectionRequested,
    connectionInProgress,
    connectionEstablished,
};
//--------------------------------------------------------------------------------------------------------------------------------

class diagramStructure{

public:
    QList<shapeProperties> shapesInfo;
    QList<connectorProperties> connectorsInfo;

};
//--------------------------------------------------------------------------------------------------------------------------------

class diagramRules{

public:
    bool hasInitialState = false;
    bool hasUniqueInitialState = false;
    bool initialStateHasOutboundTransition = false;
    bool initialStateHasNoInboundTransition = true;
    bool initialStateHasUniqueOutboundTransition = true;

    bool hasIntermediateState = false;
    bool allStatesHaveInboundTransition = true;
    bool allStatesHaveOutboundTransition = true;

    bool hasFinalState = false;
    bool hasUniqueFinalState = false;
    bool finalStateHasInboundTransitions = false;
    bool finalStateHasNoOutboundTransition = true;
    bool allElementsHaveText = true;

    int initialStatesCount = 0;
    int finalStatesCount = 0;

};
//--------------------------------------------------------------------------------------------------------------------------------

class diagram : public QGraphicsScene
{
    Q_OBJECT

public:
    explicit diagram(QGraphicsView *view, int lastID = 0);
    ~diagram();
    diagramStatus currentStatus;
    void addShape(int shapeType, QPointF insertionPoint);
    diagramStructure getStructure();
    stateShape* getShapeByID(int shapeID);
    void loadFromStructure(diagramStructure structure);
    bool hasChanged;
    bool isValid();
    bool saveImage(QString imagePath);
    void redrawDiagram();
    void fitViewerTo(QRectF fitArea);
    void findAndHighlight(QString textToFind);

private:
    int nextID;
    diagramStructure structureCache;
    stateShape* getSelectedShape();
    connector* newConnector;
    void setDiagramConnectionStatus();
    void beginConnection(shapeProperties fromshape);
    bool glueConnection(shapeProperties toshape);
    void redrawConnectors(shapeProperties sProperties);
    void deleteAssociatedConnectors(shapeProperties sProperties);
    void mouseReleaseEvent( QGraphicsSceneMouseEvent * mouseEvent ) Q_DECL_OVERRIDE;
    void mouseMoveEvent ( QGraphicsSceneMouseEvent * mouseEvent ) Q_DECL_OVERRIDE;
    QPointF lastClickPoint;
    QPointF mousePosition;
    int countOutboundTransitions(int stateID);
    int countInboundTransitions(int stateID);
    diagramRules checkRules();
    QString getRulesErrors(diagramRules rules);
    QGraphicsView *viewer;


signals:

public slots:
       void deleteSelected();
       void connectStates();
       void addInitialState();
       void addState();
       void addFinalState();
       void clearDiagram();

};

#endif // DIAGRAM_H
