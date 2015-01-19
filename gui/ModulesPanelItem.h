#ifndef _MODULESPANELITEM_H
#define _MODULESPANELITEM_H

#include "DataflowModel.h"

#include <QList>
#include <QVariant>

#include <vector>
#include <string>
#include <memory>

class ModulesPanelItem
{
public:
    enum ItemType {
        RootT,
        LibraryT,
        OperationsLabelT,
        ConstructorsLabelT,
        OperationT,
        ConstructorT,
    };

    ModulesPanelItem(const std::string name, const void * dataPtr, ItemType type, ModulesPanelItem * parent = 0);
    ~ModulesPanelItem();

    ModulesPanelItem * child(int row);
    ModulesPanelItem * parent();
    int row() const;
    int childCount();
    int columnCount();
    QVariant data();
    const void * getDataPtr() { return dataPtr; }
    ItemType getItemType() { return itemType; }
    void removeAt(int row);

    ModulesPanelItem * addLibrary(const Library * library);
    ModulesPanelItem * addOperationsLabel();
    ModulesPanelItem * addConstructorsLabel();
    ModulesPanelItem * addOperation(const std::string operation);
    ModulesPanelItem * addConstructor(const std::string constructor);

private:
    ModulesPanelItem * parentItem;
    ItemType itemType;
    std::string name;
    const void * dataPtr;
    QList <ModulesPanelItem *> children;
};

#endif
