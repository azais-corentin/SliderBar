#ifndef MOUSEWHEEL_H
#define MOUSEWHEEL_H

#include <QObject>
#include <QtPlugin>

#include "sliderinterface.h"

class MouseWheelPlugin : public QObject, public SliderInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.Nelieru.SliderBar.SliderInterface/0.1" FILE "MouseWheel.json")
    Q_INTERFACES(SliderInterface)

public:
    MouseWheelPlugin();
    ~MouseWheelPlugin();

public slots:

signals:

private:
    void mouseWheel(int n);
};

#endif // MOUSEWHEEL_H
