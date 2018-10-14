#include <QtWidgets>
#include <QScreen>
#include "mainwindow.h"

MainWindow::MainWindow()
{
    this->setWindowIcon(QIcon(":/icons/testCADIcon.png"));

    QStringList labels;
    labels << tr(STRING_COMPONENTS) << "  ≠" << tr(STRING_COMMENTS);

    treeWidget = new QTreeWidget;
    treeWidget->setHeaderLabels(labels);
    treeWidget->header()->setSectionResizeMode(0,QHeaderView::ResizeToContents);
    treeWidget->header()->setSectionResizeMode(1,QHeaderView::ResizeToContents);
    treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    setCentralWidget(treeWidget);

    editor = new treeEditor(treeWidget);
    analyzer = new treeAnalyzer(treeWidget);
    icons = new iconsCatalog;
    collector = 0;
    combiner = 0;
    sequencer = 0;
    crossChecker = 0;

    connect(treeWidget, SIGNAL(itemChanged(QTreeWidgetItem*,int)), editor, SLOT(preventDiffColumnEdition(QTreeWidgetItem*,int)));
    connect(treeWidget, SIGNAL(itemChanged(QTreeWidgetItem*,int)), this, SLOT(trackUnsavedChanges(QTreeWidgetItem*,int)));
    connect(treeWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu(QPoint)));
    connect(treeWidget, SIGNAL(itemPressed(QTreeWidgetItem*,int)),this,SLOT(trackClicks(QTreeWidgetItem*)));

    createActions();
    createMenus();
    createToolbars();

    statusBar()->showMessage(QObject::tr(STRING_READY));
    setWindowTitle(QObject::tr(STRING_TESTCAD));

    QScreen *screen = QGuiApplication::primaryScreen();
    QSize newSize = screen->availableSize();
    newSize.setHeight(newSize.height()*0.8);
    newSize.setWidth(newSize.width()*0.8);
    resize(newSize);

    hasChanges = false;
    historyFilePath = QDir::currentPath() + "/" + STRING_HISTORY_FILE;
    reloadOpenHistory();

}
//----------------------------------------------------------------------------------------------------

void MainWindow::addToOpenHistory(QString filePath)
{
    QFile hFile (historyFilePath);
    QTextStream history(&hFile);
    QString hLine;
    QStringList recentFiles;

    if (hFile.exists()){
        hFile.open(QFile::ReadWrite | QFile::Text );
        hLine = history.readLine();

        while (hLine.length() > 0){
            recentFiles.append(hLine + '\n');
            hLine = history.readLine();

        }

        hFile.remove();
    }

    hFile.open(QFile::WriteOnly | QFile::Text );

    if (!recentFiles.contains(filePath + '\n')){
        recentFiles.prepend(filePath + '\n');

    }

    for (int n = 0; n < recentFiles.length(); n++){
        history << recentFiles.at(n);

        if (n > 5)
            break;

    }

    hFile.close();;

}
//----------------------------------------------------------------------------------------------------

void MainWindow::reloadOpenHistory()
{
    QFile hFile (historyFilePath);
    QTextStream history(&hFile);
    QString hLine;
    recentFiles.clear();

    if (hFile.exists()){
        hFile.open(QFile::ReadOnly | QFile::Text );
        hLine = history.readLine();

        while (hLine.length() > 0){
            recentFiles.append(hLine);
            hLine = history.readLine();

        }
    }

    hFile.close();
    historyMenu->clear();

    for (int n = 0; n < recentFiles.length(); n++){
        QAction *actBuff = historyMenu->addAction(recentFiles.at(n));
        connect(actBuff, SIGNAL(triggered()), this, SLOT(openRecent()));
        actBuff->setCheckable(true);
        actBuff->setChecked(false);

    }

}
//----------------------------------------------------------------------------------------------------

