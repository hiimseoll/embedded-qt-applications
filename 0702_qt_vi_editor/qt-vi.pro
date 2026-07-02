QT       += core gui widgets

CONFIG += c++17

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    highlighter.cpp

HEADERS += \
    mainwindow.h \
    highlighter.h

target.path = /usr/local/bin
INSTALLS += target
