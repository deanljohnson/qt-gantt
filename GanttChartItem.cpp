#include "GanttChartItem.h"

QDateTime GanttChartItem::start() const { return m_start; }

void GanttChartItem::setStart(const QDateTime &start) { m_start = start; }

QDateTime GanttChartItem::stop() const { return m_stop; }

void GanttChartItem::setStop(const QDateTime &stop) { m_stop = stop; }

QString GanttChartItem::longText() const { return m_longText; }

void GanttChartItem::setLongText(const QString &text) { m_longText = text; }

QString GanttChartItem::shortText() const { return m_shortText; }

void GanttChartItem::setShortText(const QString &text) { m_shortText = text; }

QString GanttChartItem::decorationText() const { return m_decorationText; }

void GanttChartItem::setDecorationText(const QString &text) {
  m_decorationText = text;
}

QColor GanttChartItem::fillColor() const { return m_fillColor; }

void GanttChartItem::setFillColor(const QColor &color) { m_fillColor = color; }

QColor GanttChartItem::backgroundColor() const { return m_backgroundColor; }

void GanttChartItem::setBackgroundColor(const QColor &color) {
  m_backgroundColor = color;
}

double GanttChartItem::fillAmount() const { return m_fillAmount; }

void GanttChartItem::setFillAmount(double amount) { m_fillAmount = amount; }

bool GanttChartItem::showFillAmount() const { return m_showFillAmount; }

void GanttChartItem::setShowFillAmount(bool show) { m_showFillAmount = show; }

GanttTreeItem *GanttChartItem::parent() const { return m_parent; }

void GanttChartItem::setParent(GanttTreeItem *parent) { m_parent = parent; }

const QVector<GanttChartItem *> &GanttChartItem::connectedItems() const {
  return m_connectedItems;
}

void GanttChartItem::setConnectedItems(
    QVector<GanttChartItem *> connectedItems) {
  m_connectedItems = connectedItems;
}

/*int GanttChartItem::row() const {
  if (m_parent == nullptr)
    return 0;
  const auto &parentChildren = m_parent->children();
  for (int i = 0; i < parentChildren.count(); i++) {
    if (parentChildren[i] == this)
      return i;
  }
  return 0;
}*/
