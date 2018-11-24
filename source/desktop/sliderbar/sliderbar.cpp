#include "sliderbar.h"

#include "settingsdialog.h"

#include <pb_decode.h>
#include <pb_encode.h>
#include <protocol/messages/sliderbar.pb.h>
#include <protocol/protocol_definition.h>

#include <QDebug>

SliderBar::SliderBar(QWidget* parent)
    : m_parent(parent)
{
}

void SliderBar::connect()
{
}

void SliderBar::manageSettings()
{
    SettingsDialog set(m_parent);
    QObject::connect(&set, &SettingsDialog::requestCalibration, this, &SliderBar::requestCalibration);
    QObject::connect(this, &SliderBar::calibrationData, &set, &SettingsDialog::receiveCalibrationData);

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

void SliderBar::receive(uint8_t* buf, uint16_t len)
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

bool SliderBar::transmit(uint8_t* buf, uint16_t len)
{
    return transmitter->transmit(buf, len);
}

void SliderBar::transmit(const Request& request)
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

void SliderBar::requestCalibration()
{
    // Create calibration request
    Request calibrationRequest       = Request_init_zero;
    calibrationRequest.which_payload = Request_calibration_tag;
    calibrationRequest.ack           = true;

    // Send request
    transmit(calibrationRequest);
}

void SliderBar::requestPing()
{
    // Create ping request
    Request pingRequest       = Request_init_zero;
    pingRequest.which_payload = Request_ping_tag;

    // Send request
    m_pingTime.start();
    transmit(pingRequest);
}

void SliderBar::process(const Response& response)
{
    switch (response.which_payload) {
    case Response_calibrationData_tag:
        process(response.payload.calibrationData);
        break;
    case Response_value_tag:
        process(response.payload.value);
        break;
    case Response_ping_tag: {
        emit pingTime(m_pingTime.elapsed());
    } break;

    default:
        break;
    }
}

void SliderBar::process(const Response_CalibrationData& value)
{
    emit calibrationData(value.minPosition, value.maxPosition, value.minVelocity, value.maxVelocity);
}

void SliderBar::process(const Response_Value& value)
{
}
