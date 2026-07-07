QT += widgets
QT += network

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    keyboard.cpp \
    ledkeydev.cpp \
    main.cpp \
    mainwidget.cpp \
    socketclient.cpp \
    tab1devicecontrol.cpp \
    tab2socketclient.cpp \
    tab3controlpanel.cpp

HEADERS += \
    keyboard.h \
    ledkeydev.h \
    mainwidget.h \
    socketclient.h \
    tab1devicecontrol.h \
    tab2socketclient.h \
    tab3controlpanel.h

FORMS += \
    keyboard.ui \
    mainwidget.ui \
    tab1devicecontrol.ui \
    tab2socketclient.ui \
    tab3controlpanel.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    AIoTClient.qrc
