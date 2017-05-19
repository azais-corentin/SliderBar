#ifndef SLIDERINTERFACE_H
#define SLIDERINTERFACE_H

#include <QtPlugin>

class SliderInterface
{
public:
    virtual ~SliderInterface() {}

protected:
    bool m_active;
};

QT_BEGIN_NAMESPACE

Q_DECLARE_INTERFACE(SliderInterface, "org.Nelieru.SliderBar.SliderInterface/0.1")

QT_END_NAMESPACE

#endif // SLIDERINTERFACE_H
