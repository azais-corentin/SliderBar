#ifndef SLIDESCROLL_H
#define SLIDESCROLL_H

#include <QObject>
#include <QtPlugin>

#include "sliderinterface.h"

class SlideScrollPlugin : public SliderInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.Nelieru.SliderBar.SliderInterface/0.2" FILE "SliderScroll.json")
    Q_INTERFACES(SliderInterface)

public:
    SlideScrollPlugin();
    ~SlideScrollPlugin();

    SliderSettings exposeSettings();
    void updateSettings(SliderSettings& settings);

    QVector<SliderEventType> getEventTypes();
public slots:
    void processEvent(SliderEventType type, QVariant value);

signals:

private:
    void mouseWheel(float n);
};

#endif // SLIDESCROLL_H
