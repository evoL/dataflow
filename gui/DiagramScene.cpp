#include "DiagramScene.h"
#include "Arrow.h"
#include "modulein.h"
#include "moduleout.h"
#include <QTextCursor>
#include <QGraphicsSceneMouseEvent>
#include <QColor>
#include <QBrush>

DiagramScene::DiagramScene(ModulesListModel *modulesListModel, QListView *modulesView, QMenu *itemMenu, QObject *parent)
: QGraphicsScene(parent)
{
myItemMenu = itemMenu;
this->modulesView = modulesView;
this->modulesListModel = modulesListModel;
myMode = MoveItem;
line = 0;
myItemColor = QColor(169,191,215);
myTextColor = Qt::black;
myLineColor = Qt::black;


QLinearGradient linearGrad(QPointF(0, 0), QPointF(3000, 3000));
linearGrad.setColorAt(0, QColor(175,175,175));
linearGrad.setColorAt(1, QColor(230,230,230));

setBackgroundBrush(QBrush(linearGrad));
}
void DiagramScene::setMode(Mode mode)
{
myMode = mode;
}
void DiagramScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
if (mouseEvent->button() != Qt::LeftButton)
return;
DiagramModuleItem *item;
const Module *selectedModulePtr;
switch (myMode) {
case InsertItem:
selectedModulePtr = &modulesListModel->at( modulesView->currentIndex().row() );
item = new DiagramModuleItem(selectedModulePtr, myItemMenu);
item->setBrush(myItemColor);
addItem(item);
item->setPos(mouseEvent->scenePos());
emit itemInserted();
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
void DiagramScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
if (myMode == InsertLine && line != 0) {
QLineF newLine(line->line().p1(), mouseEvent->scenePos());
line->setLine(newLine);
} else if (myMode == MoveItem) {
QGraphicsScene::mouseMoveEvent(mouseEvent);
}
}
void DiagramScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
if (line != 0 && myMode == InsertLine) {
QList<QGraphicsItem *> startItems = items(line->line().p1());
if (startItems.count() && startItems.first() == line)
startItems.removeFirst();
QList<QGraphicsItem*> endItems = items(line->line().p2());
if (endItems.count() && endItems.first() == line)
endItems.removeFirst();
removeItem(line);
delete line;
if (startItems.count() > 0 && endItems.count() > 0 &&
 startItems.first()->type() == ModuleIn::Type &&
 endItems.first()->type() == ModuleOut::Type &&
startItems.first() != endItems.first()) {
ModuleIn *startItem = qgraphicsitem_cast<ModuleIn *>(startItems.first());
ModuleOut *endItem = qgraphicsitem_cast<ModuleOut *>(endItems.first());
Arrow *arrow = new Arrow(startItem, endItem);
arrow->setColor(myLineColor);
startItem->addArrow(arrow);
endItem->addArrow(arrow);
arrow->setZValue(-1000.0);
addItem(arrow);
arrow->updatePosition();
}
}
line = 0;
QGraphicsScene::mouseReleaseEvent(mouseEvent);
}
