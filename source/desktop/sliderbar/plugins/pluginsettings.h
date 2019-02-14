#ifndef PLUGINSETTINGS_H
#define PLUGINSETTINGS_H

#include <QFormLayout>
#include <QLineEdit>
#include <QWidget>

#include "plugin.h"

namespace sliderbar {

class PluginSettings : public QWidget {
    Q_OBJECT

public:
    explicit PluginSettings(Plugin* plugin, QWidget* parent = nullptr);
    ~PluginSettings() override;

private:
    QFormLayout* m_layout;
};

}

#endif // PLUGINSETTINGS_H
