#pragma once

#include "GanttUnit.h"
#include <QDateTime>
#include <QFrame>
#include <memory>

class GanttChartDelegate;
class GanttModel;
class QTreeView;

class GanttChartGraphArea : public QFrame {
public:
  GanttChartGraphArea(QWidget *parent = nullptr);
  ~GanttChartGraphArea() override;

  QDateTime minDateTime() const;
  void setMinDateTime(QDateTime date);

  QDateTime maxDateTime() const;
  void setMaxDateTime(QDateTime date);

  GanttUnit topUnit() const;
  void setTopUnit(GanttUnit unit);

  GanttUnit bottomUnit() const;
  void setBottomUnit(GanttUnit unit);

  void setView(QTreeView *view);
  void setModel(GanttModel *model);
  void setDelegate(GanttChartDelegate *delegate);

  QSize minimumSizeHint() const override;
  QSize sizeHint() const override;

  int headerHeight() const;

  void setRowHeight(int height);

protected:
  void paintEvent(QPaintEvent *event) override;

private:
  int cellWidth() const;

  struct GanttChartGraphAreaImpl;
  std::unique_ptr<GanttChartGraphAreaImpl> m_impl;
};