void MainWindow::openRecent()
{
    for (int n = 0; n < historyMenu->actions().count(); n++){
        QAction *actBuff = historyMenu->actions().at(n);

        if (actBuff->isChecked()){
            int decision = notSavedWarningResponse();

            if(QMessageBox::Save == decision)
            {
                save();
                treeWidget->clear();
                openFromArgument(recentFiles.at(n));
                hasChanges=false;

            }else if(QMessageBox::Ignore == decision){
                treeWidget->clear();
                openFromArgument(recentFiles.at(n));
                hasChanges=false;

            }
        }

        actBuff->setChecked(false);
    }

}
//----------------------------------------------------------------------------------------------------

void MainWindow::openFromArgument(QString filePathParam)
{
    treeOpener opener(treeWidget, this);
    opener.filePath = filePathParam;

    if (opener.openSelected(treeOpener::toLoad)){
        filePath = opener.filePath;
        addToOpenHistory(filePath);
        setWindowTitleToFileName();
        statusBar()->showMessage(QObject::tr(STRING_FILE_LOADED),2000);

    }else{
        statusBar()->showMessage(QObject::tr(STRING_NO_FILE_LOADED),2000);

    }

}
//----------------------------------------------------------------------------------------------------

void MainWindow::openTreeFor(int editionType)
{
    if (editionType == editing){
       treeWidget->clear();

    }

    treeOpener opener(treeWidget, this);

    if (opener.fileOpened()){

        if (editionType != appending){
            filePath = opener.filePath;
            addToOpenHistory(filePath);
            setWindowTitleToFileName();

        }

        reloadOpenHistory();
        statusBar()->showMessage(QObject::tr(STRING_FILE_LOADED),2000);

    }else{
        statusBar()->showMessage(QObject::tr(STRING_NO_FILE_LOADED),2000);

    }

}
//----------------------------------------------------------------------------------------------------

void MainWindow::showSplashWindow()
{
    splashWindow *spW = new splashWindow(this);
    spW->show();

}
//----------------------------------------------------------------------------------------------------

void MainWindow::openDesignerWindow()
{
    if (clickedItem->data(0, Qt::UserRole).toString() == TAG_TYPE_TEST_COLLECTION){

        if (collectorWindow::instance == 0){
            collector = new collectorWindow(this, clickedItem);
            connect(collector, SIGNAL(saved()), this, SLOT(save()));
            collector->show();
        }

    }else if (clickedItem->data(0, Qt::UserRole).toString() == TAG_TYPE_TEST_COMBINATION){

        if (combinerWindow::instance == 0){
            combiner = new combinerWindow(this, clickedItem);
            connect(combiner, SIGNAL(saved()), this, SLOT(save()));
            combiner->show();
        }

    }else if (clickedItem->data(0, Qt::UserRole).toString() == TAG_TYPE_TEST_SEQUENCE){

        if (sequencerWindow::instance == 0){
            sequencer = new sequencerWindow(this, clickedItem);
            connect(sequencer, SIGNAL(saved()), this, SLOT(save()));
            sequencer->show();

        }

    }else if (clickedItem->data(0, Qt::UserRole).toString() == TAG_TYPE_TEST_CROSSCHECK){

        if (crossCheckerWindow::instance == 0){
            crossChecker = new crossCheckerWindow(this, clickedItem);
            connect(crossChecker, SIGNAL(saved()), this, SLOT(save()));
            crossChecker->show();
        }

    }

}
//----------------------------------------------------------------------------------------------------

void MainWindow::trackClicks(QTreeWidgetItem *item)
{
    clickedItem = item;
    editor->setClickedItem(item);
    analyzer->setClickedItem(item);

    if (collector != 0){
        collectorWindow::clickedItem = item;

    }

    if (combiner != 0){
        combinerWindow::clickedItem = item;

    }

    if (crossChecker != 0){
        crossCheckerWindow::clickedItem = item;

    }

}
//----------------------------------------------------------------------------------------------------

