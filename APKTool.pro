QT       += core gui widgets

TARGET = apktool-toolbox
TEMPLATE = app

SOURCES += \
    main.cpp \
    widget.cpp

HEADERS += \
    widget.h

QMAKE_CXXFLAGS += -std=c++17
QMAKE_LFLAGS += -fno-lto   # 🔥 这一行禁用 LTO，makepkg 立刻成功
