#include "DiagramScene.h"
#include "Arrow.h"
#include "BlockIn.h"
#include "BlockOut.h"
#include <QTextCursor>
#include <QGraphicsSceneMouseEvent>
#include <QColor>
#include <QBrush>
#include <memory>
#include <iostream>
#include "DiagramConstructor.h"
#include "DiagramOperation.h"

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

/*DiagramBlock * DiagramScene::findBlockById(int id)
{
    for (auto & item : items()) {
        if (item->type() != DiagramBlock::Type) continue;
        
        DiagramBlock * block = qgraphicsitem_cast<DiagramBlock*>(item);
        if (block != nullptr && block->getId() == id)
            return block;
    }
    
	return NULL;
}*/

BlockIn * DiagramScene::findInput(DiagramOperation * block, int index)
{
	auto inputs = block->getBlockInputs();
	auto it = inputs->begin();
	while (it != inputs->end())
	{
		if ((*it)->getIndex() == index) return *it;
		it++;
	}
	return NULL;
}

BlockOut * DiagramScene::findOutput(DiagramBlock * block, int id)
{
	auto outputs = block->getBlockOutputs();
	auto it = outputs->begin();
	while (it != outputs->end())
	{
		if ((*it)->getId() == id) return *it;
		it++;
	}
	return NULL;
}

bool DiagramScene::paintConnection(int inputBlockId, int inputBlockInput, int outputBlockId, int outputBlockOutputId)
{
	DiagramOperation * inputBlock = NULL;
	DiagramBlock * outputBlock = NULL;
	for (int i = 0; i < items().size() && (inputBlock == nullptr || outputBlock == nullptr); i++)
	{
		if ((items()[i])->type() == DiagramConstructor::Type || (items()[i])->type() == DiagramOperation::Type)
		{
			DiagramBlock * item = static_cast<DiagramBlock*>(items()[i]);
			if (item->getId() == inputBlockId)
				inputBlock = static_cast<DiagramOperation*>(item);
			else if (item->getId() == outputBlockId)
				outputBlock = item;
		}
	}

	if (inputBlock == nullptr || outputBlock == nullptr) return false;

	// Find circles
	BlockIn * endItem = findInput(inputBlock, inputBlockInput);
	BlockOut * startItem = findOutput(outputBlock, outputBlockOutputId);

	if (endItem == nullptr || startItem == nullptr) return false;

	// Paint arrow
	Arrow * arrow = new Arrow(endItem, startItem);
	arrow->setColor(myLineColor);
	startItem->removeArrows();
	startItem->addArrow(arrow);
	endItem->addArrow(arrow);
	addItem(arrow);
	arrow->updatePosition();

	return true;
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
	std::string blockName, typeName, moduleName;

    switch (myMode) {
    case InsertItem:
		if (panelView->currentIndex().isValid())
		{
			blockName = panelView->currentIndex().data().toString().toStdString();
			typeName = panelView->currentIndex().parent().data().toString().toStdString();
			moduleName = panelView->currentIndex().parent().parent().data().toString().toStdString();
			Position pos = Position{ mouseEvent->scenePos().x() - 20, mouseEvent->scenePos().y() - 20 };

			if (typeName == "Constructors")
			{
				int blockId = manipulator->addConstructor(moduleName, blockName, pos);
				item = new DiagramConstructor(projectModel->getBlocks().at(blockId), myItemMenu);
				//addItem(static_cast<DiagramConstructor*>(item));
			}
			if (typeName == "Operations")
			{
				int blockId = manipulator->addOperation(moduleName, blockName, pos);
				auto temp = projectModel->getBlocks().at(blockId);
				item = new DiagramOperation(projectModel->getBlocks().at(blockId), myItemMenu);
				//addItem(static_cast<DiagramOperation*>(item));
			}			

			addItem(item);

			item->setPos(mouseEvent->scenePos() - QPointF(20, 20));
			if (item->pos().x() < 0) item->setX(0);
			if (item->pos().y() < 0) item->setY(0);

			emit itemInserted();
		}
        break;

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
