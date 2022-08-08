cmake_minimum_required(VERSION 3.20)

set(VCPKG_TARGET_TRIPLET "x64-windows")
set(VCPKG_HOST_TRIPLET "x64-windows")

project(gantt LANGUAGES CXX)

find_package(Qt5Core CONFIG REQUIRED)
find_package(Qt5Widgets CONFIG REQUIRED)

add_library(gantt)
target_sources(gantt
    PRIVATE
        GanttChartDelegate.h
        GanttChartDelegate.cpp
        GanttChartGraphArea.h
        GanttChartGraphArea.cpp
        GanttChartView.h
        GanttChartView.cpp
        GanttModel.h
        GanttModel.cpp
        GanttUnit.h
        GanttTreeItem.h
        GanttTreeItem.cpp
        GanttChartItem.h
        GanttChartItem.cpp
)
target_link_libraries(gantt PUBLIC Qt5::Core Qt5::Widgets)

add_executable(gantt_example)
target_sources(gantt_example
    PRIVATE
        main.cpp
)
target_link_libraries(gantt_example PRIVATE gantt)
