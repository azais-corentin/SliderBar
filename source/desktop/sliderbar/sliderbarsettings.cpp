#include "sliderbarsettings.h"

#include "settingsdialog.h"
#include "sliderbar.h"

SliderBarSettings::SliderBarSettings(SliderBar* parent)
    : m_parent(parent)
{
}

void SliderBarSettings::clear()
{
    m_settings.clear();
}

void SliderBarSettings::protocolStartFlag(const uint8_t& value)
{
    m_settings.setValue("sliderbar/protocol/startflag", value);
}

uint8_t SliderBarSettings::protocolStartFlag()
{
    return m_settings.value("sliderbar/protocol/startflag", 0x12).toUInt();
}

void SliderBarSettings::protocolEndFlag(const uint8_t& value)
{
    m_settings.setValue("sliderbar/protocol/endflag", value);
}

uint8_t SliderBarSettings::protocolEndFlag()
{
    return m_settings.value("sliderbar/protocol/endflag", 0x13).toUInt();
}

void SliderBarSettings::protocolEscapeFlag(const uint8_t& value)
{
    m_settings.setValue("sliderbar/protocol/escapeflag", value);
}

uint8_t SliderBarSettings::protocolEscapeFlag()
{
    return m_settings.value("sliderbar/protocol/escapeflag", 0x7D).toUInt();
}

void SliderBarSettings::protocolXORFlag(const uint8_t& value)
{
    m_settings.setValue("sliderbar/protocol/xorflag", value);
}

uint8_t SliderBarSettings::protocolXORFlag()
{
    return m_settings.value("sliderbar/protocol/xorflag", 0x20).toUInt();
}

void SliderBarSettings::protocolACKTimeout(const uint16_t& value)
{
    m_settings.setValue("sliderbar/protocol/acktimeout", value);
}

uint16_t SliderBarSettings::protocolACKTimeout()
{
    return m_settings.value("sliderbar/protocol/acktimeout", 50).toUInt();
}

void SliderBarSettings::calibration(const protocol::CalibrationData& data)
{
    m_settings.setValue("sliderbar/calibration/minpos", data.minimumPosition);
    m_settings.setValue("sliderbar/calibration/maxpos", data.maximumPosition);
    m_settings.setValue("sliderbar/calibration/minvel", data.minimumVelocity);
    m_settings.setValue("sliderbar/calibration/maxvel", data.maximumVeloicty);
}

protocol::CalibrationData SliderBarSettings::calibration()
{
    protocol::CalibrationData data;
    data.minimumPosition = m_settings.value("sliderbar/calibration/minpos", 0).toUInt();
    data.maximumPosition = m_settings.value("sliderbar/calibration/maxpos", 0).toUInt();
    data.minimumVelocity = m_settings.value("sliderbar/calibration/minvel", 0).toUInt();
    data.maximumVeloicty = m_settings.value("sliderbar/calibration/maxvel", 0).toUInt();

    return data;
}

void SliderBarSettings::showSettings()
{
    SettingsDialog settingsDialog(this, m_parent->getParent());

    // Connect signals
    QObject::connect(&settingsDialog, &SettingsDialog::requestCalibration, m_parent, &SliderBar::requestCalibration);
    QObject::connect(m_parent, &SliderBar::calibrationData, &settingsDialog, &SettingsDialog::receiveCalibrationData);

    if (settingsDialog.execute() == QDialog::Accepted) {
        emit settingsChanged();
    }
}

void SliderBarSettings::autoconnect(const bool& enabled)
{
    m_settings.setValue("sliderbar/autoconnect", enabled);
}

bool SliderBarSettings::autoconnect()
{
    return m_settings.value("sliderbar/autoconnect", true).toBool();
}
