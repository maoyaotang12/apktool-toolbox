QT       += core gui widgets

TARGET = apktool-toolbox
TEMPLATE = app

SOURCES += \
    main.cpp \
    widget.cpp

HEADERS += \
    widget.h

QMAKE_CXXFLAGS += -std=c++17
QMAKE_CXXFLAGS += -fno-lto
QMAKE_CFLAGS   += -fno-lto
QMAKE_LFLAGS   += -fno-lto
