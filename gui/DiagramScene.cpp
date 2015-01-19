#include "DiagramScene.h"
#include "Arrow.h"

DiagramScene::DiagramScene(ModulesPanelModel * panelModel, QTreeView * panelView, QMenu * itemMenu, QObject * parent)
    : QGraphicsScene(parent)
{
    mainWindow = static_cast<MainWindow*>(parent);
    myItemMenu = itemMenu;
    this->panelView = panelView;
    this->panelModel = panelModel;
    myMode = MoveItem;
    line = 0;

    myTextColor = Qt::black;
    myLineColor = Qt::black;

    /*minWidth = 50;
    minHeight = 50;
    width = minWidth;
    height = minHeight;*/
    setSceneSizeAndGradient(QSize(50,50));

    shiftIsDown = false;
}

void DiagramScene::setSceneSizeAndGradient(QSize size)
{
    setSceneRect(QRectF(0, 0, size.width(), size.height()));

    QLinearGradient linearGrad(QPointF(0, 0), QPointF(size.width(), size.height()));
    linearGrad.setColorAt(0, QColor(175, 175, 175));
    linearGrad.setColorAt(1, QColor(230, 230, 230));
    setBackgroundBrush(QBrush(linearGrad));
}

QSize DiagramScene::getSizeHint()
{
    qreal maxWidth = 0, maxHeight = 0;
    for (QGraphicsItem * item : items()) {
        if (item->type() == DiagramConstructor::Type || item->type() == DiagramOperation::Type) {
            DiagramBlock * block = static_cast<DiagramBlock*>(item);
            if (block->x() + block->getWidth() > maxWidth)
                maxWidth = block->x() + block->getWidth();
            if (block->y() + block->getHeight() > maxHeight)
                maxHeight = block->y() + block->getHeight();
        }
    }

    maxWidth += 20;
    maxHeight += 20;

    QSize widgetSize = mainWindow->getSceneViewSize();
    if (maxWidth < widgetSize.width()) maxWidth = widgetSize.width();
    if (maxHeight < widgetSize.height()) maxHeight = widgetSize.height();

    return QSize(maxWidth, maxHeight);
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

BlockIn * DiagramScene::findInput(DiagramOperation * block, int index)
{
    auto inputs = block->getBlockInputs();
    for (auto it = inputs->begin(); it != inputs->end(); it++) {
        if ((*it)->getIndex() == index) return *it;
    }
    return NULL;
}

BlockOut * DiagramScene::findOutput(DiagramBlock * block, int id)
{
    auto outputs = block->getBlockOutputs();
    for (auto it = outputs->begin(); it != outputs->end(); it++) {
        if ((*it)->getId() == id) return *it;
    }
    return NULL;
}

bool DiagramScene::paintConnectionWhenLoadingProject(int inputBlockId, int inputBlockInput, int outputBlockId, int outputBlockOutputId)
{
    DiagramOperation * inputBlock = NULL;
    DiagramBlock * outputBlock = NULL;
    for (int i = 0; i < items().size() && (inputBlock == nullptr || outputBlock == nullptr); i++) {
        if ((items()[i])->type() == DiagramConstructor::Type || (items()[i])->type() == DiagramOperation::Type) {
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

void DiagramScene::keyPressEvent(QKeyEvent * keyEvent)
{
    if (keyEvent->key() == Qt::Key_Shift)
    {
        mainWindow->clickLinePointerButton();
        shiftIsDown = true;
    }
    QGraphicsScene::keyPressEvent(keyEvent);
}

void DiagramScene::keyReleaseEvent(QKeyEvent * keyEvent)
{
    if (keyEvent->key() == Qt::Key_Shift)
    {
        mainWindow->clickPointerButton();
        if (line) removeItem(line);
        line = NULL;
        shiftIsDown = false;
    }
    QGraphicsScene::keyReleaseEvent(keyEvent);
}

void DiagramScene::mousePressEvent(QGraphicsSceneMouseEvent * mouseEvent)
{
    if (mouseEvent->button() != Qt::LeftButton)
        return;

    DiagramBlock * item;
    std::string blockName, typeName, moduleName;

    switch (myMode) {
    case InsertItem:
        if (panelView->currentIndex().isValid()) {
            // Only allow deeper levels
            if (!panelView->currentIndex().parent().isValid()) break;

            blockName = panelView->currentIndex().data().toString().toStdString();
            typeName = panelView->currentIndex().parent().data().toString().toStdString();
            moduleName = panelModel->getLibraryPtr(panelView->currentIndex().parent().parent())->getName();
            Position pos = Position{ static_cast<float>(mouseEvent->scenePos().x() - 20), static_cast<float>(mouseEvent->scenePos().y() - 20) };

            try
            {
                if (typeName == "Constructors")
                {
                    int blockId = manipulator->addConstructor(moduleName, blockName, pos);
                    item = new DiagramConstructor(projectModel->getBlocks().at(blockId), projectModel->getLibraries(), myItemMenu);
                    connect(static_cast<DiagramConstructor *>(item), &DiagramConstructor::valueChanged, static_cast<MainWindow *>(parent()), &MainWindow::updateConstructorValue);
                }
                if (typeName == "Operations")
                {
                    int blockId = manipulator->addOperation(moduleName, blockName, pos);
                    item = new DiagramOperation(projectModel, projectModel->getBlocks().at(blockId), myItemMenu);
                }

                addItem(item);

                item->setPos(mouseEvent->scenePos() - QPointF(20, 20));
                if (item->pos().x() < 0) item->setX(0);
                if (item->pos().y() < 0) item->setY(0);

                emit itemInserted();
            }
            catch (ModelManipulatorError e) {
                QMessageBox::critical(mainWindow, tr("Dataflow Creator"), e.what());
            }
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
        line = NULL;

        Arrow * arrow = nullptr;
        BlockIn * startItem = nullptr;
        BlockOut * endItem = nullptr;

        try {
            //from end to start
            if (startItems.count() > 0 && endItems.count() > 0 &&
                startItems.first()->type() == BlockIn::Type &&
                endItems.first()->type() == BlockOut::Type &&
                startItems.first() != endItems.first()) {
                startItem = qgraphicsitem_cast<BlockIn *>(startItems.first());
                endItem = qgraphicsitem_cast<BlockOut *>(endItems.first());

                if (startItem->parentItem() == endItem->parentItem())
                    throw ModelManipulatorError("Can't connect input with output of the same block");

                arrow = new Arrow(startItem, endItem);
                arrow->setColor(myLineColor);
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
                startItem = qgraphicsitem_cast<BlockIn *>(endItems.first());
                endItem = qgraphicsitem_cast<BlockOut *>(startItems.first());

                if (startItem->parentItem() == endItem->parentItem())
                    throw ModelManipulatorError("Can't connect input with output of the same block");

                arrow = new Arrow(startItem, endItem);
                arrow->setColor(myLineColor);
                startItem->addArrow(arrow);
                endItem->addArrow(arrow);
                addItem(arrow);
                arrow->updatePosition();

            }

            // Update model
            if (startItem != nullptr && endItem != nullptr) {
                DiagramBlock * inputBlock = static_cast<DiagramBlock*>(startItem->parentItem());
                DiagramBlock * outputBlock = static_cast<DiagramBlock*>(endItem->parentItem());

                manipulator->addConnection(outputBlock->getId(), endItem->getIndex(), inputBlock->getId(), startItem->getIndex());
            }
        }
        catch (ModelManipulatorError error) {
            std::cerr << error.what() << std::endl;
            if (shiftIsDown) {
                mainWindow->clickPointerButton();
                shiftIsDown = false;
            }
            QMessageBox::warning(mainWindow, "Connecting error", error.what());
            if (arrow) removeItem(arrow);

        }
    }
    else if (myMode == MoveItem) {
        for (QGraphicsItem * item : selectedItems()){
            if (item->type() == DiagramConstructor::Type || item->type() == DiagramOperation::Type) {
                DiagramBlock * blockItem = static_cast<DiagramBlock*>(item);
                Position newPosition = Position{ static_cast<float>(blockItem->pos().x()), static_cast<float>(blockItem->pos().y()) };

                if (newPosition.x < 0) newPosition.x = 0;
                if (newPosition.y < 0) newPosition.y = 0;
                blockItem->setPos(newPosition.x, newPosition.y);
                setSceneSizeAndGradient(getSizeHint());

                manipulator->setBlockPosition(blockItem->getId(), newPosition);
            }
        }
    }

    if (line) removeItem(line);
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}
