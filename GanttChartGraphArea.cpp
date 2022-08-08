#include "GanttChartGraphArea.h"

#include "GanttChartDelegate.h"
#include "GanttModel.h"
#include <QFontMetrics>
#include <QPaintEvent>
#include <QPainter>
#include <QQueue>
#include <QTextOption>
#include <QTreeView>
#include <algorithm>

struct GanttChartGraphArea::GanttChartGraphAreaImpl {
  static constexpr int HeaderLineWidth = 1;
  static constexpr int ItemVerticalMargin = 2;

  QTreeView *view{nullptr};
  GanttModel *model{nullptr};
  GanttChartDelegate *delegate{nullptr};
  QDateTime minDateTime;
  QDateTime maxDateTime;
  int rowHeight;
  GanttUnit topUnit;
  GanttUnit bottomUnit;
};

GanttChartGraphArea::GanttChartGraphArea(QWidget *parent)
    : QFrame(parent), m_impl(std::make_unique<GanttChartGraphAreaImpl>()) {
  setBackgroundRole(QPalette::ColorRole::Base);
  setAutoFillBackground(true);

  m_impl->rowHeight = 10;
  m_impl->minDateTime = QDate::currentDate().startOfDay();
  m_impl->maxDateTime = QDate::currentDate().addDays(10).startOfDay();

  m_impl->topUnit = GanttUnit::Week;
  m_impl->bottomUnit = GanttUnit::Day;

  m_impl->delegate = new GanttChartDelegate(this);
}

GanttChartGraphArea::~GanttChartGraphArea() = default;

void GanttChartGraphArea::setMinDateTime(QDateTime date) {
  if (m_impl->minDateTime == date)
    return;
  m_impl->minDateTime = date;
  updateGeometry();
}

QDateTime GanttChartGraphArea::maxDateTime() const {
  return m_impl->maxDateTime;
}

void GanttChartGraphArea::setMaxDateTime(QDateTime date) {
  if (m_impl->maxDateTime == date)
    return;
  m_impl->maxDateTime = date;
  updateGeometry();
}

GanttUnit GanttChartGraphArea::topUnit() const { return m_impl->topUnit; }

void GanttChartGraphArea::setTopUnit(GanttUnit unit) {
  m_impl->topUnit = unit;
  updateGeometry();
  update();
}

GanttUnit GanttChartGraphArea::bottomUnit() const { return m_impl->bottomUnit; }

void GanttChartGraphArea::setBottomUnit(GanttUnit unit) {
  m_impl->bottomUnit = unit;
  updateGeometry();
  update();
}

void GanttChartGraphArea::setView(QTreeView *view) {
  m_impl->view = view;
  update();

  connect(view, &QTreeView::expanded, this, QOverload<>::of(&QWidget::update));
  connect(view, &QTreeView::collapsed, this, QOverload<>::of(&QWidget::update));
}

void GanttChartGraphArea::setModel(GanttModel *model) {
  m_impl->model = model;
  updateGeometry();
}

void GanttChartGraphArea::setDelegate(GanttChartDelegate *delegate) {
  m_impl->delegate = delegate;
  updateGeometry();
  update();
}

QSize GanttChartGraphArea::minimumSizeHint() const {
  const auto minCells = 5;
  const auto w = minCells * cellWidth();

  int minRowCount;
  if (m_impl->model == nullptr)
    minRowCount = 1;
  else
    minRowCount = std::min(5, m_impl->model->totalRowCount());

  const auto h = headerHeight() + (m_impl->rowHeight * minRowCount);
  return {w, h};
}

QSize GanttChartGraphArea::sizeHint() const {
  const auto cells = std::max(
      unitsTo(m_impl->minDateTime, m_impl->maxDateTime, m_impl->bottomUnit), 5);
  const auto w = cells * cellWidth();

  // Update size hint when row count changes
  int rowCount;
  if (m_impl->model == nullptr)
    rowCount = 5;
  else
    rowCount = m_impl->model->totalRowCount();

  const auto h = headerHeight() + (m_impl->rowHeight * rowCount);
  return {w, h};
}

int GanttChartGraphArea::headerHeight() const {
  QFontMetrics metrics(font());
  return (metrics.height() * 2) + (metrics.leading() * 4) +
         (m_impl->HeaderLineWidth * 2);
}

void GanttChartGraphArea::setRowHeight(int height) {
  m_impl->rowHeight = height;
}

