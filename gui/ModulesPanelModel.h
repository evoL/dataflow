#ifndef MODULESPANELMODEL_H
#define MODULESPANELMODEL_H

#include "DataflowModel.h"
#include "ModulesPanelItem.h"

#include <QAbstractItemModel>

#include <memory>

class ModulesPanelModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    ModulesPanelModel(ProjectModel * projectModel, QObject * parent = 0);
    ~ModulesPanelModel();

    void loadFromProjectModel(ProjectModel * projectModel);
    void addLibrary(const Library & library);
    void removeLibraryAtRow(int row);

    QModelIndex index(int row, int column, const QModelIndex & parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex & index) const;
    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    int columnCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    Qt::ItemFlags flags(const QModelIndex & index) const;
    std::shared_ptr<const Block> getBlockPtr(const QModelIndex & index) const;
    const Library * getLibraryPtr(const QModelIndex & index) const;

private:
    ModulesPanelItem * rootItem;

    void appendLibrary(const Library & library);
};

#endif // MODULESPANELMODEL_H
