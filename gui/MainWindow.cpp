#include "MainWindow.h"

using namespace std;

MainWindow::MainWindow()
{
    createActions();
    createModulesList();
    createMenus();

    scene.reset(new DiagramScene(panelModel.data(), panelView.data(), itemMenu.data(), this));
    connect(scene.data(), SIGNAL(itemInserted()), this, SLOT(itemInserted()));

    createToolbars();
    QSplitter * splitter = new QSplitter;
    splitter->addWidget(panelView.data());
    sceneView.reset(new QGraphicsView(scene.data()));
    sceneView->setSizeAdjustPolicy(QAbstractScrollArea::SizeAdjustPolicy::AdjustToContentsOnFirstShow);
    splitter->addWidget(sceneView.data());
    setCentralWidget(splitter);
    splitter->setSizes({ 190, 805 });
    connect(splitter, SIGNAL(splitterMoved(int, int)), this, SLOT(splitterMovedEvent(int,int)));

    setWindowTitle(tr("Dataflow Creator"));
    setUnifiedTitleAndToolBarOnMac(true);

    newProject();
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

    projectModel.reset(new ProjectModel(tr("Untitled").toStdString()));
    manipulator.reset(new ModelManipulator(*projectModel));
    panelModel.reset(new ModulesPanelModel(projectModel.data()));
    updateInterfaceState();
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
        updateInterfaceState();
        updateExecute();

        openedFileName = QString::fromStdString(fileName);

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
                newBlock = new DiagramConstructor((it->second), projectModel->getLibraries(), itemMenu.data());
                connect(static_cast<DiagramConstructor *>(newBlock), &DiagramConstructor::valueChanged, this, &MainWindow::updateConstructorValue);
            }
            if (it->second->blockType() == BlockType::Operation)
            {
                newBlock = new DiagramOperation(projectModel.data(), (it->second), itemMenu.data());
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

    updateExecute();
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

void MainWindow::showProjectProperties()
{
	PreferencesDialog window(projectModel.data(), manipulator.data(), this);
	if (window.exec() == QDialog::Accepted)
		updateInterfaceState();
}


void MainWindow::openPanelMenu(const QPoint & pos)
{
    static QList<QString> libraries{ "basicmath", "dstring", "io_module", "ppm" };
    auto & includedLibraries = projectModel->getLibraries();

    QMenu libraryMenu;
    libraryMenu.setTitle("Add library");
    foreach (auto &libraryName, libraries) {
        QAction * action = libraryMenu.addAction(libraryName);

        if (includedLibraries.find(libraryName.toStdString()) != includedLibraries.end()) {
            action->setEnabled(false);
        }

        connect(action, &QAction::triggered, [&]{ addLibrary(libraryName); });
    }

    QMenu menu;
    menu.addMenu(&libraryMenu);
    menu.exec(panelView->mapToGlobal(pos));
}

void MainWindow::updateConstructorValue(int blockId, const QString & value)
{
    try {
        manipulator->setConstructorData(blockId, value.toStdString());
    } catch (ModelManipulatorError e) {
        QMessageBox::critical(this, tr("Dataflow Creator"), QString::fromUtf8(e.what()));
    }
}

void MainWindow::addLibrary(const QString & name)
{
    try {
        std::string stdName = name.toStdString();
        manipulator->addLibrary(stdName);

        const Library &library = projectModel->getLibraries().at(stdName);
        panelModel->addLibrary(library);
    } catch (ModelManipulatorError e) {
        QMessageBox::critical(this, tr("Dataflow Creator"), QString::fromUtf8(e.what()));
    }
}

void MainWindow::createModulesList()
{
    panelView.reset(new QTreeView());
    panelView->setContextMenuPolicy(Qt::CustomContextMenu);
    panelView->setHeaderHidden(true);
    panelView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    panelView->setSelectionMode(QAbstractItemView::SingleSelection);
    panelView->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Ignored);
    panelView->setMinimumWidth(150);
    connect(panelView.data(), SIGNAL(clicked(const QModelIndex &)),
        this, SLOT(panelViewClicked()));
    connect(panelView.data(), SIGNAL(collapsed(const QModelIndex &)),
        this, SLOT(panelViewCollapsedExpanded()));
    connect(panelView.data(), SIGNAL(expanded(const QModelIndex &)),
        this, SLOT(panelViewCollapsedExpanded()));
    connect(panelView.data(), SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(openPanelMenu(const QPoint &)));
}

