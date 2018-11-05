#-------------------------------------------------
#
# Project created by QtCreator 2018-11-04T13:29:14
#
#-------------------------------------------------

QT       += widgets gui serialport

CONFIG += c++14

TARGET = SliderBar
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    sliderbar/sliderbar.cpp \
    sliderbar/settingsdialog.cpp

HEADERS += \
        mainwindow.h \
    sliderbar/sliderbar.h \
    sliderbar/settingsdialog.h

FORMS += \
        mainwindow.ui \
    sliderbar/settingsdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
