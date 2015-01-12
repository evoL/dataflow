#include "DiagramOperation.h"

DiagramOperation::DiagramOperation(const ProjectModel * model, const std::shared_ptr<Block> blockPointer, QMenu * contextMenu, QGraphicsItem * parent)
	: blockPtr(std::static_pointer_cast<Operation>(blockPointer))
{
    myContextMenu = contextMenu;

    width = 170;
    height = 100;

    blockName = new BlockDescription(QString::fromStdString(blockPtr->name), this);
    if (blockName->isLong()) {
        width = 200;
        blockName->setTextWidth(250);
    }

    height = height + (blockName->scale());

    setRect(0, 0, width, height);
    blockName->setAlignCenter(0); //to do: block types, have to do sth with argument
	setBrush(myItemColor);
    
    // Get input names from the model
    const Library & library = model->getLibraries().at(blockPtr->module);
    const auto & inputMap = library.getInputs();
    const auto & outputMap = library.getOutputs();
    auto & inputs = inputMap.at(blockPtr->name);
    auto & outputs = outputMap.at(blockPtr->name);

    // IO circles
    for (int i = 0; i < blockPtr->inputs.size(); i++) {
        BlockIn * entry = new BlockIn(i, this);
        In.append(entry);

        entry->drawIn(QStringLiteral("%2 (%1)").arg(blockPtr->inputs[i].outputId).arg(QString::fromStdString(inputs.at(i)).section('.', 1)));
    }

    for (int i = 0; i < blockPtr->outputs.size(); i++) {
        BlockOut * exit = new BlockOut(i, blockPtr->outputs[i].id, this);
        Out.append(exit);

        exit->drawOut(QStringLiteral("%2 (%1)").arg(blockPtr->outputs[i].id).arg(QString::fromStdString(outputs.at(i)).section('.', 1)));
    }

    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}


DiagramOperation::~DiagramOperation()
{
}
