#include "GanttChartDelegate.h"

#include "GanttModel.h"
#include <QPainter>
#include <QTextOption>
#include <QWidget>

int GanttChartDelegate::cellWidth(GanttUnit unit, const QWidget *widget) {
  static constexpr int HeaderHorizontalPadding = 6;
  QFontMetrics metrics(widget->font());

  if (unit == GanttUnit::Hour) {
    const auto rect = metrics.boundingRect(QStringLiteral("24"));
    return rect.width() + (HeaderHorizontalPadding * 2);
  } else if (unit == GanttUnit::Day) {
    const auto monRect = metrics.boundingRect('M');
    const auto tueRect = metrics.boundingRect('T');
    const auto wedRect = metrics.boundingRect('W');
    const auto thuRect = metrics.boundingRect('T');
    const auto friRect = metrics.boundingRect('F');
    const auto satRect = metrics.boundingRect('S');
    const auto sunRect = metrics.boundingRect('S');

    const auto charWidth = std::max(
        {monRect.width(), tueRect.width(), wedRect.width(), thuRect.width(),
         friRect.width(), satRect.width(), sunRect.width()});

    return charWidth + (HeaderHorizontalPadding * 2);
  }
}

void GanttChartDelegate::drawTopHeader(QPainter *painter, const QRect &rect,
                                       GanttUnit unit, GanttUnit otherUnit,
                                       const QDateTime &value) {
  const auto defaultPen = painter->pen();
  const auto boldPen = QPen(defaultPen.color(), 2);

  QTextOption cellTextOpt;
  cellTextOpt.setAlignment(Qt::AlignCenter);

  const auto unitVal = getUnitValue(value, unit);
  painter->drawText(rect, QString::number(unitVal), cellTextOpt);

  // Line between each cell
  painter->setPen(boldPen);
  painter->drawLine(rect.right(), rect.top() + 1, rect.right(), rect.bottom());
  painter->setPen(defaultPen);
}

void GanttChartDelegate::drawCellHeader(QPainter *painter, const QRect &rect,
                                        GanttUnit unit, GanttUnit otherUnit,
                                        const QDateTime &value) {
  const auto defaultPen = painter->pen();
  const auto boldPen = QPen(defaultPen.color(), 2);

  QTextOption cellTextOpt;
  cellTextOpt.setAlignment(Qt::AlignCenter);

  const auto unitVal = getUnitValue(value, unit);
  painter->drawText(rect, QString::number(unitVal), cellTextOpt);

  const auto bold = shouldBeBold(value, unit, otherUnit);

  if (bold)
    painter->setPen(boldPen);

  // Line between each cell
  painter->drawLine(rect.right(), rect.top() + 1, rect.right(), rect.bottom());

  if (bold)
    painter->setPen(defaultPen);
}

void GanttChartDelegate::drawGrid(QPainter *painter, const QRect &rect,
                                  int colWidth, int rowHeight, GanttUnit unit,
                                  GanttUnit otherUnit,
                                  const QDateTime &startDate, int rows,
                                  int cols) {
  const auto defaultPen = painter->pen();
  const auto lighterPen = QPen(defaultPen.color().lighter());
  const auto boldPen = QPen(defaultPen.color(), 2);

  for (int c = 0; c < cols; c++) {
    const auto date = addUnit(startDate, unit, c);

    const auto bold = shouldBeBold(date, unit, otherUnit);
    if (bold)
      painter->setPen(boldPen);
    else
      painter->setPen(lighterPen);

    const auto x = rect.x() + ((c + 1) * colWidth) - 1;
    painter->drawLine(x, rect.top(), x, rect.bottom());
  }

  painter->setPen(defaultPen);
}

void GanttChartDelegate::drawBar(QPainter *painter, const QRect &rect,
                                 const QModelIndex &index) {
  static constexpr int ItemHorizontalMargin = 6;

  const auto backgroundColor =
      index.data(GanttModel::BackgroundColorRole).value<QColor>();
  const auto fillColor = index.data(GanttModel::FillColorRole).value<QColor>();
  const auto fillAmount =
      index.data(GanttModel::FillAmountRole).value<double>();
  // TODO: show fill amount
  // TODO: decoration text
  const auto shortText = index.data(GanttModel::ShortTextRole).value<QString>();

  const auto filledRect =
      QRect(rect.x(), rect.y(), int(rect.width() * fillAmount), rect.height());

  painter->fillRect(rect, backgroundColor);
  painter->fillRect(filledRect, fillColor);

  QTextOption itemTextOpt;
  itemTextOpt.setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

  if (!shortText.isEmpty()) {
    QRect textRect = rect;
    textRect.setX(textRect.x() + ItemHorizontalMargin);
    painter->drawText(textRect, shortText, itemTextOpt);
  }
}

void GanttChartDelegate::drawConnection(QPainter *painter, const QRect &src,
                                        const QRect &dst,
                                        const QModelIndex &srcIndex,
                                        const QModelIndex &dstIndex) {
  static constexpr int ItemConnectionHorizontalOffset = 5;
  static constexpr int ItemConnectionArrowSize = 6;

  const auto sourceX = src.right();
  const auto sourceY = src.center().y();
  const auto destX = dst.x() + ItemConnectionHorizontalOffset;
  const auto destArrowBaseY = dst.top() - ItemConnectionArrowSize;
  const auto destArrowLeft = destX - (ItemConnectionArrowSize / 2);
  const auto destArrowRight = destX + (ItemConnectionArrowSize / 2);
  const auto destY = dst.top();

  // Line to the right
  painter->drawLine(sourceX, sourceY, destX, sourceY);
  // Line down
  painter->drawLine(destX, sourceY, destX, destArrowBaseY);
  // Top of arrow
  painter->drawLine(destArrowLeft, destArrowBaseY, destArrowRight,
                    destArrowBaseY);
  // Points of arrow
  painter->drawLine(destArrowLeft, destArrowBaseY, destX, destY);
  painter->drawLine(destArrowRight, destArrowBaseY, destX, destY);
}

bool GanttChartDelegate::shouldBeBold(const QDateTime &value, GanttUnit unit,
                                      GanttUnit otherUnit) const {
  if (unit == GanttUnit::Day && otherUnit == GanttUnit::Week) {
    const auto dayOfWeek = value.date().dayOfWeek();
    return dayOfWeek == 7;
  } else if (unit == GanttUnit::Hour && otherUnit == GanttUnit::Day) {
    return value.time().hour() == 23;
  }

  return false;
}
