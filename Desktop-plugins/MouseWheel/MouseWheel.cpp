#include "MouseWheel.h"

#include <QDebug>

#include "windows.h"

MouseWheelPlugin::MouseWheelPlugin()
{
    m_active = false;

    qDebug() << "Loaded MouseWheel plugin";
}

MouseWheelPlugin::~MouseWheelPlugin()
{
    qDebug() << "Unloaded MouseWheel plugin";
}

void MouseWheelPlugin::mouseWheel(int n)
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