void GanttChartGraphArea::paintEvent(QPaintEvent *event) {
  QFrame::paintEvent(event);

  QPainter painter(this);

  if (m_impl->minDateTime >= m_impl->maxDateTime)
    return;

  const auto &bounds = geometry();
  const auto totalHeaderHeight = headerHeight();
  const auto topHeaderHeight = totalHeaderHeight / 2;
  const auto bottomHeaderHeight = totalHeaderHeight / 2;
  const auto cellHeaderWidth = cellWidth();

  // Draw top headers
  const auto lowerTop = roundDown(m_impl->minDateTime, m_impl->topUnit);
  const auto topHeaderCols =
      1 + unitsTo(m_impl->minDateTime, m_impl->maxDateTime, m_impl->topUnit);
  for (int t = 0; t < topHeaderCols; t++) {
    const auto headerStartTime =
        std::max(addUnit(lowerTop, m_impl->topUnit, t), m_impl->minDateTime);
    const auto headerStopTime = std::min(
        addUnit(lowerTop, m_impl->topUnit, t + 1), m_impl->maxDateTime);

    const auto smallCellsToHeaderStart =
        unitsTo(m_impl->minDateTime, headerStartTime, m_impl->bottomUnit);
    const auto smallCellsToHeaderStop =
        unitsTo(m_impl->minDateTime, headerStopTime, m_impl->bottomUnit);
    const auto headerSmallCellWidth =
        smallCellsToHeaderStop - smallCellsToHeaderStart;

    const auto headerStartX = smallCellsToHeaderStart * cellHeaderWidth;
    const auto headerWidth = headerSmallCellWidth * cellHeaderWidth;

    const auto topCellRect =
        QRect(headerStartX, 0, headerWidth, topHeaderHeight);
    m_impl->delegate->drawTopHeader(&painter, topCellRect, m_impl->topUnit,
                                    m_impl->bottomUnit, headerStartTime);
  }

  // Line between top and bottom header
  painter.drawLine(0, topHeaderHeight, bounds.width(), topHeaderHeight);

  const auto rows = m_impl->model->totalRowCount();
  const auto cols =
      unitsTo(m_impl->minDateTime, m_impl->maxDateTime, m_impl->bottomUnit);

  // Draw bottom headers
  for (int d = 0; d < cols; d++) {
    const auto curDate = addUnit(m_impl->minDateTime, m_impl->bottomUnit, d);
    const auto cellRect = QRect(d * cellHeaderWidth, topHeaderHeight,
                                cellHeaderWidth, bottomHeaderHeight);

    m_impl->delegate->drawCellHeader(&painter, cellRect, m_impl->bottomUnit,
                                     m_impl->topUnit, curDate);
  }

  // Line below header
  painter.drawLine(0, totalHeaderHeight, bounds.width(), totalHeaderHeight);

  // Draw the grid
  const auto gridArea = QRect(0, totalHeaderHeight, bounds.width(),
                              bounds.height() - totalHeaderHeight);
  m_impl->delegate->drawGrid(&painter, gridArea, cellHeaderWidth,
                             m_impl->rowHeight, m_impl->bottomUnit,
                             m_impl->topUnit, m_impl->minDateTime, rows, cols);

  // Create a queue of the items in the order to be processed
  // (depth first through the tree)
  QQueue<QModelIndex> itemsToProcess;
  std::function<void(const QModelIndex &)> enqueueItem;
  enqueueItem = [&](const QModelIndex &index) {
    itemsToProcess << index;
    for (int cr = 0; cr < index.model()->rowCount(index); cr++) {
      const auto child = index.model()->index(cr, 0, index);
      if (!child.isValid())
        continue;
      enqueueItem(child);
    }
  };
  for (int r = 0; r < m_impl->model->rowCount(); r++) {
    enqueueItem(m_impl->model->index(r, 0));
  }

  // Maps the internal pointer of a model index to the item rect drawn for that
  // item. Will be used for drawing dependencies.
  QHash<QModelIndex, QRect> itemRects;
  QHash<QModelIndex, QVector<QModelIndex>> connectedItems;

  // Draw the item bars/text
  int overallRow = -1;
  while (!itemsToProcess.empty()) {
    const auto curItem = itemsToProcess.dequeue();
    if (m_impl->view != nullptr &&
        !m_impl->view->isExpanded(curItem.parent()) &&
        curItem.parent().isValid()) {
      continue;
    }

    overallRow++;

    const auto startDate =
        curItem.data(GanttModel::StartRole).value<QDateTime>();
    const auto stopDate = curItem.data(GanttModel::StopRole).value<QDateTime>();

    const auto startOffset =
        unitsTo(m_impl->minDateTime, startDate, m_impl->bottomUnit);
    const auto stopOffset =
        unitsTo(m_impl->minDateTime, stopDate, m_impl->bottomUnit);
    const auto startX = startOffset * cellHeaderWidth;
    const auto stopX = stopOffset * cellHeaderWidth;

    const auto itemRect = QRect(
        startX,
        totalHeaderHeight + (overallRow * m_impl->rowHeight) +
            m_impl->ItemVerticalMargin,
        stopX - startX, m_impl->rowHeight - (m_impl->ItemVerticalMargin * 2));

    // Store the item rect for future use
    itemRects[curItem] = itemRect;

    // Store the items connected to this one for future use
    connectedItems[curItem] = m_impl->model->connectedIndices(curItem);

    m_impl->delegate->drawBar(&painter, itemRect, curItem);
  }

  // Draw connections between items
  for (const auto &item : connectedItems.keys()) {
    const auto &connections = connectedItems[item];
    if (connections.empty())
      continue;

    const auto &thisRect = itemRects[item];
    for (const auto &connection : connections) {
      // We may not have an item rect for the connection if the connection was
      // drawn (ie. it was collapsed).
      // TODO: in this situation, can we draw a connection to the collapsed
      // items parent, if it has one?
      if (!itemRects.contains(connection))
        continue;

      const auto &otherRect = itemRects[connection];

      m_impl->delegate->drawConnection(&painter, thisRect, otherRect, item,
                                       connection);
    }
  }
}

int GanttChartGraphArea::cellWidth() const {
  return m_impl->delegate->cellWidth(m_impl->bottomUnit, this);
}
