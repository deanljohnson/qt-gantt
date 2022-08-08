#include <QApplication>

#include "GanttChartGraphArea.h"
#include "GanttChartItem.h"
#include "GanttChartView.h"
#include "GanttTreeItem.h"
#include <QDate>
#include <QStandardItemModel>
#include <vector>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  GanttChartView view;
  QStandardItemModel model;

  /*{
    auto *item1 = model.addItem(nullptr);
    auto *item2 = model.addItem(nullptr);
    item1->setLongText("test1");
    item1->setShortText("test1");
    item1->setStart(QDate::currentDate().startOfDay());
    item1->setStop(QDate::currentDate().addDays(3).startOfDay());
    item1->setFillColor(QColor("Green"));
    item1->setBackgroundColor(item1->fillColor().darker());
    item2->setLongText("test2");
    item2->setStart(QDate::currentDate().addDays(3).startOfDay());
    item2->setStop(QDate::currentDate().addDays(7).startOfDay());
    item2->setFillColor(QColor("Orange"));
    item2->setBackgroundColor(item2->fillColor().darker());
    item2->setFillAmount(0.7);

    item1->setConnectedItems({item2});

    auto *item1_1 = model.addItem(item1);
    auto *item2_2 = model.addItem(item2);
    item1_1->setLongText("test1_1");
    item1_1->setStart(QDate::currentDate().addDays(5).startOfDay());
    item1_1->setStop(QDate::currentDate().addDays(7).startOfDay());
    item2_2->setLongText("test2_2");
    item2_2->setStart(QDate::currentDate().addDays(7).startOfDay());
    item2_2->setStop(QDate::currentDate().addDays(9).startOfDay());

    view.setMinDateTime(QDate::currentDate().startOfDay());
    view.setMaxDateTime(QDate::currentDate().addDays(20).startOfDay());
  }*/

  {
    // Setup the tree
    GanttTreeItem item1("test1");
    GanttTreeItem item1_1("test1_1");
    GanttTreeItem item2("test2");
    GanttTreeItem item2_1("test2_1");

    item1.appendRow(&item1_1);
    item2.appendRow(&item2_1);

    model.appendRow(&item1);
    model.appendRow(&item2);

    // Setup the chart
    GanttChartItem cItem1_0_1;
    GanttChartItem cItem1_1_1;
    GanttChartItem cItem1_1_2;
    GanttChartItem cItem2_0_1;
    GanttChartItem cItem2_1_1;
    GanttChartItem cItem2_1_2;

    item1.setChartItems({&cItem1_0_1});
    item1_1.setChartItems({&cItem1_1_1, &cItem1_1_2});
    item2.setChartItems({&cItem2_0_1});
    item2_1.setChartItems({&cItem2_1_1, &cItem2_1_2});

    cItem1_0_1.setLongText("test1");
    cItem1_0_1.setShortText("test1");
    cItem1_0_1.setStart(QDateTime::currentDateTime().addSecs(0 * 3600));
    cItem1_0_1.setStop(QDateTime::currentDateTime().addSecs(3 * 3600));
    cItem1_0_1.setFillColor(QColor("Green"));
    cItem1_0_1.setFillColor(cItem1_0_1.fillColor().darker());

    cItem2_0_1.setLongText("test2");
    cItem2_0_1.setShortText("test2");
    cItem2_0_1.setStart(QDateTime::currentDateTime().addSecs(3 * 3600));
    cItem2_0_1.setStop(QDateTime::currentDateTime().addSecs(7 * 3600));
    cItem2_0_1.setFillColor(QColor("Orange"));
    cItem2_0_1.setFillColor(cItem2_0_1.fillColor().darker());
    cItem2_0_1.setFillAmount(0.7);

    // TODO: child items

    /* auto *item1_1 = model.addItem(item1);
     auto *item2_2 = model.addItem(item2);
     item1_1->setLongText("test1_1");
     item1_1->setStart(QDateTime::currentDateTime().addSecs(5 * 3600));
     item1_1->setStop(QDateTime::currentDateTime().addSecs(7 * 3600));
     item2_2->setLongText("test2_2");
     item2_2->setStart(QDateTime::currentDateTime().addSecs(7 * 3600));
     item2_2->setStop(QDateTime::currentDateTime().addSecs(9 * 3600));

     view.graphArea()->setTopUnit(GanttUnit::Day);
     view.graphArea()->setBottomUnit(GanttUnit::Hour);*/

    view.setMinDateTime(
        roundDown(QDateTime::currentDateTime(), GanttUnit::Hour));
    view.setMaxDateTime(roundDown(
        QDateTime::currentDateTime().addSecs(20 * 3600), GanttUnit::Hour));
  }

  view.setModel(&model);
  view.show();

  return app.exec();
}