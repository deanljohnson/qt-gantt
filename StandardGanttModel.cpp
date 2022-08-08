#include "StandardGanttModel.h"

#include "GanttChartItem.h"

#include <list>

struct StandardGanttModel::StandardGanttModelImpl {
  GanttChartItem rootItem;
  QString longTextHeader;
};

StandardGanttModel::StandardGanttModel(QObject *parent)
    : GanttModel(parent), m_impl(std::make_unique<StandardGanttModelImpl>()) {
  m_impl->longTextHeader = tr("Description");
}

StandardGanttModel::~StandardGanttModel() = default;

QVariant StandardGanttModel::headerData(int section,
                                        Qt::Orientation orientation,
                                        int role) const {
  if (orientation == Qt::Horizontal) {
    if (section < 0)
      return {};
    if (section >= 1)
      return {};
    if (role == Qt::DisplayRole)
      return m_impl->longTextHeader;
  }

  return {};
};

bool StandardGanttModel::setHeaderData(int section, Qt::Orientation orientation,
                                       const QVariant &value, int role) {
  if (orientation == Qt::Horizontal) {
    if (section < 0)
      return false;
    if (section >= 1)
      return false;

    m_impl->longTextHeader = value.toString();
    return true;
  }

  return false;
}

QVariant StandardGanttModel::data(const QModelIndex &index, int role) const {
  if (!checkIndex(index))
    return {};

  const auto *item =
      static_cast<const GanttChartItem *>(index.internalPointer());

  switch (role) {
  case GanttRoles::StartRole:
    return item->start();
  case GanttRoles::StopRole:
    return item->stop();
  case Qt::DisplayRole:
  case GanttRoles::LongTextRole:
    return item->longText();
  case GanttRoles::ShortTextRole:
    return item->shortText();
  case GanttRoles::DecorationTextRole:
    return item->decorationText();
  case GanttRoles::FillColorRole:
    return item->fillColor();
  case GanttRoles::BackgroundColorRole:
    return item->backgroundColor();
  case GanttRoles::FillAmountRole:
    return item->fillAmount();
  }

  return {};
}

bool StandardGanttModel::setData(const QModelIndex &index,
                                 const QVariant &value, int role) {
  if (!checkIndex(index))
    return false;

  auto *item = static_cast<GanttChartItem *>(index.internalPointer());
  switch (role) {
  case GanttRoles::StartRole:
    item->setStart(value.value<QDateTime>());
    return true;
  case GanttRoles::StopRole:
    item->setStop(value.value<QDateTime>());
    return true;
  case GanttRoles::LongTextRole:
    item->setLongText(value.value<QString>());
    return true;
  case GanttRoles::ShortTextRole:
    item->setShortText(value.value<QString>());
    return true;
  case GanttRoles::DecorationTextRole:
    item->setDecorationText(value.value<QString>());
    return true;
  case GanttRoles::FillColorRole:
    item->setFillColor(value.value<QColor>());
    return true;
  case GanttRoles::BackgroundColorRole:
    item->setBackgroundColor(value.value<QColor>());
    return true;
  case GanttRoles::FillAmountRole:
    item->setFillAmount(value.value<double>());
    return true;
  }

  return false;
}

int StandardGanttModel::rowCount(const QModelIndex &parent) const {
  if (parent.column() > columnCount() - 1)
    return 0;

  GanttChartItem *parentItem;
  if (!parent.isValid())
    parentItem = &m_impl->rootItem;
  else
    parentItem = static_cast<GanttChartItem *>(parent.internalPointer());

  return parentItem->children().count();
}

int StandardGanttModel::columnCount(const QModelIndex &parent) const {
  return 1;
}

QModelIndex StandardGanttModel::index(int row, int column,
                                      const QModelIndex &parent) const {
  if (!hasIndex(row, column, parent))
    return {};

  GanttChartItem *parentItem;
  if (!parent.isValid())
    parentItem = &m_impl->rootItem;
  else
    parentItem = static_cast<GanttChartItem *>(parent.internalPointer());

  if (parentItem->children().size() > row)
    return createIndex(row, column, parentItem->children()[row]);
  return {};
}

QModelIndex StandardGanttModel::parent(const QModelIndex &index) const {
  if (!index.isValid())
    return {};

  auto *childItem = static_cast<GanttChartItem *>(index.internalPointer());
  auto *parentItem = childItem->parent();

  if (parentItem == &m_impl->rootItem)
    return {};

  return createIndex(parentItem->row(), 0, parentItem);
}

QVector<GanttChartItem *> StandardGanttModel::rootItems() {
  return m_impl->rootItem.children();
}

GanttChartItem *StandardGanttModel::addItem(GanttChartItem *parent) {
  if (parent == nullptr)
    parent = &m_impl->rootItem;

  auto children = parent->children();
  auto *item = new GanttChartItem();
  item->setParent(parent);
  children << item;
  parent->setChildren(children);
  return item;
}

QVector<QModelIndex>
StandardGanttModel::connectedIndices(const QModelIndex &index) const {
  if (!index.isValid())
    return {};

  const auto *item = static_cast<GanttChartItem *>(index.internalPointer());
  const auto &connectedItems = item->connectedItems();

  QVector<QModelIndex> results;
  for (const auto &connectedItem : connectedItems) {
    results << createIndex(connectedItem->row(), 0, connectedItem);
  }
  return results;
}
