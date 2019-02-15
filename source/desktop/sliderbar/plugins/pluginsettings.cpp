#include "pluginsettings.h"

#include "plugin.h"
#include "sliderbar/settings.h"

namespace sliderbar {

PluginSettings::PluginSettings(Settings* settings, Plugin* plugin, QWidget* parent)
    : QWidget(parent)
    , m_settings(settings)
    , m_plugin(plugin)
{
    m_layout = new QFormLayout(this);
    setLayout(m_layout);

    // Load the plugin's exposed settings
    auto pluginSettings = m_plugin->getSettings();
    for (auto const& setting : pluginSettings) {
        QLineEdit* field = new QLineEdit();
        field->setPlaceholderText(QString::fromStdString(setting.description));
        m_fields.push_back(field);
        m_layout->addRow(QString::fromStdString(setting.name), field);
    }
}

PluginSettings::~PluginSettings()
{
}

void PluginSettings::saveSettings()
{
    // Save settings to disk & Load it to plugin runtime
    auto pluginSettings = m_plugin->getSettings();
    for (int i = 0; i < pluginSettings.size(); i++) {
        auto setting = pluginSettings.at(i);
        switch (setting.type) {
        case sol::type::number:
            m_settings->pluginSetting(m_plugin->getName(), setting.name, m_fields.at(i)->text().toDouble());
            m_plugin->setSetting(setting.name, m_fields.at(i)->text().toDouble());
            break;
        case sol::type::string:
            m_settings->pluginSetting(m_plugin->getName(), setting.name, m_fields.at(i)->text());
            m_plugin->setSetting(setting.name, m_fields.at(i)->text().toStdString());
            break;
        default:;
        }
    }
}

void PluginSettings::loadSettings()
{
    auto pluginSettings = m_plugin->getSettings();
    for (int i = 0; i < pluginSettings.size(); i++) {
        auto setting = pluginSettings.at(i);
        // Load settings from the disk if it exists
        if (m_settings->pluginSetting_exists(m_plugin->getName(), setting.name)) {
            m_fields.at(i)->setText(QString::fromStdString(m_settings->pluginSetting_string(m_plugin->getName(), setting.name)));
            switch (setting.type) {
            case sol::type::number:
                m_plugin->setSetting(setting.name, m_fields.at(i)->text().toDouble());
                break;
            case sol::type::string:
                m_plugin->setSetting(setting.name, m_fields.at(i)->text().toStdString());
                break;
            default:;
            }

        }
        // Otherwise load the values defined in the plugin's .lua
        else {
            // May not work
            m_fields.at(i)->setText(QString::fromStdString(m_plugin->getSetting<std::string>(setting.name)));
        }
    }
}

}
