#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <memory>
#include <vector>

#include "plugin.h"

namespace sliderbar {

struct PluginContainer {
    std::string name;
    Plugin plugin;
};

/**
 * @brief The PluginManager class
 * The PluginManager class acts as an interface between the SliderBar Manager
 * and the user.
 *
 * It is in charge of loading the plugins, exposing the settings of the plugins
 * and managing the activations of the pluigns.
 *
 */
class PluginManager {
public:
    PluginManager();
    ~PluginManager();

    void loadPlugins();

private:
    std::vector<PluginContainer> m_plugins;
};

}

#endif // PLUGINMANAGER_H