void MainWindow::showContextMenu(QPoint point)
{
    Q_UNUSED(point);

    QMenu contextMenu;

    if ((editor->getItemType(treeWidget->selectedItems().at(0))!=TAG_TYPE_TEST_GROUP) &&
            (editor->getItemType(treeWidget->selectedItems().at(0))!=TAG_TYPE_TEST_COLLECTION) &&
            (editor->getItemType(treeWidget->selectedItems().at(0))!=TAG_TYPE_TEST_COMBINATION) &&
            (editor->getItemType(treeWidget->selectedItems().at(0))!=TAG_TYPE_TEST_SEQUENCE) &&
            (editor->getItemType(treeWidget->selectedItems().at(0))!=TAG_TYPE_TEST_CROSSCHECK)){
        contextMenu.addAction(markPlannedAct);
        contextMenu.addAction(markValidatedAct);
        contextMenu.addAction(markReviewAct);
        contextMenu.addAction(markPendingAct);
        contextMenu.addAction(markFailed);
        contextMenu.addAction(markUnsupported);
        contextMenu.addSeparator();
        contextMenu.addAction(copyPathClipAct);
        contextMenu.addAction(copyBranchClipAct);
        contextMenu.addSeparator();
        contextMenu.addAction(collapseAct);
        contextMenu.addAction(expandAct);
        contextMenu.addSeparator();
        contextMenu.addAction(addMultipleAct);
        contextMenu.addSeparator();
        contextMenu.addAction(copyVariableStats);
        contextMenu.addSeparator();
        contextMenu.addAction(copyAct);
        contextMenu.addAction(cutAct);
        contextMenu.addAction(pasteAct);

    }else if (editor->getItemType(treeWidget->selectedItems().at(0)) == TAG_TYPE_TEST_GROUP){
        contextMenu.addAction(addTestCollectionAct);
        contextMenu.addAction(addTestCombinationAct);
        contextMenu.addAction(addTestSequenceAct);
        contextMenu.addAction(addTestCrossCheckerAct);
        contextMenu.addSeparator();
        contextMenu.addAction(collapseAct);
        contextMenu.addAction(expandAct);
        contextMenu.addSeparator();
        contextMenu.addAction(pasteAct);

    }else if ((editor->getItemType(treeWidget->selectedItems().at(0)) == TAG_TYPE_TEST_COLLECTION) ||
              (editor->getItemType(treeWidget->selectedItems().at(0)) == TAG_TYPE_TEST_COMBINATION)||
              (editor->getItemType(treeWidget->selectedItems().at(0)) == TAG_TYPE_TEST_SEQUENCE)||
              (editor->getItemType(treeWidget->selectedItems().at(0)) == TAG_TYPE_TEST_CROSSCHECK)){
        contextMenu.addAction(openTestDesignerAct);
        contextMenu.addSeparator();
        contextMenu.addAction(copyAct);
        contextMenu.addAction(cutAct);

    }

    contextMenu.exec(QCursor::pos());

}
//----------------------------------------------------------------------------------------------------

void MainWindow::createToolbars()
{
    QToolBar *tb =  this->addToolBar(tr(STRING_MENU_TOOLS));
    tb->setMovable(false);
    tb->addAction(openAct);
    tb->addAction(saveAct);
    tb->addAction(saveAsAct);
    tb->addSeparator();
    tb->addAction(addComponentAct);
    tb->addSeparator();
    tb->addAction(copyAct);
    tb->addAction(cutAct);
    tb->addAction(pasteAct);
    tb->addAction(deleteAct);
    tb->addAction(undoAct);
    tb->addAction(moveUpAct);
    tb->addAction(moveDownAct);
    tb->addSeparator();
    tb->addAction(searchAct);
    tb->addAction(findFailedAct);
    tb->addAction(findReviewAct);
    tb->addAction(findUnsupportedAct);
    tb->addAction(findPendingAct);
    tb->addAction(findPlannedAct);
    tb->addAction(findPassedAct);
    tb->addAction(showStatsAct);
    tb->addSeparator();
    tb->addAction(addTestGroupAct);
    tb->addAction(addTestCollectionAct);
    tb->addAction(addTestCombinationAct);
    tb->addAction(addTestSequenceAct);
    tb->addAction(addTestCrossCheckerAct);
    tb->addSeparator();
    tb->addAction(clearHighlightsAct);

}
//----------------------------------------------------------------------------------------------------

