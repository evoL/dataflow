#ifndef MODULESLISTMODEL_H
#define MODULESLISTMODEL_H
#include <QAbstractListModel>
#include <QList>
#include "DiagramModuleItem.h"
#include "Module.h"
class ModulesListModel : public QAbstractListModel
{
public:
    ModulesListModel();
    int rowCount(const QModelIndex & parent) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    void append(Module element);
    const Module & at(int index);
    void removeAt(int index);
private:
    QList<Module> list;
};
#endif // MODULESLISTMODEL_H
