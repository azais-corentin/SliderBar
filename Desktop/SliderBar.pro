#-------------------------------------------------
#
# Project created by QtCreator 2017-04-26T21:10:12
#
#-------------------------------------------------

QT          += widgets serialport

TARGET      = SliderBar
TEMPLATE    = app

DEFINES     += QT_DEPRECATED_WARNINGS

SOURCES     += main.cpp\
            mainwindow.cpp \
            settingsdialog.cpp \
            serialprotocol.cpp \
    systemkeyboardhook.cpp

HEADERS     += mainwindow.h \
            settingsdialog.h \
            serialprotocol.h \
            serialpacketdefinition.h \
            sliderinterface.h \
    systemkeyboardhook.h

FORMS       += mainwindow.ui \
            settingsdialog.ui
