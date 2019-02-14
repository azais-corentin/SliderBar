#include "pluginsettings.h"

namespace sliderbar {

PluginSettings::PluginSettings(Plugin* plugin, QWidget* parent)
    : QWidget(parent)
{
    m_layout = new QFormLayout(this);
    setLayout(m_layout);

    // Load the plugin's exposed settings
    auto settings = plugin->getSettings();
    for (auto const& kv : settings) {
        QLineEdit* field = new QLineEdit();
        field->setPlaceholderText(QString::fromStdString(kv.second));
        m_layout->addRow(QString::fromStdString(kv.first), field);
    }
}

PluginSettings::~PluginSettings()
{
}

}
