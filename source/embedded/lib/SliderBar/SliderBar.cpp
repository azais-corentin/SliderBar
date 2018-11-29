#include "SliderBar.h"

#include <Buffer.hpp>

// Protocol
#include <messages/sliderbar.pb.h>
#include <pb_decode.h>
#include <pb_encode.h>

void SliderBar::receive(uint8_t* buf, const uint16_t& len)
{
    if (m_decodeBuffer.full())
        m_decodeBuffer.clear();

    m_decodeBuffer.append(buf, len);
    newData = true;
}

inline bool SliderBar::transmit(uint8_t* buf, const uint16_t& len)
{
    if (!transmitter)
        return false;
    return transmitter->transmit(buf, len);
}

void SliderBar::transmit(const Response& response)
{
    uint8_t dataBuffer[64];
    pb_ostream_t stream = pb_ostream_from_buffer(dataBuffer, sizeof(dataBuffer));

    bool status = pb_encode(&stream, Response_fields, &response);

    if (!status)
        return;

    const size_t message_length = stream.bytes_written;

    // TODO: Implement escaping/unescaping of a buffer

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

void SliderBar::transmitNack()
{
    // Create NACK
    Response nack      = Response_init_zero;
    nack.which_payload = Response_nack_tag;

    // Send NACK
    transmit(nack);
}

void SliderBar::decode()
{
    if (!newData)
        return;

    // Makes sure buffer contains at least 1 startflag & 1 endflag
    if (!m_decodeBuffer.contains(protocol::startflag) || !m_decodeBuffer.contains(protocol::endflag)) {
        m_decodeBuffer.clear();
        newData = false;
        return;
    }

    // Makes sure buffer starts with the first startflag
    m_decodeBuffer.mid(m_decodeBuffer.indexOf(protocol::startflag));

    // Makes sure buffer ends with the last endflag
    m_decodeBuffer.resize(m_decodeBuffer.lastIndexOf(protocol::endflag) + 1);

    // Packet is complete -- remove startflag and endflag
    // TODO: Assume that there are multiple command packets in one buffer: split
    // the buffer with start/end flags and process each buffer independantly.
    m_decodeBuffer.mid(1);
    m_decodeBuffer.chop(1);
    newData = false;

    // The buffer now contains:
    // - message
    // - crc

    // TODO: Unescape the data before decoding, calculate CRC before unescaping

    // Decode request using Nanopb:
    Request decoded     = Request_init_zero;
    pb_istream_t stream = pb_istream_from_buffer(m_decodeBuffer.data(), m_decodeBuffer.size() - 4);
    bool status         = pb_decode(&stream, Request_fields, &decoded);

    if (!status) {
        m_decodeBuffer.clear();
        return;
    }

    // Compute & compare CRC32 over m_decodeBuffer
    uint32_t CRC = 1;
    if (CRC != 1) {
        m_decodeBuffer.clear();
        return;
    }

    // Processes packet
    process(decoded);

    m_decodeBuffer.clear();
}

void SliderBar::process(const Request& request)
{
    switch (request.which_payload) {
    case Request_setValue_tag:
        process(request.payload.setValue);
        break;
    case Request_getValue_tag:
        process(request.payload.getValue);
        break;
    case Request_vibrate_tag:
        process(request.payload.vibrate);
        break;
    case Request_resistAt_tag:
        process(request.payload.resistAt);
        break;
    case Request_resistClear_tag:
        process(request.payload.resistClear);
        break;
    case Request_calibration_tag:
        process(request.payload.calibration);
        break;
    case Request_ping_tag: {
        Response ping      = Response_init_zero;
        ping.which_payload = Response_ping_tag;
        transmit(ping);
    } break;

    default:
        transmitNack();
        break;
    }
}

void SliderBar::process(const Request_SetValue& value)
{
    switch (value.which_parameter) {
    case Request_SetValue_position_tag:
        m_values.position = value.parameter.position;
        break;

    case Request_SetValue_velocity_tag:
        m_values.velocity = value.parameter.velocity;
        break;

    case Request_SetValue_P_tag:
        m_values.gainP = value.parameter.P;
        break;

    case Request_SetValue_I_tag:
        m_values.gainI = value.parameter.I;
        break;

    default: {
        // Error: Unknown value in setValue
    } break;
    }
}

void SliderBar::process(const Request_GetValue& value)
{
    switch (value.which_period) {
    case Request_GetValue_position_tag:
        break;

    case Request_GetValue_velocity_tag:
        break;

    default:
        // Error: Unknown value in getValue
        break;
    }
}

void SliderBar::process(const Request_Vibrate& value)
{
    vibrate(value.duration);
}

void SliderBar::process(const Request_ResistAt& value)
{
}

void SliderBar::process(const Request_ResistClear& value)
{
}

void SliderBar::process(const Request_Calibration& value)
{
    // Computes calibration values
    calibrate();

    // Create response
    Response calibrationData                            = Response_init_zero;
    calibrationData.which_payload                       = Response_calibrationData_tag;
    calibrationData.payload.calibrationData.minPosition = m_calibrationData.minimumPosition;
    calibrationData.payload.calibrationData.maxPosition = m_calibrationData.maximumPosition;
    calibrationData.payload.calibrationData.minVelocity = m_calibrationData.minimumVelocity;
    calibrationData.payload.calibrationData.maxVelocity = m_calibrationData.maximumVeloicty;

    // Send response
    transmit(calibrationData);
}

void SliderBar::calibrate()
{
    // Find the mininmum velocity that causes a movement

    // Go the the minmum then maximum positions at minimum speed

    // Find the maximum velocity
}

void SliderBar::vibrate(uint32_t duration)
{
}