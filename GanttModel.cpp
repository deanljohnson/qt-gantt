#include "GanttModel.h"

int GanttModel::totalRowCount(const QModelIndex &parent) const {
  // TODO: cache
  if (!hasChildren(parent))
    return 0;
  int initialCount = rowCount(parent);
  int count = initialCount;
  for (int r = 0; r < initialCount; r++) {
    const auto child = index(r, 0, parent);
    count += totalRowCount(child);
  }
  return count;
}
