#include "Arrow.h"
#include "DiagramScene.h"
#include "DiagramBlock.h"
#include "DiagramConstructor.h"
#include "DiagramOperation.h"
#include "MainWindow.h"
#include <QtWidgets>
#include <iostream>
#include <QVector>
#include <QTextStream>
#include <typeinfo>

using namespace std;

MainWindow::MainWindow()
{
    createActions();
    createModulesList();
    createMenus();
    scene = new DiagramScene(panelModel, panelView, itemMenu, this);
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
    //QTextStream cout(stdout);
    string fileName = QFileDialog::getOpenFileName(this, tr("Open project"),
                                                    "",
                                                    tr("Dataflow projects (*.xml)")).toStdString();

    if (!fileName.empty())
    {
        projectModel = XMLParser().loadModelFromFile(fileName);
        panelModel = new ModulesPanelModel(projectModel);

        panelView->setModel(panelModel);

        setWindowTitle( QString(projectModel->getName().data()) + " - Dataflow Creator" );

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
                newBlock = new DiagramOperation((it->second), itemMenu);
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
                    DiagramBlock * blockWithOutput = scene->findBlockById(transition->second.outputBlock->id);
                    DiagramBlock * blockWithInput = scene->findBlockById(currentBlock->id);

                    BlockIn * startItem = blockWithInput->findInputByIndex(transition->first);
                    BlockOut * endItem = blockWithOutput->findOutputById(transition->second.outputId);
                    Arrow * arrow = new Arrow(startItem, endItem);
                    arrow->setColor(scene->myLineColor);
                    startItem->removeArrows();
                    startItem->addArrow(arrow);
                    endItem->addArrow(arrow);
                    scene->addItem(arrow);
                    arrow->updatePosition();

                    transition++;
                }
            }
            it++;
        }

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
    openFileAction = new QAction(tr("Open..."), this);
    openFileAction->setShortcut(tr("Ctrl+O"));
    openFileAction->setStatusTip(tr("Load project file"));
    connect(openFileAction, SIGNAL(triggered()), this, SLOT(openFile()));

    deleteAction = new QAction(QIcon(":/images/delete.png"), tr("&Delete"), this);
    deleteAction->setShortcut(tr("Delete"));
    deleteAction->setStatusTip(tr("Delete item from diagram"));
    connect(deleteAction, SIGNAL(triggered()), this, SLOT(deleteItem()));

    exitAction = new QAction(tr("E&xit"), this);
    exitAction->setShortcuts(QKeySequence::Quit);
    exitAction->setStatusTip(tr("Quit"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    aboutAction = new QAction(tr("A&bout"), this);
    aboutAction->setShortcut(tr("Ctrl+B"));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));
}
void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openFileAction);
    fileMenu->addAction(exitAction);
    itemMenu = menuBar()->addMenu(tr("&Item"));
    itemMenu->addAction(deleteAction);
    itemMenu->addSeparator();
    aboutMenu = menuBar()->addMenu(tr("&Help"));
    aboutMenu->addAction(aboutAction);
}
void MainWindow::createToolbars()
{
    editToolBar = addToolBar(tr("Edit"));
    editToolBar->addAction(deleteAction);
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
    pointerToolbar = addToolBar(tr("Pointer type"));
    pointerToolbar->addWidget(pointerButton);
    pointerToolbar->addWidget(linePointerButton);
}