void MainWindow::closeEvent(QCloseEvent *event)
{
    int decision = notSavedWarningResponse();
    bool deleteChildren = false;

    if(QMessageBox::Save == decision)
    {
        save();
        deleteChildren = true;

    }else if(QMessageBox::Ignore == decision)
    {
        deleteChildren = true;

    }else if(QMessageBox::Cancel == decision)
    {
        event->ignore();

    }

    if (deleteChildren){

        if (collectorWindow::instance != 0)
            delete(collector);

        if (combinerWindow::instance != 0)
            delete(combiner);

        if (crossCheckerWindow::instance != 0)
            delete(crossChecker);

        if (sequencerWindow::instance != 0)
            delete(sequencer);

        event->accept();
    }


}
//----------------------------------------------------------------------------------------------------

void MainWindow::exportImage()
{
    treeSaver saver(treeWidget, this);
    saver.exportTreeToImage();

}
//----------------------------------------------------------------------------------------------------

void MainWindow::clearHighlights()
{
    treeEditor::clearHighlightsIn(treeWidget);
    clearHighlightsAct->setVisible(false);

}
//----------------------------------------------------------------------------------------------------

void MainWindow::trackUnsavedChanges(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(item);
    Q_UNUSED(column);
    hasChanges = true;

}
//----------------------------------------------------------------------------------------------------

void MainWindow::compare()
{
    treeOpener opener(treeWidget, this);
    opener.selectToCompare();
    analyzer->showDiffWith(opener.diffHashTable);

}
//----------------------------------------------------------------------------------------------------

void MainWindow::setWindowTitleToFileName()
{
    if(!filePath.isEmpty()){
        QFileInfo details;
        details.setFile(filePath);
        setWindowTitle(details.fileName());

    }else{
        setWindowTitle(QObject::tr(STRING_TESTCAD));

    }

}
//----------------------------------------------------------------------------------------------------

int MainWindow::notSavedWarningResponse()
{
    QMessageBox msgBox;
    msgBox.setText(STRING_UNSAVEDCHANGES_DIALOG_TITLE);
    msgBox.setInformativeText(STRING_SAVE_BEFORE);
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Cancel | QMessageBox::Ignore);
    msgBox.setDefaultButton(QMessageBox::Save);

    int decision = QMessageBox::Ignore;

    if(hasChanges)
    {
        decision = msgBox.exec();

    }

    return decision;
}
//----------------------------------------------------------------------------------------------------

void MainWindow::show()
{
    QMainWindow::show();
    showSplashWindow();

}
//----------------------------------------------------------------------------------------------------

void MainWindow::open()
{    
    int decision = notSavedWarningResponse();

    if(QMessageBox::Save == decision)
    {
        save();
        hasChanges=false;

    }else if(QMessageBox::Ignore == decision){
        openTreeFor(editing);
        hasChanges=false;

    }

}
//----------------------------------------------------------------------------------------------------

void MainWindow::append()
{
    openTreeFor(appending);
    hasChanges=true;

}
//----------------------------------------------------------------------------------------------------

void MainWindow::save()
{
    treeSaver saver(treeWidget, this);

    if (!filePath.isEmpty()){

        if(saver.saveTreeTo(filePath)){
            statusBar()->showMessage(QObject::tr(STRING_FILE_SAVED),2000);
            hasChanges = false;

        }else{
            statusBar()->showMessage(QObject::tr(STRING_FILE_NOT_SAVED),2000);

        }

    }else{
        saveAs();

    }
}
//----------------------------------------------------------------------------------------------------

void MainWindow::saveAs()
{
    treeSaver saver(treeWidget, this);

    if(saver.saveTreeAs()){
        filePath = saver.filePath;
        setWindowTitleToFileName();
        statusBar()->showMessage(QObject::tr(STRING_FILE_SAVED),2000);
        hasChanges = false;

    }else{
        statusBar()->showMessage(QObject::tr(STRING_FILE_NOT_SAVED),2000);

    }
}
//----------------------------------------------------------------------------------------------------

