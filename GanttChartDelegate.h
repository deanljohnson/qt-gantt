#pragma once

#include "GanttUnit.h"
#include <QDateTime>
#include <QModelIndex>
#include <QObject>
#include <QRect>

class QPainter;
class QWidget;

///
/// @brief Contains functions for customizing the rendering of a Gantt chart.
///
/// Derive from this class and override these function to customize how the Gantt chart is
/// displayed.
class GanttChartDelegate : public QObject {
public:
  using QObject::QObject;

  /// @brief Returns how wide a cell should be of the given unit.
  virtual int cellWidth(GanttUnit unit, const QWidget *widget);

  ///
  /// @brief Draws the top header for the given column of the chart.
  ///
  /// Note that this function is opnly responsible for the upper header of the chart.
  ///
  /// @param painter The painter object to use to do the drawing
  /// @param rect The rect containing this header
  /// @param unit The primary unit used by the gantt chart. This unit is displayed on the upper
  ///             gantt axis.
  /// @param otherUnit The other gantt chart unit. This is the unit shown in the lower gantt axis
  ///                  and the unit that dictates the placement of data in the chart.
  /// @param value The time value the cell should be drawn for.
  virtual void drawTopHeader(QPainter *painter, const QRect &rect,
                             GanttUnit unit, GanttUnit otherUnit,
                             const QDateTime &value);

  ///
  /// @brief Draws the header for the given column of the chart.
  ///
  /// Note that this function is opnly responsible for the lower header of the chart.
  ///
  /// @param painter The painter object to use to do the drawing
  /// @param rect The rect containing this header
  /// @param unit The main gantt chart unit. This is the unit shown in the lower gantt axis and the
  ///             unit that dictates the placement of data in the chart.
  /// @param otherUnit The other unit used by the gantt chart. This unit is displayed on the upper
  ///                  gantt axis.
  /// @param value The time value the cell should be drawn for.
  virtual void drawCellHeader(QPainter *painter, const QRect &rect,
                              GanttUnit unit, GanttUnit otherUnit,
                              const QDateTime &value);

  ///
  /// @brief Draws the grid between the cells of a gantt chart.
  ///
  /// @param painter The painter object to use to do the drawing
  /// @param rect The total space to draw the grid over
  /// @param colWidth The width of a column in pixels
  /// @param rowHeight The height of a row in pixels
  /// @param unit The main gantt chart unit. This is the unit shown in the lower gantt axis and the
  ///             unit that dictates the placement of data in the chart.
  /// @param otherUnit The other unit used by the gantt chart. This unit is displayed on the upper
  ///                  gantt axis.
  /// @param startDate The start date of the gantt chart - ie. the time value of the left-most
  ///                  point on the chart
  /// @param rows The number of rows in the chart
  /// @param cols The number of columns in the chart
  virtual void drawGrid(QPainter *painter, const QRect &rect, int colWidth,
                        int rowHeight, GanttUnit unit, GanttUnit otherUnit,
                        const QDateTime &startDate, int rows, int cols);

  ///
  /// @brief Draws a bar representing a item in the gantt chart
  ///
  /// @param painter The painter object to use to do the drawing
  /// @param rect The rect encompassing the area for the bar. The delegate implementation is free
  ///             to draw anywhere within this rect for this bar.
  /// @param index The index of the item within the gantt model. This can be used to lookup
  ///              additional information about the item.
  virtual void drawBar(QPainter *painter, const QRect &rect,
                       const QModelIndex &index);

  ///
  /// @brief Draws a connection between two bars within the chart.
  ///
  /// @param painter The painter object to use to do the drawing
  /// @param src The rect of the source gantt item
  /// @param dst The rect of the destination gantt item
  /// @param srcIndex The index of the source item within the gantt model. This can be used to
  ///              lookup additional information about the item.
  /// @param dstIndex The index of the destiunation item within the gantt model. This can be used
  ///                 to lookup additional information about the item.
  virtual void drawConnection(QPainter *painter, const QRect &src,
                              const QRect &dst, const QModelIndex &srcIndex,
                              const QModelIndex &dstIndex);

  ///
  /// @brief Returns whether or not the given time value should be bolded on the graph
  ///
  /// @param value The time value being considered.
  /// @param unit The main gantt chart unit. This is the unit shown in the lower gantt axis and the
  ///             unit that dictates the placement of data in the chart.
  /// @param otherUnit The other unit used by the gantt chart. This unit is displayed on the upper
  ///                  gantt axis.
  virtual bool shouldBeBold(const QDateTime &value, GanttUnit unit,
                            GanttUnit otherUnit) const;

private:
};
