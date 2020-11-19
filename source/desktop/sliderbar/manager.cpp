#include "manager.h"

#include "settingsdialog.h"

#include <pb_decode.h>
#include <pb_encode.h>
#include <protocol/messages/sliderbar.pb.h>
#include <protocol/protocol_definition.h>

#include "serialinterface.h"

#include <QDebug>

namespace sliderbar {

Manager::Manager(QWidget* parent)
    : m_parent(parent)
{
    m_pluginManager = new PluginManager;
    m_settings      = new Settings(this);
    m_dataInterface = new SerialInterface;

    setTransmitter(m_dataInterface);
    m_dataInterface->setReceiver(this);
}

void Manager::initialiseConnections()
{
    QObject::connect(m_dataInterface, &SerialInterface::connected,
                     this, &Manager::handleConnected);
    QObject::connect(m_dataInterface, &SerialInterface::disconnected,
                     this, &Manager::handleDisconnected);

    if (m_settings->autoconnect())
        connect();
}

void Manager::connect()
{
    m_dataInterface->connect();
}

void Manager::disconnect()
{
    m_dataInterface->disconnect();
}

Settings* Manager::settings()
{
    return m_settings;
}

void Manager::managePlugins()
{
}

void Manager::receive(uint8_t* buf, const uint16_t& len)
{
    m_dataBuffer.append(buf, len);

    // Makes sure buffer contains at least 1 startflag & 1 endflag
    if (!m_dataBuffer.contains(protocol::startflag) || !m_dataBuffer.contains(protocol::endflag)) {
        m_dataBuffer.clear();
        return;
    }

    // Makes sure buffer starts with the first startflag
    m_dataBuffer.mid(m_dataBuffer.indexOf(protocol::startflag));

    // Makes sure buffer ends with the last endflag
    m_dataBuffer.resize(m_dataBuffer.lastIndexOf(protocol::endflag) + 1);

    // Packet is complete -- remove startflag and endflag
    // TODO: Assume that there are multiple command packets in one buffer: split
    // the buffer with start/end flags and process each buffer independantly.
    m_dataBuffer.mid(1);
    m_dataBuffer.chop(1);

    // The buffer now contains:
    // - message
    // - crc

    // Decode packet using nanopb:
    Response decoded    = Response_init_zero;
    pb_istream_t stream = pb_istream_from_buffer(m_dataBuffer.data(), m_dataBuffer.size() - 4);
    bool status         = pb_decode(&stream, Response_fields, &decoded);

    if (!status) {
        m_dataBuffer.clear();
        return;
    }

    process(decoded);

    m_dataBuffer.clear();
}

bool Manager::transmit(uint8_t* buf, const uint16_t& len)
{
    if (!m_connected)
        connect();
    return transmitter->transmit(buf, len);
}

void Manager::transmit(const Request& request)
{
    uint8_t dataBuffer[64];
    pb_ostream_t stream = pb_ostream_from_buffer(dataBuffer, sizeof(dataBuffer));

    bool status = pb_encode(&stream, Request_fields, &request);

    if (!status)
        return;

    const size_t message_length = stream.bytes_written;

    // Create the data packet
    uint8_t buf[6 + message_length];

    // Append startflag
    buf[0] = protocol::startflag;
    // Append encoded message
    memcpy(buf + 1, dataBuffer, message_length);
    // Compute CRC32 over dataBuffer
    uint32_t crc = 1;
    // Append CRC32, from MSB to LSB
    buf[message_length + 1] = (crc >> 24);
    buf[message_length + 2] = (crc >> 16);
    buf[message_length + 3] = (crc >> 8);
    buf[message_length + 4] = (crc >> 0);
    // Append endflag
    buf[message_length + 5] = protocol::endflag;

    // Send buffer
    transmit(buf, 6 + message_length);
}

void Manager::setCalibration(const protocol::CalibrationData& data)
{
    // Create calibration request
    Request calReq       = Request_init_zero;
    calReq.which_payload = Request_calibration_tag;
    calReq.ack           = true;

    // Set SetCalibration request
    calReq.payload.calibration.which_request           = Request_Calibration_set_tag;
    calReq.payload.calibration.request.set.minPosition = data.minimumPosition;
    calReq.payload.calibration.request.set.maxPosition = data.maximumPosition;
    calReq.payload.calibration.request.set.minVelocity = data.minimumVelocity;
    calReq.payload.calibration.request.set.maxVelocity = data.maximumVeloicty;

    // Send request
    transmit(calReq);
}

void Manager::requestCalibration()
{
    // Create calibration request
    Request calReq       = Request_init_zero;
    calReq.which_payload = Request_calibration_tag;
    calReq.ack           = true;

    // Set GetCalibration request
    calReq.payload.calibration.which_request = Request_Calibration_get_tag;

    // Send request
    transmit(calReq);
}

void Manager::requestPing()
{
    // Create ping request
    Request pingRequest       = Request_init_zero;
    pingRequest.which_payload = Request_ping_tag;

    // Send request
    m_pingTime.start();
    transmit(pingRequest);
}

void Manager::handleConnected()
{
    emit connected();
    m_connected = true;
}

void Manager::handleDisconnected()
{
    emit disconnected();
    m_connected = false;
}

QWidget* Manager::getParent()
{
    return m_parent;
}

bool Manager::isConnected()
{
    return m_connected;
}

std::vector<Plugin*> Manager::getPlugins()
{
    return m_pluginManager->getPlugins();
}

void Manager::process(const Response& response)
{
    switch (response.which_payload) {
    case Response_calibrationData_tag:
        process(response.payload.calibrationData);
        break;
    case Response_value_tag:
        process(response.payload.value);
        break;

    case Response_nack_tag: {
    } break;
    case Response_ping_tag: {
        emit pingTime(m_pingTime.elapsed());
    } break;

    default:
        break;
    }
}

void Manager::process(const Response_CalibrationData& value)
{
    m_calibrationData.minimumPosition = value.minPosition;
    m_calibrationData.maximumPosition = value.maxPosition;
    m_calibrationData.minimumVelocity = value.minVelocity;
    m_calibrationData.maximumVeloicty = value.maxVelocity;

    emit calibrationData(m_calibrationData);
}

void Manager::process(const Response_Value& value)
{
    switch (value.which_parameter) {
    case Response_Value_position_tag: {
        // Got a new position
        // Send the new position to the plugins
        float position = 100.f * value.parameter.position / m_calibrationData.maximumPosition;
        m_pluginManager->processPosition(position);
    } break;
    case Response_Value_velocity_tag:
        // Got a new velocity
        break;
    }
}

} // namespace SliderBar
