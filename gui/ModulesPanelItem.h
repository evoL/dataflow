#ifndef _MODULESPANELITEM_H
#define _MODULESPANELITEM_H

#include <DataflowModel.h>
#include <QList>
#include <QVariant>

#include <vector>
#include <string>

class ModulesPanelItem
{
public:
	enum ItemType
	{
		RootT,
		LibraryT,
		OperationsLabelT,
		OperationT,
		ConstructorT,
	};

	ModulesPanelItem(const std::string name, const void * data, ItemType type, ModulesPanelItem * parent = 0);
	~ModulesPanelItem();

	ModulesPanelItem * child(int row);
	ModulesPanelItem * parent();
	int row() const;
	int childCount();
	int columnCount();
	QVariant data();

	ModulesPanelItem * addLibrary(const Library * library);
	ModulesPanelItem * addOperationsLabel();
	ModulesPanelItem * addOperation(const std::string operation);

private:
	ModulesPanelItem * parentItem;
	ItemType itemType;
	std::string name;
	const void * dataPtr;
	QList <ModulesPanelItem*> children;
	
};

#endif