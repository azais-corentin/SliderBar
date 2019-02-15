#include "settings.h"

namespace sliderbar {

template <class T>
void Settings::pluginSetting(const std::string plugin, const std::string setting, const T value)
{
    m_settings.setValue(QString("sliderbar/plugin/%1/%2")
                            .arg(QString::fromStdString(plugin))
                            .arg(QString::fromStdString(setting)),
                        value);
}

}