void MainWindow::closeTree()
{
    int decision = notSavedWarningResponse();

    if(QMessageBox::Save == decision)
    {
        save();
        hasChanges = false;

    }else if(QMessageBox::Ignore == decision)
    {
        treeWidget->clear();
        hasChanges = false;

    }

}
//----------------------------------------------------------------------------------------------------

void MainWindow::about()
{
    QMessageBox::about(this, tr(STRING_ABOUT_TESTCAD), tr(ABOUT_TEXT) + tr("Version:") + " " + tr(STRING_VERSION_NUMBER));

}
//----------------------------------------------------------------------------------------------------

void MainWindow::createActions()
{
    createFileActions();
    createInsertActions();
    createEditActions();
    createViewActions();
    createToolsActions();
    createTestActions();
    createHelpActions();
}
//----------------------------------------------------------------------------------------------------

void MainWindow::createFileActions()
{
    openAct = new QAction(icons->openIcon,tr(STRING_ACTION_OPEN), this);
    openAct->setShortcuts(QKeySequence::Open);
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    saveAct = new QAction(tr(STRING_ACTION_SAVE), this);
    saveAct->setIcon(icons->saveIcon);
    saveAct->setShortcuts(QKeySequence::Save);
    connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

    saveAsAct = new QAction(tr(STRING_ACTION_SAVE_AS), this);
    saveAsAct->setShortcuts(QKeySequence::SaveAs);
    saveAsAct->setIcon(icons->saveAsIcon);
    connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));

    appendAct = new QAction(QIcon(":/icons/appendIcon.png"),tr(STRING_ACTION_APPEND), this);
    connect(appendAct, SIGNAL(triggered()), this, SLOT(append()));

    closeTreeAct = new QAction(tr("Close"), this);
    connect(closeTreeAct, SIGNAL(triggered()), this, SLOT(closeTree()));

    exitAct = new QAction(tr(STRING_ACTION_EXIT), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));
}
//----------------------------------------------------------------------------------------------------

void MainWindow::createEditActions()
{
    markPendingAct = new QAction(tr(STRING_ACTION_MARK_PENDING), this);
    markPendingAct->setIcon(icons->pendingIcon);
    markPendingAct->setShortcut(QKeySequence(tr("Ctrl+E")));
    connect(markPendingAct, SIGNAL(triggered()), editor, SLOT(markAsPending()));

    markPlannedAct = new QAction(tr(STRING_ACTION_MARK_PLANNED), this);
    markPlannedAct->setIcon(icons->plannedIcon);
    markPlannedAct->setShortcut(QKeySequence(tr("Ctrl+A")));
    connect(markPlannedAct, SIGNAL(triggered()), editor, SLOT(markAsPlanned()));

    markValidatedAct = new QAction(tr(STRING_ACTION_MARK_VALIDATED), this);
    markValidatedAct->setIcon(icons->validatedIcon);
    markValidatedAct->setShortcut(QKeySequence(tr("Ctrl+P")));
    connect(markValidatedAct, SIGNAL(triggered()), editor, SLOT(markAsValidated()));

    markReviewAct = new QAction(tr(STRING_ACTION_MARK_REVIEW), this);
    markReviewAct->setIcon(icons->reviewIcon);
    markReviewAct->setShortcut(QKeySequence(tr("Ctrl+R")));
    connect(markReviewAct, SIGNAL(triggered()), editor, SLOT(markAsReview()));

    markFailed = new QAction(tr(STRING_ACTION_MARK_FAILED), this);
    markFailed->setIcon(icons->failedIcon);
    markFailed->setShortcut(QKeySequence(tr("Ctrl+L")));
    connect(markFailed, SIGNAL(triggered()), editor, SLOT(markAsFailed()));

    markUnsupported = new QAction(tr(STRING_ACTION_MARK_UNSUPPORTED), this);
    markUnsupported->setIcon(icons->unsupportedIcon);
    connect(markUnsupported, SIGNAL(triggered()), editor, SLOT(markAsUnsupported()));

    deleteAct = new QAction(tr(STRING_ACTION_DELETE), this);
    deleteAct->setIcon(icons->deleteIcon);
    deleteAct->setShortcut(QKeySequence::Delete);
    connect(deleteAct, SIGNAL(triggered()), editor, SLOT(removeItem()));

    undoAct = new QAction(tr(STRING_ACTION_UNDO), this);
    undoAct->setIcon(icons->undoIcon);
    undoAct->setShortcut(QKeySequence::Undo);
    connect(undoAct, SIGNAL(triggered()), editor, SLOT(undo()));

    copyAct = new QAction(tr(STRING_ACTION_COPY), this);
    copyAct->setIcon(icons->copyIcon);
    copyAct->setShortcut(QKeySequence::Copy);
    connect(copyAct, SIGNAL(triggered()), editor, SLOT(copy()));

    cutAct = new QAction(tr(STRING_ACTION_CUT), this);
    cutAct->setIcon(icons->cutIcon);
    cutAct->setShortcut(QKeySequence::Cut);
    connect(cutAct, SIGNAL(triggered()), editor, SLOT(cut()));

    pasteAct = new QAction(tr(STRING_ACTION_PASTE), this);
    pasteAct->setIcon(icons->pasteIcon);
    pasteAct->setShortcut(QKeySequence::Paste);
    connect(pasteAct, SIGNAL(triggered()), editor, SLOT(paste()));

    moveUpAct = new QAction(tr(STRING_ACTION_MOVE_UP), this);
    moveUpAct->setIcon(icons->upIcon);
    moveUpAct->setShortcut(QKeySequence(tr("Ctrl+U")));
    connect(moveUpAct, SIGNAL(triggered()), editor, SLOT(moveUp()));

    moveDownAct = new QAction(tr(STRING_ACTION_MOVE_DOWN), this);
    moveDownAct->setIcon(icons->downIcon);
    moveDownAct->setShortcut(QKeySequence(tr("Ctrl+D")));
    connect(moveDownAct, SIGNAL(triggered()), editor, SLOT(moveDown()));
}
//----------------------------------------------------------------------------------------------------

