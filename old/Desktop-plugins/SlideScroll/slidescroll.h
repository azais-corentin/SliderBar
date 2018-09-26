#ifndef SLIDESCROLL_H
#define SLIDESCROLL_H

#include <QObject>
#include <QtPlugin>

#include "sliderinterface.h"

class SlideScrollPlugin : public SliderInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.Nelieru.SliderBar.SliderInterface/0.3" FILE "SlideScroll.json")
    Q_INTERFACES(SliderInterface)

public:
    SlideScrollPlugin();
    ~SlideScrollPlugin();

    SliderSettings exposeSettings();
    void updateSettings(SliderSettings& settings);

    QVector<SliderEventType> getEventTypes();
public slots:
    void processEvent(SliderEventType type, QVariant value);

private:
    void mouseWheel(float n);

    float m_scrollMultiplier = 20;
};

#endif // SLIDESCROLL_H
