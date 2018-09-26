#ifndef MOUSEWHEEL_H
#define MOUSEWHEEL_H

#include <QObject>
#include <QtPlugin>

#include "sliderinterface.h"

class MouseWheelPlugin : public SliderInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.Nelieru.SliderBar.SliderInterface/0.3" FILE "MouseWheel.json")
    Q_INTERFACES(SliderInterface)

public:
    MouseWheelPlugin();
    ~MouseWheelPlugin();

    SliderSettings exposeSettings();
    void updateSettings(SliderSettings& settings);

    QVector<SliderEventType> getEventTypes();
public slots:
    void processEvent(SliderEventType type, QVariant value);

private:
    void mouseWheel(float n);

    float m_scrollMultiplier = 20;
};

#endif // MOUSEWHEEL_H