void MainWindow::createInsertActions()
{
    addComponentAct = new QAction(tr(STRING_ACTION_ADD_COMPONENT), this);
    addComponentAct->setIcon(icons->addTopIcon);
    connect(addComponentAct, SIGNAL(triggered()), editor, SLOT(addComponent()));

    addMultipleAct = new QAction(tr(STRING_ACTION_ADD), this);
    addMultipleAct->setIcon(icons->addIcon);
    connect(addMultipleAct, SIGNAL(triggered()), editor, SLOT(addMultiple()));

}
//----------------------------------------------------------------------------------------------------

void MainWindow::createViewActions()
{
    findFailedAct = new QAction(tr(STRING_ACTION_SHOW_FAILED), this);
    findFailedAct->setIcon(icons->findFailedIcon);
    connect(findFailedAct, SIGNAL(triggered()), editor, SLOT(showMissing()));

    findReviewAct = new QAction(tr(STRING_ACTION_SHOW_REVIEW), this);
    findReviewAct->setIcon(icons->findReviewIcon);
    connect(findReviewAct, SIGNAL(triggered()), editor, SLOT(showReview()));

    findPlannedAct = new QAction(tr(STRING_ACTION_SHOW_PLANNED), this);
    findPlannedAct->setIcon(icons->findPlannedIcon);
    connect(findPlannedAct, SIGNAL(triggered()), editor, SLOT(showPlanned()));

    findPendingAct = new QAction(tr(STRING_ACTION_SHOW_PENDING), this);
    findPendingAct->setIcon(icons->findPendingIcon);
    connect(findPendingAct, SIGNAL(triggered()), editor, SLOT(showPending()));

    findPassedAct = new QAction(tr(STRING_ACTION_SHOW_VALIDATED), this);
    findPassedAct->setIcon(icons->findPassedIcon);
    connect(findPassedAct, SIGNAL(triggered()), editor, SLOT(showValidated()));

    findUnsupportedAct = new QAction(tr(STRING_ACTION_SHOW_UNSUPPORTED), this);
    findUnsupportedAct->setIcon(icons->findUnsupportedIcon);
    connect(findUnsupportedAct, SIGNAL(triggered()), editor, SLOT(showUnsupported()));

    collapseAct = new QAction(tr(STRING_ACTION_COLLAPSE), this);
    connect(collapseAct, SIGNAL(triggered()), editor, SLOT(collapseSelected()));

    expandAct = new QAction(tr(STRING_ACTION_EXPAND), this);
    connect(expandAct, SIGNAL(triggered()), editor, SLOT(expandSelected()));

    collapseAllAct = new QAction(tr(STRING_ACTION_COLLAPSE_ALL), this);
    connect(collapseAllAct, SIGNAL(triggered()), editor, SLOT(collapseAll()));

    expandAllAct = new QAction(tr(STRING_ACTION_EXPAND_ALL), this);
    connect(expandAllAct, SIGNAL(triggered()), editor, SLOT(expandAll()));
}
//----------------------------------------------------------------------------------------------------

