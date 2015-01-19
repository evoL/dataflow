#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Arrow.h"
#include "DiagramScene.h"
#include "DiagramBlock.h"
#include "DiagramConstructor.h"
#include "DiagramOperation.h"
#include "ModulesPanelModel.h"
#include "DataflowModel.h"
#include "ModelManipulator.h"
#include "XMLParser.h"
#include "Interpreter.h"
#include "PreferencesDialog.h"

#include <QMainWindow>
#include <QScopedPointer>
#include <QAction>
#include <QScopedPointer>
#include <QtWidgets>
#include <QVector>
#include <QTextStream>

#include <iostream>
#include <typeinfo>

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
    QSize getSceneViewSize();
    void clickPointerButton() { pointerButton->click(); }
    void clickLinePointerButton() { linePointerButton->click(); }

public slots:
    void updateConstructorValue(int blockId, const QString & value);

private slots:
    void splitterMovedEvent(int pos, int index);
    void resizeEvent(QResizeEvent * event);
    void panelViewClicked();
    void panelViewCollapsedExpanded();
    void newProject();
    void openFile();
    void deleteItem();
    void pointerGroupClicked(int id);
    void itemInserted();
    void about();
    void execute();
    void saveFile();
    void saveAs();
    void toggleEntryPoint();
    void showProjectProperties();
    void openPanelMenu(const QPoint &);
    void removeLibrary();

private:
    void createModulesList();
    void createActions();
    void createMenus();
    void createToolbars();

    void saveModelAs(const QString & filename);
    void updateInterfaceState();
    void updateExecute();

    void addLibrary(const QString & name);

    QScopedPointer<DiagramScene> scene;
    QScopedPointer<QGraphicsView> sceneView;

    QScopedPointer<QAction> newProjectAction;
    QScopedPointer<QAction> openFileAction;
    QScopedPointer<QAction> exitAction;
    QScopedPointer<QAction> deleteAction;
    QScopedPointer<QAction> aboutAction;
    QScopedPointer<QAction> executeAction;
    QScopedPointer<QAction> saveAction;
    QScopedPointer<QAction> saveAsAction;
    QScopedPointer<QAction> entryPointAction;
    QScopedPointer<QAction> projectPropertiesAction;

    QScopedPointer<QMenu> fileMenu;
    QScopedPointer<QMenu> itemMenu;
    QScopedPointer<QMenu> aboutMenu;
    QScopedPointer<QToolBar> editToolBar;
    QScopedPointer<QToolBar> pointerToolbar;
    QScopedPointer<QToolButton> pointerButton;
    QScopedPointer<QToolButton> linePointerButton;
    QScopedPointer<QToolBar> fileToolbar;

    QScopedPointer<QButtonGroup> pointerTypeGroup;

    QString openedFileName;
    QScopedPointer<ProjectModel> projectModel;
    QScopedPointer<ModelManipulator> manipulator;

    QScopedPointer<ModulesPanelModel> panelModel;
    QScopedPointer<QTreeView> panelView;

    QScopedPointer<QPlainTextEdit> programOutputTextbox;
};
#endif // MAINWINDOW_H
