#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>

#include "DiagramModuleItem.h"
#include "ModulesPanelModel.h"
#include <DataflowModel.h>
#include <XMLParser.h>

class DiagramScene;

QT_BEGIN_NAMESPACE
class QAction;
class QToolBox;
class QButtonGroup;
class QLineEdit;
class QGraphicsTextItem;
class QToolButton;
class QAbstractButton;
class QGraphicsView;
class QStringListModel;
class QTreeView;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow();
private slots:
    void panelViewClicked();
	void panelViewCollapsedExpanded();
    void openFile();
    void deleteItem();
    void pointerGroupClicked(int id);
    void itemInserted();
    void about();
private:
    void createModulesList();
    void createActions();
    void createMenus();
    void createToolbars();

    DiagramScene * scene;
    QGraphicsView * view;

    QAction * openFileAction;
    QAction * exitAction;
    QAction * deleteAction;
    QAction * aboutAction;
    QMenu * fileMenu;
    QMenu * itemMenu;
    QMenu * aboutMenu;
    QToolBar * editToolBar;
    QToolBar * pointerToolbar;
    
    QButtonGroup * pointerTypeGroup;
    QAction * lineAction;

    ProjectModel * projectModel;
	ModulesPanelModel * panelModel;
	QTreeView * panelView;
};
#endif // MAINWINDOW_H
