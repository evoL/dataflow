#ifndef MODULESPANELMODEL_H
#define MODULESPANELMODEL_H

#include <QAbstractItemModel>

#include <DataflowModel.h>
#include <ModulesPanelItem.h>

class ModulesPanelModel : public QAbstractItemModel
{
	Q_OBJECT

public:
	ModulesPanelModel(ProjectModel *projectModel, QObject * parent = 0);
	~ModulesPanelModel();

	void loadFromProjectModel(ProjectModel * projectModel);

    QModelIndex index(int row, int column, const QModelIndex & parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex & index) const;
    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    int columnCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
	Qt::ItemFlags flags(const QModelIndex &index) const;
    std::shared_ptr<const Block> getBlockPtr(const QModelIndex &index) const;

private:
    ModulesPanelItem * rootItem;

};

#endif // MODULESPANELMODEL_H
