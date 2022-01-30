cmake_minimum_required(VERSION 3.5)

project(gearboxplus LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

set(PROJECT_SOURCES
        src/main.cpp
        src/cores/basecore.hpp
        src/cores/a06core.cpp
        src/cores/a04core.cpp
        src/cores/corefactory.cpp
        src/mainwindow.cpp
        src/mainwindow.hpp
        resources.qrc
)


add_executable(gearboxplus ${PROJECT_SOURCES})


target_link_libraries(gearboxplus PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
