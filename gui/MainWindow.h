#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "DiagramModuleItem.h"
#include "ModulesListModel.h"
#include <QMainWindow>
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
class QListView;
QT_END_NAMESPACE
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow();
private slots:
    void modulesViewClicked();
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
    QAction * exitAction;
    QAction * deleteAction;
    QAction * aboutAction;
    QMenu * fileMenu;
    QMenu * itemMenu;
    QMenu * aboutMenu;
    QToolBar * editToolBar;
    QToolBar * pointerToolbar;
    ModulesListModel * modulesListModel;
    QListView * modulesView;
    QButtonGroup * pointerTypeGroup;
    QAction * lineAction;
};
#endif // MAINWINDOW_H
