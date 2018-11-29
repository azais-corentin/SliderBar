#ifndef SLIDERBARSETTINGS_H
#define SLIDERBARSETTINGS_H

#include <QSettings>

#include "protocol/protocol_structures.h"

class SliderBar;

/**
 * @brief The SliderBar settings class
 *
 * This allows easy management of the settings of the SliderBar, without having
 * to work with QSetting's API and strings.
 *
 * @note This is simply a wrapper around QSettings.
 */
class SliderBarSettings : public QObject {
    Q_OBJECT

public:
    SliderBarSettings(SliderBar* parent);
    ~SliderBarSettings() = default;

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
    SliderBar* m_parent;
    QSettings m_settings;
};

#endif // SLIDERBARSETTINGS_H
