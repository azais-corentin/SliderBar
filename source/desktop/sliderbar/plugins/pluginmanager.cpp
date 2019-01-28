#include "pluginmanager.h"

#include <QDir>

namespace sliderbar {

PluginManager::PluginManager()
{
    loadPlugins();
}

PluginManager::~PluginManager()
{
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

        m_plugins.emplace_back(name, file.readAll().toStdString());
    }

    // Verify all plugins are valid (Sol2's lua.script(blabla).valid())
    auto it = m_plugins.begin();
    while (it != m_plugins.end()) {
        if (!(*it).isValid())
            it = m_plugins.erase(it);
        else
            it++;
    }
}

void PluginManager::processPosition(float position)
{
    auto it = m_plugins.begin();
    while (it != m_plugins.end()) {
        (*it).processPosition(position);
        it++;
    }
}

}
