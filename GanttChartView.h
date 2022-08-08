#pragma once

#include <QDateTime>
#include <QWidget>
#include <memory>

class GanttChartGraphArea;
class QAbstractItemModel;
class QTreeView;

///
/// @brief The main widget defining the Gantt Chart.
///
/// A GanttChartView is made up of a regular QTreeView widget that shows a
/// collapsible hierarchy of items from a GanttModel and a GanttChartGraphAreaa
/// which shows the Gantt Chart itself.
class GanttChartView : public QWidget {
public:
  GanttChartView(QWidget *parent = nullptr);
  ~GanttChartView() override;

  QTreeView *treeView() const;
  GanttChartGraphArea *graphArea() const;

  QDateTime minDateTime() const;
  void setMinDateTime(QDateTime date);

  QDateTime maxDateTime() const;
  void setMaxDateTime(QDateTime date);

  void setModel(QAbstractItemModel *model);

private:
  struct GanttChartViewImpl;
  std::unique_ptr<GanttChartViewImpl> m_impl;
};