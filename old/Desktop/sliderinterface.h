#ifndef SLIDERINTERFACE_H
#define SLIDERINTERFACE_H

#include <QtPlugin>
#include <QObject>

#include <QString>
#include <QVariant>
#include <QHash>

using SliderSettings = QHash<QString, QVariant>;

typedef enum SliderEventType
{
    SLIDER_POSITION,
    SLIDER_DELTA
} SliderEventType;

class SliderInterface
{
public:
    virtual ~SliderInterface() {}

    //expose the plugin's settings
    virtual SliderSettings exposeSettings() = 0;
    virtual void updateSettings(SliderSettings& settings) = 0;

    //add a way to register to certain events (sliderbar's dx)
    virtual QVector<SliderEventType> getEventTypes() = 0;

public /*slots*/:
    virtual void processEvent(SliderEventType type, QVariant value) = 0;

protected:
    bool m_active;
};

Q_DECLARE_INTERFACE(SliderInterface, "org.Nelieru.SliderBar.SliderInterface/0.3")

#endif // SLIDERINTERFACE_H
