#pragma once

#include <QAbstractItemModel>
#include <QVector>

///
/// @brief Base class for a generic Gantt Model
///
class GanttModel : public QAbstractItemModel {
public:
  enum GanttRoles {
    StartRole = Qt::UserRole + 1,
    StopRole = Qt::UserRole + 2,
    LongTextRole = Qt::UserRole + 3,
    ShortTextRole = Qt::UserRole + 4,
    DecorationTextRole = Qt::UserRole + 5,
    FillColorRole = Qt::UserRole + 6,
    // TODO: use default qt background color role
    BackgroundColorRole = Qt::UserRole + 7,
    FillAmountRole = Qt::UserRole + 8
  };

  using QAbstractItemModel::QAbstractItemModel;

  virtual QVector<QModelIndex>
  connectedIndices(const QModelIndex &index) const = 0;

  int totalRowCount(const QModelIndex &parent = QModelIndex()) const;

private:
};
