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

INCLUDEPATH += $$PWD/../common $$PWD/../common/nanopb $$PWD/libs

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    sliderbar/settingsdialog.cpp \
    ../common/Buffer/Buffer.tpp \
    ../common/nanopb/pb_common.c \
    ../common/nanopb/pb_decode.c \
    ../common/nanopb/pb_encode.c \
    ../common/protocol/protocol_definition.cpp \
    sliderbar/serialinterface.cpp \
    ../common/protocol/messages/sliderbar.pb.c \
    sliderbar/manager.cpp \
    sliderbar/settings.cpp

HEADERS += \
    mainwindow.h \
    sliderbar/settingsdialog.h \
    ../common/Buffer/Buffer.hpp \
    ../common/nanopb/pb.h \
    ../common/nanopb/pb_common.h \
    ../common/nanopb/pb_decode.h \
    ../common/nanopb/pb_encode.h \
    ../common/protocol/protocol_definition.h \
    ../common/protocol/messages/sliderbar.pb.h \
    ../common/DataInterface/DataInterface.h \
    sliderbar/serialinterface.h \
    ../common/protocol/messages/sliderbar.pb.h \
    ../common/protocol/protocol_structures.h \
    sliderbar/manager.h \
    sliderbar/settings.h

FORMS += \
        mainwindow.ui \
    sliderbar/settingsdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    ../common/protocol/messages/sliderbar.proto
