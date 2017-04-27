#-------------------------------------------------
#
# Project created by QtCreator 2017-04-26T21:10:12
#
#-------------------------------------------------

QT       += widgets serialport

TARGET = SlideBar
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += main.cpp\
        mainwindow.cpp \
    serialsettings.cpp

HEADERS  += mainwindow.h \
    serialsettings.h

FORMS    += mainwindow.ui \
    serialsettings.ui

RESOURCES +=
