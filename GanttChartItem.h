#pragma once

#include <QColor>
#include <QDateTime>
#include <QString>
#include <QVector>

class GanttTreeItem;

///
/// @brief An item within the Gantt chart
///
class GanttChartItem {
public:
  QDateTime start() const;
  void setStart(const QDateTime &start);

  QDateTime stop() const;
  void setStop(const QDateTime &stop);

  QString longText() const;
  void setLongText(const QString &text);

  QString shortText() const;
  void setShortText(const QString &text);

  QString decorationText() const;
  void setDecorationText(const QString &text);

  QColor fillColor() const;
  void setFillColor(const QColor &color);

  QColor backgroundColor() const;
  void setBackgroundColor(const QColor &color);

  double fillAmount() const;
  void setFillAmount(double amount);

  bool showFillAmount() const;
  void setShowFillAmount(bool show);

  GanttTreeItem *parent() const;
  void setParent(GanttTreeItem *parent);

  const QVector<GanttChartItem *> &connectedItems() const;
  void setConnectedItems(QVector<GanttChartItem *> connectedItems);

  // TODO: remove?
  // int row() const;

private:
  QDateTime m_start;
  QDateTime m_stop;
  QString m_longText;
  QString m_shortText;
  QString m_decorationText;
  QColor m_fillColor;
  QColor m_backgroundColor;
  double m_fillAmount{1.0};
  bool m_showFillAmount{true};
  GanttTreeItem *m_parent{nullptr};
  QVector<GanttChartItem *> m_connectedItems;
};
