#include "plugin.h"

namespace sliderbar {

template <class T>
void Plugin::setSetting(std::string name, T value)
{
    m_lua[name] = value;
}

template <class T>
T Plugin::getSetting(std::string name)
{
    return m_lua.get<T>(name);
}

}
