#include "MouseWheel.h"

#include <QDebug>

MouseWheelPlugin::MouseWheelPlugin()
{
    qDebug() << "Loaded MouseWheel plugin";
}

MouseWheelPlugin::~MouseWheelPlugin()
{
    qDebug() << "Unloaded MouseWheel plugin";
}
