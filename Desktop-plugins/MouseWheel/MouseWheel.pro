CONFIG      += plugin c++11
TARGET      = MouseWheel
TEMPLATE    = lib

INCLUDEPATH += ../../Desktop

LIBS += -lUser32

SOURCES     += MouseWheel.cpp
HEADERS     += MouseWheel.h

DISTFILES   += MouseWheel.json

DESTDIR     = ../../build-SliderBar-Desktop_Qt_5_9_1_MinGW_32bit-Debug/plugins
