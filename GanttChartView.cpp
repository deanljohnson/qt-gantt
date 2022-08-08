#include "GanttChartView.h"

#include "GanttChartGraphArea.h"
#include "GanttModel.h"
#include <QGridLayout>
#include <QHeaderView>
#include <QScrollArea>
#include <QSizePolicy>
#include <QTreeView>

struct GanttChartView::GanttChartViewImpl {
  QTreeView treeView;
  GanttChartGraphArea graph;
  QDateTime minDateTime;
  QDateTime maxDateTime;
};

GanttChartView::GanttChartView(QWidget *parent)
    : QWidget(parent), m_impl(std::make_unique<GanttChartViewImpl>()) {
  auto *layout = new QGridLayout();
  layout->addWidget(&m_impl->treeView, 0, 0);
  layout->setSpacing(1);

  auto *scrollArea = new QScrollArea(this);
  scrollArea->setWidget(&m_impl->graph);
  scrollArea->setWidgetResizable(true);
  scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  layout->addWidget(scrollArea, 0, 1);
  setLayout(layout);

  const auto headerHeight = m_impl->graph.headerHeight();
  m_impl->treeView.header()->setMinimumHeight(headerHeight);
  m_impl->treeView.header()->setMaximumHeight(headerHeight);

  m_impl->treeView.setUniformRowHeights(true);

  const auto role = m_impl->treeView.backgroundRole();
  const auto role2 = m_impl->treeView.viewport()->backgroundRole();

  m_impl->treeView.setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
  m_impl->graph.setSizePolicy(QSizePolicy::MinimumExpanding,
                              QSizePolicy::Preferred);
  scrollArea->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

  m_impl->graph.setView(&m_impl->treeView);
}

GanttChartView::~GanttChartView() = default;

QTreeView *GanttChartView::treeView() const { return &m_impl->treeView; }

GanttChartGraphArea *GanttChartView::graphArea() const {
  return &m_impl->graph;
}

QDateTime GanttChartView::minDateTime() const { return m_impl->minDateTime; }

void GanttChartView::setMinDateTime(QDateTime date) {
  if (m_impl->minDateTime == date)
    return;
  m_impl->minDateTime = date;
  m_impl->graph.setMinDateTime(date);
}

QDateTime GanttChartView::maxDateTime() const { return m_impl->maxDateTime; }

void GanttChartView::setMaxDateTime(QDateTime date) {
  if (m_impl->maxDateTime == date)
    return;
  m_impl->maxDateTime = date;
  m_impl->graph.setMaxDateTime(date);
}

void GanttChartView::setModel(QAbstractItemModel *model) {
  m_impl->treeView.setModel(model);

  const auto sizingIndex = m_impl->treeView.indexAt({0, 0});
  const auto cellRect = m_impl->treeView.visualRect(sizingIndex);
  m_impl->graph.setRowHeight(cellRect.height());
}
