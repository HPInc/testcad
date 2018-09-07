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

#ifndef SEQUENCERWINDOW_H
#define SEQUENCERWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QFileDialog>
#include "diagram.h"
#include "diagramView.h"
#include "testCAD.h"
#include "fsmRouter.h"

class sequencerWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit sequencerWindow(QWidget *parent = 0, QTreeWidgetItem *testItem = 0);
    QTreeWidgetItem *treeItem;
    ~sequencerWindow();
    static int instance;

    static diagramStructure structureFromItem(QTreeWidgetItem *treeItem);
    static QVariant propertiesToVariant(shapeProperties properties);
    static QVariant propertiesToVariant(connectorProperties properties);

    static shapeProperties variantToShapeProperties(QVariant shapePropertiesVariant);
    static connectorProperties variantToConnectorProperties(QVariant connectorPropertiesVariant);

    void loadDiagram();

    enum exportAs{
        html,
        txt
    };

private:
    diagramView* diagramEditor;
    sequencesList testCases;
    statsByTest testCasesStatistics;
    int switchLevel;

    void createMenu();
    void createActions();
    void createToolBar();
    void closeEvent(QCloseEvent *event);
    bool writeHtmlTo(QString filePath);
    bool writeTxtTo(QString filePath);

    QAction *addStateAction;
    QAction *addInitialStateAction;
    QAction *addFinalStateAction;
    QAction *connectAction;
    QAction *newDiagramAction;
    QAction *deleteSelectedAction;
    QAction *storeDiagramAction;
    QAction *exportAsHtmlAction;
    QAction *exportAsTxtAction;
    QAction *showHelpAction;
    QAction *clearDiagramAction;
    QAction *centerDiagramAction;
    QAction *saveImageAction;
    QAction *findTextAction;

    QXmlStreamWriter *xmlWriter;

    void writeHtmlStyle();
    void writeHtmlH1(int testsCount);
    void writeColumnHeaders();
    void writeDataRows(switchSequence testSequence);
    void writeCoverageStatistics(int testIndex);
    void writeTestStatistics(int testIndex);
    void writeMissingZeroSwitches(int testIndex);
    void writeMissingNSwitches(int testIndex);
    void exportTestsAs(int fileFormat);

    void writeFormattedLines(QString textToFormat);

private slots:

    void newDiagram();
    void exportHtml();
    void exportTxt();
    void storeDiagram();
    void showHelp();
    void clearDiagram();
    void centerOnDiagram();
    void saveAsPicture();
    void findText();

signals:
    void saved();

};

#endif // SEQUENCERWINDOW_H
