#-------------------------------------------------
#
# Project created by QtCreator 2017-04-26T21:10:12
#
#-------------------------------------------------

QT          += widgets serialport printsupport

TARGET      = SliderBar
TEMPLATE    = app

DEFINES     += QT_DEPRECATED_WARNINGS

SOURCES     += main.cpp\
            mainwindow.cpp \
            settingsdialog.cpp \
            serialprotocol.cpp \
    systemkeyboardhook.cpp \
    qcustomplot.cpp \
    pluginsettingsdialog.cpp

HEADERS     += mainwindow.h \
            settingsdialog.h \
            serialprotocol.h \
            serialpacketdefinition.h \
            sliderinterface.h \
            systemkeyboardhook.h \
            qcustomplot.h \
    pluginsettingsdialog.h

FORMS       += mainwindow.ui \
            settingsdialog.ui
