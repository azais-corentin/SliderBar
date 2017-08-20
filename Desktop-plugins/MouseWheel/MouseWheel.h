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

    SliderSettings exposeSettings();
    void updateSettings(SliderSettings& settings);

public slots:
    void processEvent(const SliderEventType& type, const QVariant& value);

signals:

private:
    void mouseWheel(int n);

    float m_scrollMultiplier = 20;
};

#endif // MOUSEWHEEL_H
