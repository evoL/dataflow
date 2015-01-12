#include "MainWindow.h"

using namespace std;

MainWindow::MainWindow()
{
    createActions();
    createModulesList();
    createMenus();
    scene = new DiagramScene(panelModel.data(), panelView, itemMenu, this);
    scene->setSceneRect(QRectF(0, 0, 1000, 1000));
    connect(scene, SIGNAL(itemInserted()), this, SLOT(itemInserted()));
    createToolbars();
    QSplitter * splitter = new QSplitter;
    splitter->addWidget(panelView);
    view = new QGraphicsView(scene);

    splitter->addWidget(view);
    setCentralWidget(splitter);
    setWindowTitle(tr("Dataflow Creator"));
    setUnifiedTitleAndToolBarOnMac(true);
    //openFile();
    //view->verticalScrollBar()->setSliderPosition(0);
    //view->horizontalScrollBar()->setSliderPosition(0);
    //view->verticalScrollBar()->setValue(1);
    //view->horizontalScrollBar()->setValue(1);
}

void MainWindow::panelViewClicked()
{
    scene->setMode(DiagramScene::InsertItem);
}

void MainWindow::panelViewCollapsedExpanded()
{
    panelView->resizeColumnToContents(0);
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
        scene->setModels(panelModel.data(), projectModel.data(), manipulator.data());

        panelView->setModel(panelModel.data());

        openedFileName = QString::fromStdString(fileName);

        setWindowTitle( QString(projectModel->getName().data()) + " - Dataflow Creator" );
        executeAction->setEnabled(true);

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
                newBlock = new DiagramConstructor((it->second), itemMenu);
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
                    if (transition->second.outputBlock != nullptr) // Assumption: not connected input has nullptr as outputBlock
                    {
                        bool paintingOK = scene->paintConnection(currentBlock->id,
                            transition->first,
                            transition->second.outputBlock->id,
                            transition->second.outputId);
                        if (!paintingOK) throw std::string("Can't connect blocks!");
                    }
                    transition++;
                }
            }
            it++;
        }
    } catch (XMLParserError e) {
        QMessageBox::critical(this, tr("Dataflow Creator"), e.what());
    }
}

void MainWindow::deleteItem()
{
    foreach (QGraphicsItem * item, scene->selectedItems()) {
        if (item->type() == Arrow::Type) {
            scene->removeItem(item);
            Arrow * arrow = qgraphicsitem_cast<Arrow *>(item);
            arrow->startItem()->removeArrow(arrow);
            arrow->endItem()->removeArrow(arrow);
            delete item;
        }
    }

    foreach (QGraphicsItem * item, scene->selectedItems()) {
        if (item->type() == DiagramBlock::Type) {
            qgraphicsitem_cast<DiagramBlock *>(item)->removeArrows();
            scene->removeItem(item);
            delete item;
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

void MainWindow::createModulesList()
{
    panelView = new QTreeView();
    //panelView->setModel(panelModel);
    panelView->setHeaderHidden(true);
    panelView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    panelView->setSelectionMode(QAbstractItemView::SingleSelection);
    panelView->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Ignored);
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
    itemMenu->addAction(deleteAction);
    itemMenu->addSeparator();

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
