#ifndef SLIDERBAR_H
#define SLIDERBAR_H

#include <QObject>

#include <QTime>

#include <Buffer/Buffer.hpp>
#include <DataInterface/DataInterface.h>
#include <protocol/messages/sliderbar.pb.h>
#include <protocol/protocol_definition.h>
#include <protocol/protocol_structures.h>

class SliderBar : public QObject, public DataOutInterface {
    Q_OBJECT

public:
    SliderBar(QWidget* parent);
    ~SliderBar() override = default;

public slots:
    /**
     * @brief Connects the SliderBar.
     */
    void connect();

    /**
     * @brief Show the settings window.
     * @note Blocking function.
     *
     */
    void manageSettings();

    /**
     * @brief Show the plugin management window.
     * @note Blocking function.
     */
    void managePlugins();

    /**
     * @brief Sets the autoconnect setting.
     * @param enabled True if autoconnect should be enabled.
     */
    void autoconnect(bool enabled);

    void receive(uint8_t* buf, uint16_t len) final;
    bool transmit(uint8_t* buf, uint16_t len) final;
    void transmit(const Request& request);

    void setCalibration(const protocol::calibrationData& data);
    void requestCalibration();
    void requestPing();

signals:
    void settingsChanged();
    void calibrationData(const protocol::calibrationData& data);
    void pingTime(uint32_t time);

private:
    void process(const Response& response);
    void process(const Response_CalibrationData& value);
    void process(const Response_Value& value);

private:
    QWidget* m_parent;
    Buffer<protocol::MAX_PACKET_SIZE> m_dataBuffer;

    QTime m_pingTime;
};

#endif // SLIDERBAR_H
