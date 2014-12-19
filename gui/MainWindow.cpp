#include "Arrow.h"
#include "DiagramScene.h"
#include "DiagramModuleItem.h"
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
    scene->setSceneRect(QRectF(0, 0, 3000, 3000));
    connect(scene, SIGNAL(itemInserted()), this, SLOT(itemInserted()));
    createToolbars();
    QHBoxLayout * layout = new QHBoxLayout;
    layout->addWidget(panelView);
    view = new QGraphicsView(scene);
    layout->addWidget(view);
    QWidget * widget = new QWidget;
    widget->setLayout(layout);
    setCentralWidget(widget);
    setWindowTitle(tr("Dataflow Creator"));
    setUnifiedTitleAndToolBarOnMac(true);
}
void MainWindow::panelViewClicked()
{
    scene->setMode(DiagramScene::InsertItem);
}

void MainWindow::openFile()
{
    //QTextStream cout(stdout);
    string fileName = QFileDialog::getOpenFileName(this, tr("Open project"),
                                                    "",
                                                    tr("Dataflow projects (*.xml)")).toStdString();

    if (!fileName.empty())
    {
        cout<<fileName<<endl;
        projectModel = XMLParser().loadModelFromFile(fileName);
		panelModel = new ModulesPanelModel(projectModel);

		panelView->setModel(panelModel);
		

        setWindowTitle( QString(projectModel->getName().data()) + " - Dataflow Creator" );
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
        if (item->type() == DiagramModuleItem::Type) {
            qgraphicsitem_cast<DiagramModuleItem *>(item)->removeArrows();

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

    /*modulesListModel = new ModulesListModel();
    // Adding some "operations" to the model.
    // Need to be removed when modules importing is implemented.

    QVector <QString> multInput;
    multInput.append("a");
    multInput.append("b");

    QVector <QString> addInput;
    addInput.append("a");
    addInput.append("b");
    addInput.append("c");

    QVector <QString> multOutput;
    multOutput.append("a*b");

    QVector <QString> addOutput;
    addOutput.append("a+b+c");

    QVector <QString> inOut;
    inOut.append("Number");

    QVector <QString> s;
    s.append("IN_1");
    s.append("IN_2");
    s.append("IN_3");

    QVector <QString> t;
    t.append("OUT_1");
    t.append("OUT_2");

    QVector <QString> empty;
    modulesListModel->append(Module("Give me a number", QSize(80, 80), empty, inOut,-1));
    modulesListModel->append(Module("Result is:", QSize(80, 80), inOut,empty,1));
    modulesListModel->append(Module("Multiply 2 numbers", QSize(80, 80), multInput, multOutput));
    modulesListModel->append(Module("Add 3 numbers", QSize(80, 80), addInput, addOutput));

    //modulesListModel->append(Module("Module 1 is very, very, very long and it still looks nice", QSize(100, 50), s, t));
    //modulesListModel->append(Module("Module 2", QSize(80, 80), s, t,-1));
    //modulesListModel->append(Module("Module 3", QSize(120, 120), s, t));
    panelView = new QListView();
    panelView->setModel(modulesListModel);
    panelView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    panelView->setSelectionMode(QAbstractItemView::SingleSelection);
    panelView->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Ignored);
    panelView->setMinimumWidth(150);
    connect(panelView, SIGNAL(clicked(const QModelIndex &)),
            this, SLOT(panelViewClicked()));*/
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

