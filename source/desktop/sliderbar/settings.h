#ifndef SLIDERBARSETTINGS_H
#define SLIDERBARSETTINGS_H

#include <QSettings>

#include "protocol/protocol_structures.h"

namespace sliderbar {

class SettingsDialog;
class Plugin;
class Manager;

/**
 * @brief The SliderBar settings class
 *
 * This allows easy management of the settings of the SliderBar, without having
 * to work with QSetting's API and strings.
 *
 * @note This is simply a wrapper around QSettings.
 */
class Settings : public QObject {
    Q_OBJECT

public:
    Settings(Manager* parent);
    ~Settings() = default;

    void clear();

    /**
     * @brief Sets the autoconnect setting.
     * @param enabled True if autoconnect should be enabled next time.
     */
    void autoconnect(const bool& enabled);
    bool autoconnect();

    /**
     * @brief Set the protocolX value.
     * @param value The value to set.
     */
    void protocolStartFlag(const uint8_t& value);
    void protocolEndFlag(const uint8_t& value);
    void protocolEscapeFlag(const uint8_t& value);
    void protocolXORFlag(const uint8_t& value);
    void protocolACKTimeout(const uint16_t& value);

    /**
     * @brief Get the protocolX value.
     * @return The protocolX value.
     */
    uint8_t protocolStartFlag();
    uint8_t protocolEndFlag();
    uint8_t protocolEscapeFlag();
    uint8_t protocolXORFlag();
    uint16_t protocolACKTimeout();

    void calibration(const protocol::CalibrationData& data);
    protocol::CalibrationData calibration();

    template <class T>
    void pluginSetting(const std::string plugin, const std::string setting, const T value);
    bool pluginSetting_exists(const std::string plugin, const std::string setting) const;
    std::string pluginSetting_string(const std::string plugin, const std::string setting) const;
    double pluginSetting_double(const std::string plugin, const std::string setting) const;

    std::vector<Plugin*> getPlugins();
public slots:
    /**
     * @brief Show the settings in a modal widows.
     * @note This is a blocking function.
     *
     */
    void showSettings();

signals:
    void settingsChanged();

private:
    SettingsDialog* m_settingsDialog;
    Manager* m_manager;
    QSettings m_settings;
};

}

#include "settings_impl.tpp"

#endif // SLIDERBARSETTINGS_H
