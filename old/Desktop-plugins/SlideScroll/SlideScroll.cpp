#include "SlideScroll.h"

#include <QDebug>

#include "windows.h"

SlideScrollPlugin::SlideScrollPlugin()
{
    m_active = false;

    qDebug() << "Loaded MouseWheel plugin";
}

SlideScrollPlugin::~SlideScrollPlugin()
{
    qDebug() << "Unloaded MouseWheel plugin";
}

SliderSettings SlideScrollPlugin::exposeSettings()
{
    SliderSettings settings;
    settings["Wheel scroll multiplier (scroll/mm)"] = m_scrollMultiplier;

    return settings;
}

void SlideScrollPlugin::updateSettings(SliderSettings& settings)
{
    if (settings.contains("Wheel scroll multiplier (scroll/mm)"))
    {
        m_scrollMultiplier = settings.value("Wheel scroll multiplier (scroll/mm)").toFloat();
        return;
    }

    qDebug() << "MouseWheel error: wrong SliderSettings values";
}

QVector<SliderEventType> SlideScrollPlugin::getEventTypes()
{
    return QVector<SliderEventType>(SLIDER_DELTA);
}

void SlideScrollPlugin::processEvent(SliderEventType type, QVariant value)
{
    if (type == SLIDER_DELTA)
        mouseWheel(value.toFloat() * m_scrollMultiplier * WHEEL_DELTA);
}

void SlideScrollPlugin::mouseWheel(float n)
{
    INPUT input;
    input.type = INPUT_MOUSE;
    input.mi.dx = 0;
    input.mi.dy = 0;
    input.mi.dwFlags = MOUSEEVENTF_WHEEL;
    input.mi.time = 0;
    input.mi.mouseData = static_cast<DWORD>(n);

    SendInput(1, &input, sizeof(input));
}