void MainWindow::search()
{
    editor->search();
    clearHighlightsAct->setVisible(true);
}
//----------------------------------------------------------------------------------------------------

void MainWindow::createToolsActions()
{
    searchAct = new QAction(tr(STRING_ACTION_SEARCH), this);
    searchAct->setIcon(icons->searchIcon);
    searchAct->setShortcut(QKeySequence::Find);
    connect(searchAct, SIGNAL(triggered()), this, SLOT(search()));

    copyPathClipAct = new QAction(QObject::tr(STRING_ACTION_COPY_PATH_TO_CLIPBOARD), this);
    copyPathClipAct->setIcon(icons->pathIcon);
    connect(copyPathClipAct, SIGNAL(triggered()), analyzer, SLOT(copyPathToClipBoard()));

    copyBranchClipAct= new QAction(QObject::tr(STRING_ACTION_COPY_BRANCH_TO_CLIPBOARD), this);
    copyBranchClipAct->setIcon(icons->copyBranchIcon);
    connect(copyBranchClipAct, SIGNAL(triggered()), analyzer, SLOT(copyBranchToClipboard()));

    showStatsAct = new QAction(QObject::tr(STRING_ACTION_SHOW_STATISTICS), this);
    showStatsAct->setIcon(icons->statsIcon);
    connect(showStatsAct, SIGNAL(triggered()), analyzer, SLOT(showStatistics()));

    showDiffAct = new QAction(QObject::tr(STRING_ACTION_COMPARE), this);
    connect(showDiffAct, SIGNAL(triggered()), this, SLOT(compare()));

    exportImageAct = new QAction(icons->pictureIcon, QObject::tr(STRING_ACTION_EXPORT_IMAGE), this);
    connect(exportImageAct, SIGNAL(triggered()), this, SLOT(exportImage()));

    clearHighlightsAct = new QAction(QObject::tr(STRING_ACTION_CLEAR_HIGHLIGHTS), this);
    clearHighlightsAct->setShortcut(QKeySequence(tr("Ctrl+H")));
    clearHighlightsAct->setIcon(icons->clearHighlightsIcon);
    clearHighlightsAct->setVisible(false);
    connect(clearHighlightsAct, SIGNAL(triggered()), this, SLOT(clearHighlights()));

    copyVariableStats = new QAction(QIcon(":/icons/statsIcon.png"),QObject::tr("Copy variable statistics"), this);
    connect(copyVariableStats, SIGNAL(triggered()), analyzer, SLOT(copyVariableStatisticsToClipboard()));

}
//----------------------------------------------------------------------------------------------------

