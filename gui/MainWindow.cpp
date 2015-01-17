#include "MainWindow.h"

using namespace std;

MainWindow::MainWindow()
    : projectModel(new ProjectModel())
{
    createActions();
    createModulesList();
    createMenus();

    scene = new DiagramScene(panelModel.data(), panelView, itemMenu, this);
    connect(scene, SIGNAL(itemInserted()), this, SLOT(itemInserted()));

    createToolbars();
    QSplitter * splitter = new QSplitter;
    splitter->addWidget(panelView);
    sceneView = new QGraphicsView(scene);
    sceneView->setSizeAdjustPolicy(QAbstractScrollArea::SizeAdjustPolicy::AdjustToContentsOnFirstShow);
    splitter->addWidget(sceneView);
    setCentralWidget(splitter);
    splitter->setSizes({ 190, 805 });
    connect(splitter, SIGNAL(splitterMoved(int, int)), this, SLOT(splitterMovedEvent(int,int)));

    setWindowTitle(tr("Dataflow Creator"));
    setUnifiedTitleAndToolBarOnMac(true);
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
    QMainWindow::resizeEvent(event);
    scene->setSceneSizeAndGradient(scene->getSizeHint());
}

void MainWindow::splitterMovedEvent(int pos, int index)
{
    scene->setSceneSizeAndGradient(scene->getSizeHint());
}

void MainWindow::panelViewClicked()
{
    scene->setMode(DiagramScene::InsertItem);
}

void MainWindow::panelViewCollapsedExpanded()
{
    panelView->resizeColumnToContents(0);
}

void MainWindow::newProject()
{
    // TODO: if a project is open, ask to save

    projectModel.reset(new ProjectModel());
    manipulator.reset(new ModelManipulator(*projectModel));
    panelModel.reset(new ModulesPanelModel(projectModel.data()));
    updateModels();
}

void MainWindow::openFile()
{
    string fileName = QFileDialog::getOpenFileName(this, tr("Open project"),
                                                    "",
                                                    tr("Dataflow projects (*.xml)")).toStdString();

    if (fileName.empty()) return;

    try {
        projectModel.reset(XMLParser().loadModelFromFile(fileName));
        manipulator.reset(new ModelManipulator(*projectModel));
        panelModel.reset(new ModulesPanelModel(projectModel.data()));
        updateModels();
        updateExecute();

        openedFileName = QString::fromStdString(fileName);

        setWindowTitle( QString(projectModel->getName().data()) + " - Dataflow Creator" );

        // Clear scene before redraw
        scene->clear();

        // Load blocks into diagram scene
        const BlocksMap blocks = projectModel->getBlocks();
        BlocksMap::const_iterator it = blocks.cbegin();

        DiagramBlock *newBlock;
        while (it != blocks.cend())
        {
            if (it->second->blockType() == BlockType::Constructor)
            {
                newBlock = new DiagramConstructor((it->second), projectModel->getLibraries(), itemMenu);
            }
            if (it->second->blockType() == BlockType::Operation)
            {
                newBlock = new DiagramOperation(projectModel.data(), (it->second), itemMenu);
            }

            scene->addItem(newBlock);
            newBlock->setPos(newBlock->getX(), newBlock->getY());
            if (newBlock->pos().x() < 0) newBlock->setX(0);
            if (newBlock->pos().y() < 0) newBlock->setY(0);
            emit itemInserted();

            it++;
        }

        // Load connections
        it = blocks.cbegin();
        while (it != blocks.cend())
        {
            if (it->second->blockType() == BlockType::Operation)
            {
                std::shared_ptr<Operation> currentBlock = std::static_pointer_cast<Operation>(it->second);
                const InputTransitionMap transitions = currentBlock->inputs;
                InputTransitionMap::const_iterator transition = transitions.cbegin();
                while (transition != transitions.cend())
                {
                    bool paintingOK = scene->paintConnectionWhenLoadingProject(currentBlock->id,
                        transition->first,
                        transition->second.outputBlock->id,
                        transition->second.outputId);
                    if (!paintingOK) throw std::string("Can't connect blocks!");

                    transition++;
                }
            }
            it++;
        }

        scene->setSceneSizeAndGradient(scene->getSizeHint());
        sceneView->horizontalScrollBar()->setValue(0);
        sceneView->verticalScrollBar()->setValue(0);
    } catch (XMLParserError e) {
        QMessageBox::critical(this, tr("Dataflow Creator"), e.what());
    }
    catch (ModelManipulatorError e) {
        QMessageBox::critical(this, tr("Dataflow Creator"), e.what());
    }
    catch (std::string e){
        QMessageBox::critical(this, tr("Dataflow Creator"), QString::fromStdString(e));
    }
}

