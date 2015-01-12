#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>

#include "DiagramBlock.h"
#include "ModulesPanelModel.h"
#include <DataflowModel.h>
#include <ModelManipulator.h>
#include <XMLParser.h>
#include <QAction>
#include <QScopedPointer>

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
class QPushButton;
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
    void execute();
    void saveFile();
    void saveAs();
private:
    void createModulesList();
    void createActions();
    void createMenus();
    void createToolbars();
    
    void saveModelAs(const QString & filename);

    DiagramScene * scene;
    QGraphicsView * view;

    QAction * openFileAction;
    QAction * exitAction;
    QAction * deleteAction;
    QAction * aboutAction;
    QScopedPointer<QAction> executeAction;
    QScopedPointer<QAction> saveAction;
    QScopedPointer<QAction> saveAsAction;

    QMenu * fileMenu;
    QMenu * itemMenu;
    QMenu * aboutMenu;
    QToolBar * editToolBar;
    QToolBar * pointerToolbar;
    QToolBar * fileToolbar;

    QButtonGroup * pointerTypeGroup;
    QAction * lineAction;

    QString openedFileName;
    QScopedPointer<ProjectModel> projectModel;
    QScopedPointer<ModelManipulator> manipulator;

    QScopedPointer<ModulesPanelModel> panelModel;
    QTreeView * panelView;
};
#endif // MAINWINDOW_H
