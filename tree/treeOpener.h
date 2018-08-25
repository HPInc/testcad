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

#ifndef TREEOPENER_H
#define TREEOPENER_H

#include <QtWidgets>
#include <QHash>
#include <QXmlStreamReader>
#include <QMessageBox>
#include "testCAD.h"
#include "treeEditor.h"
#include "sequencer/sequencerWindow.h"

class treeOpener
{
public:

    enum callPurpose{
        toLoad,
        toCompare,
    };

    treeOpener(QTreeWidget *treeWidget, QWidget *parent);
    bool fileOpened();
    bool selectToCompare();
    QString filePath;
    QHash<QString, QString> diffHashTable;
    bool openSelected(callPurpose purpose);
    QFile *fileChosen;

private:
    void addToDiagramPropertiesBuffer(shapeProperties stateProperties);
    void addToDiagramPropertiesBuffer(connectorProperties transitionProperties);
    QList<QVariant> allShapesProperties;
    QList<QVariant> allConnectorsProperties;
    shapeProperties shapePropertiesBuffer;
    connectorProperties connectorPropertiesBuffer;
    QString lastTextBuffer;
    bool isComparableTag(QString tagValue);

    void loadStateProperties();
    void loadTransitionProperties();
    void setActiveItemSequenceDesign();

    QWidget *parentWidget;
    QTreeWidget *treeWidget;
    bool selectFile();
    QFile *openFileDialog();

    QXmlStreamReader *xmlReader;
    void processElement(int tokenType, callPurpose purpose);
    void processStart(callPurpose purpose);
    void processEnd(callPurpose purpose);
    bool rootTagFound;
    treeEditor *editor;
    QList<QVariant> tableHeaderParents;
    QList<QVariant> tableRowHeaders;
    QList<QVariant> tableColumnHeaders;
    QStringList tableColumn;
    QList<QVariant> designData;

    QList<QString> xPathParts;
    QString getXPath(QList<QString> xPathParts);

};

#endif // TREEOPENER_H