void MainWindow::deleteItem()
{
    Arrow * arrow = NULL;
    DiagramOperation * inputBlock = NULL;
    BlockIn * input = NULL;

    foreach(QGraphicsItem * item, scene->selectedItems()) {
        switch (item->type())
        {
        case Arrow::Type:
            arrow = static_cast<Arrow *>(item);
            input = static_cast<BlockIn*>(arrow->startItem());
            inputBlock = static_cast<DiagramOperation*>(input->parentItem());

            manipulator->deleteConnection(inputBlock->getId(), input->getIndex());

            scene->removeItem(item);
            arrow->startItem()->removeArrow(arrow);
            arrow->endItem()->removeArrow(arrow);
            delete item;
            break;

        case DiagramOperation::Type:
        case DiagramConstructor::Type:
            static_cast<DiagramBlock *>(item)->removeArrows();
            manipulator->deleteBlock(static_cast<DiagramBlock *>(item)->getId());
            scene->removeItem(item);
            delete item;
            break;
        default:
            break;
        }
    }
}

void MainWindow::pointerGroupClicked(int)
{
    scene->setMode(DiagramScene::Mode(pointerTypeGroup->checkedId()));
}

void MainWindow::itemInserted()
{
    pointerTypeGroup->button(int(DiagramScene::MoveItem))->setChecked(true);
    scene->setMode(DiagramScene::Mode(pointerTypeGroup->checkedId()));
    panelView->clearSelection();
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About Dataflow"),
                       tr("About Dataflow Project"));
}

void MainWindow::execute()
{
    // TODO: we're assuming the Manipulator will be constantly checking for
    //       correctness, so I don't add it here

    try {
        Interpreter interpreter(*projectModel);
        interpreter.interpret();
    } catch (InterpreterError e) {
        QMessageBox::critical(this, tr("Runtime error"), e.what());
    }
}

void MainWindow::saveFile()
{
    if (openedFileName.isEmpty()) {
        saveAs();
    } else {
        saveModelAs(openedFileName);
    }
}

void MainWindow::saveAs()
{
    QString dir;
    if (!openedFileName.isEmpty()) {
        dir = QFileInfo(openedFileName).dir().absolutePath();
    }

    QString filename = QFileDialog::getSaveFileName(this, tr("Save project"), dir, tr("Dataflow projects (*.xml)"));

    if (filename.isEmpty()) return;

    saveModelAs(filename);
}

void MainWindow::toggleEntryPoint()
{
    // FIXME: This actually makes no sense for multiple selected blocks.
    // When implementing multiple selections, change this to toggle
    // according to the first selected item.

    foreach (QGraphicsItem * item, scene->selectedItems()) {
        // Only operations can be entry points
        if (item->type() != DiagramOperation::Type) continue;

        auto operation = qgraphicsitem_cast<DiagramOperation *>(item);
        if (operation->isEntryPoint()) {
            manipulator->unsetEntryPoint(operation->getId());
        } else {
            manipulator->setEntryPoint(operation->getId());
        }
        operation->updateAppearance();
    }

    updateExecute();
}

void MainWindow::createModulesList()
{
    panelView = new QTreeView();
    //panelView->setModel(panelModel);
    panelView->setHeaderHidden(true);
    panelView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    panelView->setSelectionMode(QAbstractItemView::SingleSelection);
    panelView->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Ignored);
    panelView->setMinimumWidth(150);
    connect(panelView, SIGNAL(clicked(const QModelIndex &)),
        this, SLOT(panelViewClicked()));
    connect(panelView, SIGNAL(collapsed(const QModelIndex &)),
        this, SLOT(panelViewCollapsedExpanded()));
    connect(panelView, SIGNAL(expanded(const QModelIndex &)),
        this, SLOT(panelViewCollapsedExpanded()));
}

