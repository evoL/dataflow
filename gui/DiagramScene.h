#ifndef DIAGRAMSCENE_H
#define DIAGRAMSCENE_H
#include "DiagramBlock.h"
#include "DiagramOperation.h"
#include "BlockIn.h"
#include "BlockOut.h"
#include "ModulesPanelModel.h"
#include <QGraphicsScene>
#include <QTreeView>

QT_BEGIN_NAMESPACE
class QGraphicsSceneMouseEvent;
class QMenu;
class QPointF;
class QGraphicsLineItem;
class QColor;
class QTreeView;
QT_END_NAMESPACE

class DiagramScene : public QGraphicsScene
{
    Q_OBJECT
public:
    enum Mode { InsertItem, InsertLine, MoveItem };
    explicit DiagramScene(ModulesPanelModel * panelModel, QTreeView * panelView, QMenu * itemMenu, QObject * parent = 0);
	void setProjectModel(ProjectModel * projectModel, ModelManipulator * modelManipulator) 
		{ this->projectModel = projectModel; this->manipulator = modelManipulator; }

	//DiagramBlock * findBlockById(int id);
	BlockIn * findInput(DiagramOperation * block, int index);
	BlockOut * findOutput(DiagramBlock * block, int id);
	bool paintConnection(int inputBlockId, int inputBlockInput, int outputBlockId, int outputBlockOutputId);
	
	
public slots:
    void setMode(Mode mode);
signals:
    void itemInserted();
    void itemSelected(QGraphicsItem * item);
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent * mouseEvent);
    void mouseMoveEvent(QGraphicsSceneMouseEvent * mouseEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent * mouseEvent);
private:
    QMenu * myItemMenu;
    QTreeView * panelView;
    ModulesPanelModel * panelModel;

	ProjectModel * projectModel;
	ModelManipulator * manipulator;

    Mode myMode;
    QPointF startPoint;
    QGraphicsLineItem * line;
    QColor myTextColor;
    QColor myLineColor;    
};
#endif // DIAGRAMSCENE_H
