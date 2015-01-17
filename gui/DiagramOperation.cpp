#include "DiagramOperation.h"

DiagramOperation::DiagramOperation(const ProjectModel * model, const std::shared_ptr<Block> blockPointer, QMenu * contextMenu, QGraphicsItem * parent)
    : model(model)
    , operationPointer(std::static_pointer_cast<Operation>(blockPointer))
{
    myContextMenu = contextMenu;

    width = 150;
    height = 100;

    blockName = new BlockDescription(QString::fromStdString(operationPointer->name), this);
    blockName->setAlignCenter(0); //to do: block types, have to do sth with argument
    setBrush(myItemColor);
    updateAppearance();

    // Get input names from the model
    const Library & library = model->getLibraries().at(operationPointer->module);
    const auto & inputMap = library.getInputs();
    const auto & outputMap = library.getOutputs();
    auto & inputs = inputMap.at(operationPointer->name);
    auto & outputs = outputMap.at(operationPointer->name);

    // IO circles
    for (int i = 0; i < inputs.size(); i++) {
        BlockIn * entry = new BlockIn(i, this);
        In.append(entry);

        entry->drawIn(QStringLiteral("%2").arg(QString::fromStdString(inputs.at(i)).section('.', 1)));
    }

    for (int i = 0; i < outputs.size(); i++) {
        BlockOut * exit = new BlockOut(i, operationPointer->outputs[i].id, this);
        Out.append(exit);

        exit->drawOut(QStringLiteral("%2").arg(QString::fromStdString(outputs.at(i)).section('.', 1)));
    }

	/*if (blockName->isLong()) {
			width = 200;
			blockName->setTextWidth(250);
		}

	height = height + (blockName->scale());*/

	height = blockName->getHeight() + (In.size() > Out.size() ? In.size() * 20 : Out.size() * 20) + 5;
	setRect(0, 0, width, height);

    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}


DiagramOperation::~DiagramOperation()
{
}

void DiagramOperation::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    // This code is super brittle, but the whole context menu handling is broken.
    // Seriously, normally you should avoid block-specific code in the MainWindow
    // object, which is already too large.
    //
    // What follows is a hack that selects the "Entry point" menu item if the block
    // really is an entry point.
    QAction *firstAction = myContextMenu->actions().first();
    firstAction->setChecked(isEntryPoint());
    firstAction->setEnabled(true);

    DiagramBlock::contextMenuEvent(event);
}

bool DiagramOperation::isEntryPoint()
{
    auto entryPoints = model->getEntryPoints();
    auto result = std::find(entryPoints.cbegin(), entryPoints.cend(), operationPointer->id);
    return result != entryPoints.cend();
}

void DiagramOperation::updateAppearance()
{
    QPen pen;
    if (isEntryPoint()) {
        pen.setWidth(2);
        pen.setStyle(Qt::DashLine);
    } else {
        pen.setWidth(1);
        pen.setStyle(Qt::SolidLine);
    }

    setPen(pen);
}
