#ifndef PLUGINSETTINGSDIALOG_H
#define PLUGINSETTINGSDIALOG_H

#include <QGridLayout>
#include <QLabel>
#include <QDoubleSpinBox>

#include "sliderinterface.h"

class PluginsSettingDialog : public QGridLayout
{
public:
    PluginsSettingDialog(SliderSettings settings);

    SliderSettings getSettings();

private:
    SliderSettings m_settings;
};

#endif // PLUGINSETTINGSDIALOG_H
