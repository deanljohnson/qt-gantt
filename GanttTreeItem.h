#pragma once

#include <QStandardItem>
#include <QString>
#include <QVector>

class GanttChartItem;

///
/// @brief An item displayed within the Gantt chart tree view
///
class GanttTreeItem : public QStandardItem {
public:
  explicit GanttTreeItem(QString displayText = QString());

  QString displayText() const;
  void setDisplayText(const QString &text);

  QVector<GanttTreeItem *> childrenTreeItems() const;

  QVector<GanttChartItem *> chartItems() const;
  void setChartItems(QVector<GanttChartItem *> chartItems);

private:
  QString m_displayText;
  QVector<GanttChartItem *> m_items;
};
