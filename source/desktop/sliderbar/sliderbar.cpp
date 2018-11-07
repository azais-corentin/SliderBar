#include "sliderbar.h"

#include "settingsdialog.h"

SliderBar::SliderBar(QWidget* parent)
    : m_parent(parent)
{
}

SliderBar::~SliderBar() = default;

void SliderBar::connect()
{
}

void SliderBar::manageSettings()
{
    SettingsDialog set(m_parent);
    if (set.execute() == QDialog::Accepted) {
        emit settingsChanged();
    }
}

void SliderBar::managePlugins()
{
}

void SliderBar::autoconnect(bool enabled)
{
    QSettings set;
    set.setValue("sliderbar/autoconnect", enabled);
}
