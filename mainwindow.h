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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "iconsCatalog.h"
#include "collector/collectorWindow.h"
#include "combiner/combinerWindow.h"
#include "sequencer/sequencerWindow.h"
#include "crossChecker/crossCheckerWindow.h"
#include "tree/treeOpener.h"
#include "tree/treeSaver.h"
#include "tree/treeEditor.h"
#include "tree/treeAnalyzer.h"
#include "testCAD.h"
#include "splashWindow.h"

QT_BEGIN_NAMESPACE
class QTreeWidget;
class QTreeWidgetItem;
class treeEditor;
class treeAnalyzer;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    void openFromArgument(QString filePathParam);
    QTreeWidgetItem *clickedItem;

public slots:
    void show();
    void open();
    void append();
    void save();
    void saveAs();
    void closeTree();
    void about();
    void compare();
    void exportImage();
    void clearHighlights();
    void trackUnsavedChanges(QTreeWidgetItem *item, int column);
    void showContextMenu(QPoint point);
    void openDesignerWindow();
    void trackClicks(QTreeWidgetItem *item);
    void openRecent();
    void search();

private:

    enum editionType{
        editing,
        appending,
    };

    QString filePath;
    QStringList recentFiles;
    QString historyFilePath;
    void addToOpenHistory(QString filePath);
    void reloadOpenHistory();
    void createActions();
    void createFileActions();
    void createEditActions();
    void createInsertActions();
    void createViewActions();
    void createToolsActions();
    void createTestActions();
    void createHelpActions();
    void createMenus();
    void createToolbars();
    void setWindowTitleToFileName();
    void openTreeFor(int editionType);
    int notSavedWarningResponse();
    bool hasChanges;
    void closeEvent(QCloseEvent * event);
    void showSplashWindow();

    QTreeWidget *treeWidget;
    iconsCatalog *icons;
    treeEditor *editor;
    treeAnalyzer *analyzer;
    collectorWindow *collector;
    combinerWindow *combiner;
    sequencerWindow *sequencer;
    crossCheckerWindow *crossChecker;

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *insertMenu;
    QMenu *viewMenu;
    QMenu *toolsMenu;
    QMenu *testMenu;
    QMenu *helpMenu;
    QMenu *historyMenu;

    QAction *openAct;
    QAction *appendAct;
    QAction *saveAsAct;
    QAction *saveAct;
    QAction *closeTreeAct;
    QAction *exitAct;
    QAction *aboutAct;

    QAction *markPendingAct;
    QAction *markPlannedAct;
    QAction *markValidatedAct;
    QAction *markReviewAct;
    QAction *markFailed;
    QAction *markUnsupported;
    QAction *collapseAct;
    QAction *expandAct;
    QAction *undoAct;

    QAction *collapseAllAct;
    QAction *expandAllAct;
    QAction *findFailedAct;
    QAction *findPlannedAct;
    QAction *findPendingAct;
    QAction *findReviewAct;
    QAction *findPassedAct;
    QAction *findUnsupportedAct;
    QAction *moveUpAct;
    QAction *moveDownAct;
    QAction *clearHighlightsAct;

    QAction *addComponentAct;
    QAction *addMultipleAct;

    QAction *deleteAct;
    QAction *copyAct;
    QAction *cutAct;
    QAction *pasteAct;

    QAction *showStatsAct;
    QAction *copyPathClipAct;
    QAction *copyBranchClipAct;

    QAction *searchAct;
    QAction *showDiffAct;
    QAction *exportImageAct;
    QAction *copyVariableStats;

    QAction *addTestGroupAct;
    QAction *addTestCollectionAct;
    QAction *addTestCombinationAct;
    QAction *addTestSequenceAct;
    QAction *addTestCrossCheckerAct;

    QAction *openTestDesignerAct;

};

#endif
