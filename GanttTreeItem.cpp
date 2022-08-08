#include "GanttTreeItem.h"

GanttTreeItem::GanttTreeItem(QString displayText) {
  setDisplayText(displayText);
}

QString GanttTreeItem::displayText() const { return m_displayText; }

void GanttTreeItem::setDisplayText(const QString &text) {
  m_displayText = text;
}

QVector<GanttChartItem *> GanttTreeItem::chartItems() const { return m_items; }

void GanttTreeItem::setChartItems(QVector<GanttChartItem *> items) {
  m_items = items;
}
