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

        PluginContainer container {
            QFileInfo(dir.absoluteFilePath(filename))
                .completeBaseName()
                .toStdString(),
            Plugin(file.readAll().toStdString())
        };

        m_plugins.push_back(container);
    }

    // Verify all plugins are valid after parsing
    auto it = m_plugins.begin();
    while (it != m_plugins.end()) {
        if (!(*it).plugin.isValid())
            it = m_plugins.erase(it);
        else
            it++;
    }
}

}
