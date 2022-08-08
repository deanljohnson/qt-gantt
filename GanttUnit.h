#pragma once

#include <QDateTime>

/// @brief A unit that can be display on a Gantt chart
enum class GanttUnit { Hour, Day, Week, Month };

/// @brief Adds a certain count of a given unit to a time value
inline QDateTime addUnit(const QDateTime &dateTime, GanttUnit unit, int count) {
  switch (unit) {
  case GanttUnit::Hour:
    return dateTime.addSecs(count * 3600);
  case GanttUnit::Day:
    return dateTime.addDays(count);
  case GanttUnit::Week:
    return dateTime.addDays(count * 7);
  case GanttUnit::Month:
    return dateTime.addMonths(count);
  }
  return dateTime;
}

/// @brief Gets the value of a given unit from a time value
inline int getUnitValue(const QDateTime &dateTime, GanttUnit unit) {
  switch (unit) {
  case GanttUnit::Hour:
    return dateTime.time().hour();
  case GanttUnit::Day:
    return dateTime.date().day();
  case GanttUnit::Week:
    return dateTime.date().weekNumber();
  case GanttUnit::Month:
    return dateTime.date().month();
  }
  return 0;
}

/// @brief Returns the difference between two times in the given unit
inline int unitsTo(const QDateTime &left, const QDateTime &right,
                   GanttUnit unit) {
  switch (unit) {
  case GanttUnit::Hour:
    return left.secsTo(right) / 3600;
  case GanttUnit::Day:
    return left.daysTo(right);
  case GanttUnit::Week:
    return right.date().weekNumber() - left.date().weekNumber();
  case GanttUnit::Month:
    return right.date().month() - left.date().month();
  }
  return 0;
}

/// @brief Rounds a time value down to the given unit
inline QDateTime roundDown(const QDateTime &value, GanttUnit unit) {
  static constexpr qint64 MsPerHour = 1000 * 60 * 60;
  switch (unit) {
  case GanttUnit::Hour:
    return QDateTime::fromMSecsSinceEpoch(
        (value.toMSecsSinceEpoch() / MsPerHour) * MsPerHour);
  case GanttUnit::Day:
    return value.date().startOfDay(value.timeSpec());
  case GanttUnit::Week: {
    const auto date = value.date();
    return date.addDays(-date.dayOfWeek()).startOfDay(value.timeSpec());
  }
  case GanttUnit::Month: {
    const auto date = value.date();
    return date.addDays(-date.day()).startOfDay(value.timeSpec());
  }
  }
}
