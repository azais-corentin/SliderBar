#ifndef SLIDERINTERFACE_H
#define SLIDERINTERFACE_H

#include <QtPlugin>

class SliderInterface
{
public:
    virtual ~SliderInterface() {}
};

QT_BEGIN_NAMESPACE

#define SliderInterface_iid "org.Nelieru.SliderBar.SliderInterface/0.1"
Q_DECLARE_INTERFACE(SliderInterface, SliderInterface_iid)

QT_END_NAMESPACE

#endif // SLIDERINTERFACE_H
