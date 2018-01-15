CONFIG      += plugin c++11
TARGET      = SlideScroll
TEMPLATE    = lib

INCLUDEPATH += ../../Desktop

LIBS        += -lUser32

SOURCES     += SlideScroll.cpp
HEADERS     += SlideScroll.h

DISTFILES   += SlideScroll.json

DESTDIR     = ../../build-SliderBar-Desktop_Qt_5_9_1_MinGW_32bit-Debug/plugins
