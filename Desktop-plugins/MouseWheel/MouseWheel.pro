CONFIG      += plugin
TARGET      = MouseWheel
TEMPLATE    = lib

INCLUDEPATH += ../../Desktop

LIBS += -lUser32

SOURCES     += MouseWheel.cpp
HEADERS     += MouseWheel.h

DISTFILES   += MouseWheel.json

DESTDIR     = ../../build-SliderBar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/plugins
