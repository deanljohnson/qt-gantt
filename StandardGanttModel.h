#pragma once

#include "GanttModel.h"
#include <memory>

class GanttChartItem;

class StandardGanttModel : public GanttModel {
public:
  StandardGanttModel(QObject *parent = nullptr);
  ~StandardGanttModel() override;

  QVariant headerData(int section, Qt::Orientation orientation,
                      int role = Qt::DisplayRole) const override;
  bool setHeaderData(int section, Qt::Orientation orientation,
                     const QVariant &value, int role = Qt::EditRole) override;
  QVariant data(const QModelIndex &index,
                int role = Qt::DisplayRole) const override;
  bool setData(const QModelIndex &index, const QVariant &value,
               int role = Qt::EditRole) override;
  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  int columnCount(const QModelIndex &parent = QModelIndex()) const override;

  QModelIndex index(int row, int column,
                    const QModelIndex &parent = QModelIndex()) const override;
  QModelIndex parent(const QModelIndex &index) const override;

  QVector<GanttChartItem *> rootItems();
  GanttChartItem *addItem(GanttChartItem *parent);

  QVector<QModelIndex>
  connectedIndices(const QModelIndex &index) const override;

private:
  struct StandardGanttModelImpl;
  std::unique_ptr<StandardGanttModelImpl> m_impl;
};
