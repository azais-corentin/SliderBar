#ifndef SLIDERINTERFACE_H
#define SLIDERINTERFACE_H

#include <QtPlugin>

#include <QString>
#include <QVariant>
#include <QHash>

using SliderSettings = QHash<QString, QVariant>;

class SliderInterface
{
public:
    typedef enum SliderEventType
    {
        SLIDER_POSITION
        SLIDER_MOVED
    } SliderEventType;

    virtual ~SliderInterface() {}

    //expose the plugin's settings
    virtual SliderSettings exposeSettings();
    virtual void updateSettings(SliderSettings& settings);

    //add a way to register to certain events (sliderbar's dx)
public slots:
    virtual void processEvent(const SliderEventType& type, const QVariant& value);

protected:
    bool m_active;
};

QT_BEGIN_NAMESPACE

Q_DECLARE_INTERFACE(SliderInterface, "org.Nelieru.SliderBar.SliderInterface/0.2")

QT_END_NAMESPACE

#endif // SLIDERINTERFACE_H
