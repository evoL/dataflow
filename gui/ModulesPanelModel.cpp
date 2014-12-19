#include "ModulesPanelModel.h"

#include <iostream>

const Library * att(const LibraryMap *map, unsigned int index)
{
	if (index >= map->size()) 
		throw std::out_of_range("The given index (" + std::to_string(index) + 
								") is out of range. Map size = " + std::to_string(map->size()));

	LibraryMap::const_iterator it = map->begin();
	while (it != map->end() && index > 0)
	{
		index--;
		it++;
	}
	return &it->second;
}

ModulesPanelModel::ModulesPanelModel(ProjectModel * projectModel, QObject * parent) : QAbstractItemModel(parent)
{
	this->rootItem = new ModulesPanelItem("root", projectModel, ModulesPanelItem::RootT);
	loadFromProjectModel(projectModel);
}

ModulesPanelModel::~ModulesPanelModel()
{
	delete rootItem;
}

void ModulesPanelModel::loadFromProjectModel(ProjectModel * projectModel)
{
	// Libraries
	const LibraryMap * libraries = &(projectModel->getLibraries());
	LibraryMap::const_iterator it = libraries->begin();
	while (it != libraries->end())
	{
		auto * libraryItem = rootItem->addLibrary(&it->second);

		// Block types
		const std::vector<std::string> operations = (it->second.getOperations());
		if (operations.size() > 0)
		{
			auto * operationsLabel = libraryItem->addOperationsLabel();
			// Operations
			for (int i = 0; i < operations.size(); i++)
			{
				operationsLabel->addOperation(operations[i]);
			}
		}
		it++;
	}

}

QModelIndex ModulesPanelModel::index(int row, int column, const QModelIndex & parent) const
{
	ModulesPanelItem *parentItem;
	if (!parent.isValid())
		parentItem = rootItem;
	else
		parentItem = static_cast<ModulesPanelItem*>(parent.internalPointer());

	ModulesPanelItem *childItem = parentItem->child(row);
	if (childItem)
		return createIndex(row, column, childItem);
	else
		return QModelIndex();
}

QModelIndex ModulesPanelModel::parent(const QModelIndex & index) const
{
	if (!index.isValid())
		return QModelIndex();

	auto * childItem = static_cast<ModulesPanelItem*>(index.internalPointer());
	auto * parentItem = childItem->parent();

	if (parentItem == rootItem)
		return QModelIndex();

	return createIndex(parentItem->row(), 0, parentItem);
}

int ModulesPanelModel::rowCount(const QModelIndex & parent) const
{
	ModulesPanelItem *parentItem;
	if (parent.column() > 0)
		return 0;

	if (!parent.isValid())
		parentItem = rootItem;
	else
		parentItem = static_cast<ModulesPanelItem*>(parent.internalPointer());

	return parentItem->childCount();
}

int ModulesPanelModel::columnCount(const QModelIndex & parent) const
{
	if (parent.isValid())
		return static_cast<ModulesPanelItem*>(parent.internalPointer())->columnCount();
	else
		return rootItem->columnCount();
}

QVariant ModulesPanelModel::data(const QModelIndex & index, int role) const
{
	if (!index.isValid())
		return QVariant();

	if (role != Qt::DisplayRole)
		return QVariant();

	ModulesPanelItem *item = static_cast<ModulesPanelItem*>(index.internalPointer());

	return item->data();
}

Qt::ItemFlags ModulesPanelModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return 0;

	return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

