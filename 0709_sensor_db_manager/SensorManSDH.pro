QT += 	widgets \
        network \
        charts \
        sql

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    keyboard.cpp \
    main.cpp \
    mainwidget.cpp \
    socketclient.cpp \
    tab1_socket.cpp \
    tab2_sensor.cpp \
    tab3_sqlite.cpp

HEADERS += \
    keyboard.h \
    mainwidget.h \
    socketclient.h \
    tab1_socket.h \
    tab2_sensor.h \
    tab3_sqlite.h

FORMS += \
    keyboard.ui \
    mainwidget.ui \
    tab1_socket.ui \
    tab2_sensor.ui \
    tab3_sqlite.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