void MainWindow::createTestActions()
{
    addTestGroupAct = new QAction(tr("Add a test group"), this);
    addTestGroupAct->setIcon(icons->testGroupIcon);
    connect(addTestGroupAct, SIGNAL(triggered()), editor, SLOT(addTestGroup()));

    addTestCollectionAct = new QAction(tr("Add a collection"), this);
    addTestCollectionAct->setIcon(icons->testCollectionIcon);
    connect(addTestCollectionAct, SIGNAL(triggered()), editor, SLOT(addTestCollection()));

    addTestCombinationAct = new QAction(tr("Add a combination"), this);
    addTestCombinationAct->setIcon(icons->testCombinationIcon);
    connect(addTestCombinationAct, SIGNAL(triggered()), editor, SLOT(addTestCombination()));

    addTestSequenceAct = new QAction(tr("Add a sequence"), this);
    addTestSequenceAct->setIcon(icons->testSequenceIcon);
    connect(addTestSequenceAct, SIGNAL(triggered()), editor, SLOT(addTestSequence()));

    openTestDesignerAct = new QAction(icons->designerIcon,tr("Open designer"), this);
    connect(openTestDesignerAct, SIGNAL(triggered()), this, SLOT(openDesignerWindow()));

    addTestCrossCheckerAct = new QAction(tr("Add a cross checker"), this);
    addTestCrossCheckerAct->setIcon(icons->testCrossCheckerIcon);
    connect(addTestCrossCheckerAct, SIGNAL(triggered()), editor, SLOT(addTestCrossChecker()));

}
//----------------------------------------------------------------------------------------------------

void MainWindow::createHelpActions()
{
    aboutAct = new QAction(tr("About..."), this);
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));
}
//----------------------------------------------------------------------------------------------------

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("File"));
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addAction(saveAct);
    fileMenu->addSeparator();
    fileMenu->addAction(appendAct);
    fileMenu->addSeparator();
    fileMenu->addAction(closeTreeAct);
    fileMenu->addAction(exitAct);
    fileMenu->addSeparator();
    historyMenu = new QMenu("Recent files...");
    fileMenu->addMenu(historyMenu);

    menuBar()->addSeparator();

    editMenu = menuBar()->addMenu(tr("Edit"));
    editMenu->addAction(addComponentAct);
    editMenu->addSeparator();
    editMenu->addAction(searchAct);
    editMenu->addAction(moveUpAct);
    editMenu->addAction(moveDownAct);
    editMenu->addSeparator();
    editMenu->addAction(markPlannedAct);
    editMenu->addAction(markValidatedAct);
    editMenu->addAction(markReviewAct);
    editMenu->addAction(markPendingAct);
    editMenu->addAction(markFailed);
    editMenu->addAction(markUnsupported);
    editMenu->addSeparator();
    editMenu->addAction(copyAct);
    editMenu->addAction(cutAct);
    editMenu->addAction(pasteAct);
    editMenu->addSeparator();
    editMenu->addAction(deleteAct);
    editMenu->addSeparator();
    editMenu->addAction(undoAct);

    menuBar()->addSeparator();

    viewMenu = menuBar()->addMenu(tr("View"));
    viewMenu->addAction(clearHighlightsAct);
    editMenu->addSeparator();
    viewMenu->addAction(findFailedAct);
    viewMenu->addAction(findReviewAct);
    viewMenu->addAction(findUnsupportedAct);
    viewMenu->addAction(findPendingAct);
    viewMenu->addAction(findPlannedAct);
    viewMenu->addAction(findPassedAct);
    viewMenu->addSeparator();
    viewMenu->addAction(expandAllAct);
    viewMenu->addAction(collapseAllAct);
    viewMenu->addSeparator();
    viewMenu->addAction(expandAct);
    viewMenu->addAction(collapseAct);

    menuBar()->addSeparator();

    toolsMenu = menuBar()->addMenu(tr("Tools"));
    toolsMenu->addAction(showStatsAct);
    toolsMenu->addSeparator();
    toolsMenu->addAction(exportImageAct);
    toolsMenu->addSeparator();
    toolsMenu->addAction(showDiffAct);

    menuBar()->addSeparator();

    testMenu = menuBar()->addMenu(tr("Test"));
    testMenu->addAction(addTestGroupAct);
    testMenu->addAction(addTestCollectionAct);
    testMenu->addAction(addTestCombinationAct);
    testMenu->addAction(addTestSequenceAct);
    testMenu->addAction(addTestCrossCheckerAct);
    testMenu->addSeparator();

    menuBar()->addSeparator();

    helpMenu = menuBar()->addMenu(tr("Help"));
    helpMenu->addAction(aboutAct);
}



