QT += \
    widgets \
    network \
    charts \
    sql	\
    webenginewidgets

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
    tab3controlpanel.cpp \
    tab4sensorchart.cpp \
    tab5sensordatabase.cpp \
    tab6webcamera.cpp \
    tab7camopencv.cpp \
    webcamthread.cpp

HEADERS += \
    keyboard.h \
    ledkeydev.h \
    mainwidget.h \
    socketclient.h \
    tab1devicecontrol.h \
    tab2socketclient.h \
    tab3controlpanel.h \
    tab4sensorchart.h \
    tab5sensordatabase.h \
    tab6webcamera.h \
    tab7camopencv.h \
    webcamthread.h

FORMS += \
    keyboard.ui \
    mainwidget.ui \
    tab1devicecontrol.ui \
    tab2socketclient.ui \
    tab3controlpanel.ui \
    tab4sensorchart.ui \
    tab5sensordatabase.ui \
    tab6webcamera.ui \
    tab7camopencv.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    AIoTClient.qrc

#added
INCLUDEPATH += /usr/include/opencv4
LIBS += -L/usr/lib64 \
        -lopencv_videoio \
        -lopencv_highgui \
        -lopencv_imgcodecs \
        -lopencv_imgproc \
        -lopencv_core

#added