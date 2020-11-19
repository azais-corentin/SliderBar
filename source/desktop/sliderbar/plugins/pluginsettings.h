#ifndef PLUGINSETTINGS_H
#define PLUGINSETTINGS_H

#include <QFormLayout>
#include <QLineEdit>
#include <QWidget>

namespace sliderbar {
class Settings;
class Plugin;

class PluginSettings : public QWidget {
    Q_OBJECT

public:
    explicit PluginSettings(Settings* settings, Plugin* plugin, QWidget* parent = nullptr);
    ~PluginSettings() override;

    void saveSettings();
    void loadSettings();

private:
    QFormLayout* m_layout;
    Settings* m_settings;
    Plugin* m_plugin;

    std::vector<QLineEdit*> m_fields;
};

}

#endif // PLUGINSETTINGS_H
