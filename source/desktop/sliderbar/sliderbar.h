#ifndef SLIDERBAR_H
#define SLIDERBAR_H

#include <QObject>

#include <QTime>

#include <Buffer/Buffer.hpp>
#include <DataInterface/DataInterface.h>
#include <protocol/messages/sliderbar.pb.h>
#include <protocol/protocol_definition.h>
#include <protocol/protocol_structures.h>

#include "serialinterface.h"
#include "sliderbarsettings.h"

class SliderBar : public QObject, public DataOutInterface {
    Q_OBJECT

public:
    SliderBar(QWidget* parent);
    ~SliderBar() override = default;

    void initialiseConnections();
    QWidget* getParent();
    bool isConnected();

public slots:
    /**
     * @brief Connects the SliderBar.
     */
    void connect() final;

    /**
     * @brief Disconnects the SliderBar.
     */
    void disconnect() final;

    /**
     * @brief Returns an access to settings.
     * @return The settings class.
     */
    SliderBarSettings* settings();

    /**
     * @brief Show the plugin management window.
     * @note Blocking function.
     */
    void managePlugins();

    void receive(uint8_t* buf, const uint16_t& len) final;
    bool transmit(uint8_t* buf, const uint16_t& len) final;
    void transmit(const Request& request);

    void setCalibration(const protocol::CalibrationData& data);
    void requestCalibration();
    void requestPing();

private slots:
    void handleConnected();
    void handleDisconnected();

signals:
    void connected();
    void disconnected();

    void calibrationData(const protocol::CalibrationData& data);
    void pingTime(uint32_t time);

private:
    void process(const Response& response);
    void process(const Response_CalibrationData& value);
    void process(const Response_Value& value);

private:
    QWidget* m_parent;
    SliderBarSettings* m_settings;

    Buffer<protocol::MAX_PACKET_SIZE> m_dataBuffer;

    QTime m_pingTime;

    SerialInterface* m_dataInterface = nullptr;

    bool m_connected = false;
};

#endif // SLIDERBAR_H
