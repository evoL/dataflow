#include "DiagramScene.h"
#include "Arrow.h"
#include "BlockIn.h"
#include "BlockOut.h"
#include <QTextCursor>
#include <QGraphicsSceneMouseEvent>
#include <QColor>
#include <QBrush>

DiagramScene::DiagramScene(ModulesPanelModel * panelModel, QTreeView * panelView, QMenu * itemMenu, QObject * parent)
    : QGraphicsScene(parent)
{
    myItemMenu = itemMenu;
    this->panelView = panelView;
    this->panelModel = panelModel;
    myMode = MoveItem;
    line = 0;
    
    myTextColor = Qt::black;
    myLineColor = Qt::black;


    QLinearGradient linearGrad(QPointF(0, 0), QPointF(3000, 3000));
    linearGrad.setColorAt(0, QColor(175, 175, 175));
    linearGrad.setColorAt(1, QColor(230, 230, 230));

    setBackgroundBrush(QBrush(linearGrad));
}

DiagramBlock * DiagramScene::findBlockById(int id)
{
    for (auto & item : items()) {
        if (item->type() != DiagramBlock::Type) continue;
        
        DiagramBlock * block = qgraphicsitem_cast<DiagramBlock*>(item);
        if (block != nullptr && block->getId() == id)
            return block;
    }
    
	return NULL;
}

void DiagramScene::setMode(Mode mode)
{
    myMode = mode;
}
void DiagramScene::mousePressEvent(QGraphicsSceneMouseEvent * mouseEvent)
{
    if (mouseEvent->button() != Qt::LeftButton)
        return;

    DiagramBlock * item;
    const Block * selectedBlockPtr;

    switch (myMode) {
    //poprawić nazwy!!!!!
    /*case InsertItem:
        selectedBlockPtr = &panelModel->at(panelView->currentIndex().row());
        item = new DiagramModuleItem(selectedBlockPtr, myItemMenu);

        if(selectedBlockPtr->getType()==0)item->setBrush(myItemColor);
        if(selectedBlockPtr->getType()==-1)item->setBrush(myInputColor);
        if(selectedBlockPtr->getType()==1)item->setBrush(myOutputColor);
        addItem(item);

        item->setPos(mouseEvent->scenePos() - QPointF(20,20));
        if (item->pos().x() < 0) item->setX(0);
        if (item->pos().y() < 0) item->setY(0);

        emit itemInserted();
        break;*/

    case InsertLine:
        line = new QGraphicsLineItem(QLineF(mouseEvent->scenePos(),
                                            mouseEvent->scenePos()));
        line->setPen(QPen(myLineColor, 2));
        addItem(line);
        break;

    default:
        break;
    }

    QGraphicsScene::mousePressEvent(mouseEvent);
}
void DiagramScene::mouseMoveEvent(QGraphicsSceneMouseEvent * mouseEvent)
{
    if (myMode == InsertLine && line != 0) {
        QLineF newLine(line->line().p1(), mouseEvent->scenePos());
        line->setLine(newLine);
    } else if (myMode == MoveItem) {
        QGraphicsScene::mouseMoveEvent(mouseEvent);
    }
}
void DiagramScene::mouseReleaseEvent(QGraphicsSceneMouseEvent * mouseEvent)
{
    if (line != 0 && myMode == InsertLine) {
        QList<QGraphicsItem *> startItems = items(line->line().p1());

        if (startItems.count() && startItems.first() == line)
            startItems.removeFirst();

        QList<QGraphicsItem *> endItems = items(line->line().p2());

        if (endItems.count() && endItems.first() == line)
            endItems.removeFirst();

        removeItem(line);
        delete line;

        //from end to start
        if (startItems.count() > 0 && endItems.count() > 0 &&
            startItems.first()->type() == BlockIn::Type &&
            endItems.first()->type() == BlockOut::Type &&
            startItems.first() != endItems.first()) {
            BlockIn * startItem = qgraphicsitem_cast<BlockIn *>(startItems.first());
            BlockOut * endItem = qgraphicsitem_cast<BlockOut *>(endItems.first());
            Arrow * arrow = new Arrow(startItem, endItem);
            arrow->setColor(myLineColor);
            startItem->removeArrows();
            startItem->addArrow(arrow);
            endItem->addArrow(arrow);
            addItem(arrow);
            arrow->updatePosition();
        }

        //from start to end
        if (startItems.count() > 0 && endItems.count() > 0 &&
            startItems.first()->type() == BlockOut::Type &&
            endItems.first()->type() == BlockIn::Type &&
            startItems.first() != endItems.first()) {
            BlockIn * startItem = qgraphicsitem_cast<BlockIn *>(endItems.first());
            BlockOut * endItem = qgraphicsitem_cast<BlockOut *>(startItems.first());
            Arrow * arrow = new Arrow(startItem, endItem);
            arrow->setColor(myLineColor);
            startItem->removeArrows();
            startItem->addArrow(arrow);
            endItem->addArrow(arrow);
            addItem(arrow);
            arrow->updatePosition();
        }
    }

    line = 0;
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}
