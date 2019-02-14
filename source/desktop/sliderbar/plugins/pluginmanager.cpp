#include "pluginmanager.h"

#include <QDebug>
#include <QDir>

namespace sliderbar {

PluginManager::PluginManager()
{
    loadPlugins();
}

PluginManager::~PluginManager()
{
    for (auto plugin : m_plugins)
        delete plugin;
}

void PluginManager::loadPlugins()
{
    QDir dir("plugins");
    QStringList filters;
    filters << "*.lua";
    dir.setNameFilters(filters);

    // Open all files and create a plugin for each file
    QStringList filenames = dir.entryList(QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks);
    for (auto filename : filenames) {
        QFile file(dir.absoluteFilePath(filename));
        file.open(QIODevice::ReadOnly | QIODevice::Text);

        auto name = QFileInfo(dir.absoluteFilePath(filename)).completeBaseName().toStdString();

        Plugin* loadedPlugin = new Plugin(name, file.readAll().toStdString());
        m_plugins.push_back(loadedPlugin);
    }

    // Verify all plugins are valid (Sol2's lua.script(blabla).valid())
    auto it = m_plugins.begin();
    while (it != m_plugins.end()) {
        if (!(*it)->isValid()) {
            delete (*it);
            it = m_plugins.erase(it);
        } else
            it++;
    }
}

std::vector<Plugin*> PluginManager::getPlugins()
{
    return m_plugins;
}

void PluginManager::processPosition(float position)
{
    auto it = m_plugins.begin();
    while (it != m_plugins.end()) {
        (*it)->processPosition(position);
        it++;
    }
}

}