void MainWindow::createActions()
{
    openFileAction.reset(new QAction(QIcon(":/images/open.png"), tr("&Open..."), this));
    openFileAction->setShortcut(tr("Ctrl+O"));
    openFileAction->setStatusTip(tr("Load project file"));
    connect(openFileAction.data(), SIGNAL(triggered()), this, SLOT(openFile()));

    deleteAction.reset(new QAction(QIcon(":/images/delete.png"), tr("&Delete"), this));
    deleteAction->setShortcuts(QList<QKeySequence>{ tr("Delete"), tr("Backspace") });
    deleteAction->setStatusTip(tr("Delete item from diagram"));
    connect(deleteAction.data(), SIGNAL(triggered()), this, SLOT(deleteItem()));

    exitAction.reset(new QAction(tr("E&xit"), this));
    exitAction->setShortcuts(QKeySequence::Quit);
    exitAction->setStatusTip(tr("Quit"));
    connect(exitAction.data(), SIGNAL(triggered()), this, SLOT(close()));

    aboutAction.reset(new QAction(tr("A&bout"), this));
    connect(aboutAction.data(), SIGNAL(triggered()), this, SLOT(about()));

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

	projectPropertiesAction.reset(new QAction(QIcon(":/images/gear.png"), tr("Project properties"), this));
	projectPropertiesAction->setShortcut(QKeySequence::Preferences);
	connect(projectPropertiesAction.data(), SIGNAL(triggered()), this, SLOT(showProjectProperties()));
}

void MainWindow::createMenus()
{
    fileMenu.reset(menuBar()->addMenu(tr("&File")));
    fileMenu->addAction(openFileAction.data());
    fileMenu->addAction(saveAction.data());
    fileMenu->addAction(saveAsAction.data());
    fileMenu->addSeparator();
    fileMenu->addAction(executeAction.data());
	fileMenu->addAction(projectPropertiesAction.data());
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction.data());

    itemMenu.reset(menuBar()->addMenu(tr("&Item")));
    itemMenu->addAction(entryPointAction.data());
    itemMenu->addSeparator();
    itemMenu->addAction(deleteAction.data());

    aboutMenu.reset(menuBar()->addMenu(tr("&Help")));
    aboutMenu->addAction(aboutAction.data());
}

void MainWindow::createToolbars()
{
    pointerButton.reset(new QToolButton);
    pointerButton->setCheckable(true);
    pointerButton->setChecked(true);
    pointerButton->setIcon(QIcon(":/images/pointer.png"));
    linePointerButton.reset(new QToolButton);
    linePointerButton->setCheckable(true);
    linePointerButton->setIcon(QIcon(":/images/linepointer.png"));
    pointerTypeGroup.reset(new QButtonGroup(this));
    pointerTypeGroup->addButton(pointerButton.data(), int(DiagramScene::MoveItem));
    pointerTypeGroup->addButton(linePointerButton.data(), int(DiagramScene::InsertLine));
    connect(pointerTypeGroup.data(), SIGNAL(buttonClicked(int)),
            this, SLOT(pointerGroupClicked(int)));

    fileToolbar.reset(addToolBar(tr("File")));
    fileToolbar->addAction(openFileAction.data());
    fileToolbar->addAction(saveAction.data());
    fileToolbar->addAction(executeAction.data());

    pointerToolbar.reset(addToolBar(tr("Pointer type")));
    pointerToolbar->addWidget(pointerButton.data());
    pointerToolbar->addWidget(linePointerButton.data());

    editToolBar.reset(addToolBar(tr("Edit")));
    editToolBar->addAction(deleteAction.data());
}

void MainWindow::saveModelAs(const QString & filename)
{
    try {
        XMLParser().saveModelToFile(*projectModel, filename.toStdString());
    } catch (XMLParserError e) {
        QMessageBox::critical(this, tr("Dataflow Project"), e.what());
    }
}

void MainWindow::updateInterfaceState()
{
    scene->setModels(panelModel.data(), projectModel.data(), manipulator.data());
    panelView->setModel(panelModel.data());

    setWindowTitle( QString::fromStdString(projectModel->getName()) + " - Dataflow Creator" );
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
