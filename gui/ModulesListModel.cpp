#include "ModulesListModel.h"
#include <iostream>

ModulesListModel::ModulesListModel()
{
}

int ModulesListModel::rowCount(const QModelIndex & parent) const
{
    // Yes, we want it to be invalid (QModelIndex() creates empty model index)
    // In simple list we don't care about parent
    if (!parent.isValid())
        return list.size();
    else
        return 0;
}

QVariant ModulesListModel::data(const QModelIndex & index, int role) const
{
    if (index.isValid() && index.row() < list.size() && role == Qt::DisplayRole)
    {
        return QVariant(list.at(index.row()).toString());
    }
    else
        return QVariant();
}

void ModulesListModel::append(Module element)
{
    list.append(element);
}

const Module& ModulesListModel::at(int index)
{
    return list.at(index);
}

void ModulesListModel::removeAt(int index)
{
    list.removeAt(index);
}
