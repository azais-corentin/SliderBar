#include "pluginsettingsdialog.h"

PluginsSettingDialog::PluginsSettingDialog(SliderSettings settings)
    : m_settings(settings)
{
    int row = 0;
    auto i = m_settings.constBegin();
    while (i != m_settings.constEnd())
    {
        addWidget(new QLabel(i.key()), row, 0);
        auto val = i.value();
        if (val.canConvert<float>())
            addWidget(new QDoubleSpinBox(), row, 0);
        row++;
    }
}

SliderSettings PluginsSettingDialog::getSettings()
{
    int row = m_settings.size();
    while (row > 0)
    {
        auto widget = itemAtPosition(row, 1)->widget();
        auto name = qobject_cast<QLabel*>(itemAtPosition(row, 0)->widget())->text();
        auto type = m_settings.value(name);
        if (type.canConvert<float>())
            m_settings.insert(name, qobject_cast<QDoubleSpinBox*>(widget)->value());
        row--;
    }

    return m_settings;
}
