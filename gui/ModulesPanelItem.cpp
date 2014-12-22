#include "ModulesPanelItem.h"
#include <memory>


ModulesPanelItem::ModulesPanelItem(const std::string name, const void * dataPtr, ItemType itemType, ModulesPanelItem * parent)
{
	this->name = name;
	this->itemType = itemType;
	this->dataPtr = dataPtr;
	this->parentItem = parent;
}


ModulesPanelItem::~ModulesPanelItem()
{
}

ModulesPanelItem * ModulesPanelItem::child(int row)
{
	switch (itemType)
	{
	case RootT:
	case LibraryT:
	case OperationsLabelT:
		if (row >= 0 && row < children.size())
			return children.at(row);
	}
	return NULL;
}

ModulesPanelItem * ModulesPanelItem::parent()
{
	return parentItem;
}

int ModulesPanelItem::row() const
{
	if (parentItem)
		return parentItem->children.indexOf(const_cast<ModulesPanelItem*>(this));

	return 0;
}

int ModulesPanelItem::childCount()
{
	return children.count();
}

int ModulesPanelItem::columnCount()
{
	return 1;
}

QVariant ModulesPanelItem::data()
{
	switch (itemType)
	{
	case LibraryT:
	case OperationsLabelT:
	case OperationT:
		return QString::fromStdString(name);
	}
	return QString("Brak danych");
}

ModulesPanelItem * ModulesPanelItem::addLibrary(const Library * library)
{
	ModulesPanelItem * newLibrary = new ModulesPanelItem(library->getName(), library, LibraryT, this);
	children.push_back(newLibrary);
	return newLibrary;
}

ModulesPanelItem * ModulesPanelItem::addOperationsLabel()
{
	ModulesPanelItem * newLabel = new ModulesPanelItem("Operations", NULL, OperationsLabelT, this);
	children.push_back(newLabel);
	return newLabel;
}

ModulesPanelItem * ModulesPanelItem::addOperation(const std::string operation)
{
	ModulesPanelItem * newOperation = new ModulesPanelItem(operation, NULL, OperationT, this);
	children.push_back(newOperation);
	return newOperation;
}