void MainWindow::createActions()
{
    openFileAction = new QAction(QIcon(":/images/open.png"), tr("&Open..."), this);
    openFileAction->setShortcut(tr("Ctrl+O"));
    openFileAction->setStatusTip(tr("Load project file"));
    connect(openFileAction, SIGNAL(triggered()), this, SLOT(openFile()));

    deleteAction = new QAction(QIcon(":/images/delete.png"), tr("&Delete"), this);
    deleteAction->setShortcuts(QList<QKeySequence>{ tr("Delete"), tr("Backspace") });
    deleteAction->setStatusTip(tr("Delete item from diagram"));
    connect(deleteAction, SIGNAL(triggered()), this, SLOT(deleteItem()));

    exitAction = new QAction(tr("E&xit"), this);
    exitAction->setShortcuts(QKeySequence::Quit);
    exitAction->setStatusTip(tr("Quit"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    aboutAction = new QAction(tr("A&bout"), this);
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

    executeAction.reset(new QAction(QIcon(":/images/run.png"), tr("&Execute"), this));
    executeAction->setShortcut(tr("Ctrl+E"));
    executeAction->setEnabled(false);
    connect(executeAction.data(), SIGNAL(triggered()), this, SLOT(execute()));

    saveAction.reset(new QAction(QIcon(":/images/save.png"), tr("&Save"), this));
    saveAction->setShortcut(QKeySequence::Save);
    connect(saveAction.data(), SIGNAL(triggered()), this, SLOT(saveFile()));

    saveAsAction.reset(new QAction(QIcon(":/images/save-as.png"), tr("Save as..."), this));
    saveAsAction->setShortcut(QKeySequence::SaveAs);
    connect(saveAsAction.data(), SIGNAL(triggered()), this, SLOT(saveAs()));

    entryPointAction.reset(new QAction(tr("Entry point"), this));
    entryPointAction->setCheckable(true);
    entryPointAction->setEnabled(false);
    connect(entryPointAction.data(), SIGNAL(triggered()), this, SLOT(toggleEntryPoint()));
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openFileAction);
    fileMenu->addAction(saveAction.data());
    fileMenu->addAction(saveAsAction.data());
    fileMenu->addSeparator();
    fileMenu->addAction(executeAction.data());
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    itemMenu = menuBar()->addMenu(tr("&Item"));
    itemMenu->addAction(entryPointAction.data());
    itemMenu->addSeparator();
    itemMenu->addAction(deleteAction);

    aboutMenu = menuBar()->addMenu(tr("&Help"));
    aboutMenu->addAction(aboutAction);
}

void MainWindow::createToolbars()
{
    QToolButton * pointerButton = new QToolButton;
    pointerButton->setCheckable(true);
    pointerButton->setChecked(true);
    pointerButton->setIcon(QIcon(":/images/pointer.png"));
    QToolButton * linePointerButton = new QToolButton;
    linePointerButton->setCheckable(true);
    linePointerButton->setIcon(QIcon(":/images/linepointer.png"));
    pointerTypeGroup = new QButtonGroup(this);
    pointerTypeGroup->addButton(pointerButton, int(DiagramScene::MoveItem));
    pointerTypeGroup->addButton(linePointerButton, int(DiagramScene::InsertLine));
    connect(pointerTypeGroup, SIGNAL(buttonClicked(int)),
            this, SLOT(pointerGroupClicked(int)));

    fileToolbar = addToolBar(tr("File"));
    fileToolbar->addAction(openFileAction);
    fileToolbar->addAction(saveAction.data());
    fileToolbar->addAction(executeAction.data());

    pointerToolbar = addToolBar(tr("Pointer type"));
    pointerToolbar->addWidget(pointerButton);
    pointerToolbar->addWidget(linePointerButton);

    editToolBar = addToolBar(tr("Edit"));
    editToolBar->addAction(deleteAction);
}

void MainWindow::saveModelAs(const QString & filename)
{
    try {
        XMLParser().saveModelToFile(*projectModel, filename.toStdString());
    } catch (XMLParserError e) {
        QMessageBox::critical(this, tr("Dataflow Project"), e.what());
    }
}

void MainWindow::updateModels()
{
    scene->setModels(panelModel.data(), projectModel.data(), manipulator.data());
    panelView->setModel(panelModel.data());
}

void MainWindow::updateExecute()
{
    bool entryPointsExist = !projectModel->getEntryPoints().empty();

    executeAction->setEnabled(entryPointsExist);
}

QSize MainWindow::getSceneViewSize()
{
    return sceneView->size();
}
