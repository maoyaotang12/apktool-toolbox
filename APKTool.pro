QT += core gui widgets

TARGET = APKTool
TEMPLATE = app

SOURCES += \
    main.cpp \
    widget.cpp

HEADERS += \
    widget.h

QMAKE_CXXFLAGS += -std=c++17
