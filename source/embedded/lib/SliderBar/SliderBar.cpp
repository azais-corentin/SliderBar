#include "SliderBar.h"

#include <Buffer.hpp>

// Protocol
#include <messages/sliderbar.pb.h>
#include <pb_decode.h>
#include <pb_encode.h>

void SliderBar::receive(uint8_t* buf, uint16_t len)
{
    if (m_decodeBuffer.full())
        m_decodeBuffer.clear();

    m_decodeBuffer.append(buf, len);
    newData = true;
}

inline bool SliderBar::transmit(uint8_t* buf, uint16_t len)
{
    if (!transmitter)
        return false;
    return transmitter->transmit(buf, len);
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

    // The buffer now contains:
    // - message
    // - crc

    // Decode packet using nanopb:
    Request decoded     = Request_init_zero;
    pb_istream_t stream = pb_istream_from_buffer(m_decodeBuffer.data(), m_decodeBuffer.size() - 4);
    bool status         = pb_decode(&stream, Request_fields, &decoded);

    if (!status) {
        m_decodeBuffer.clear();
        newData = false;
        return;
    }

    // Compute & compare CRC32 over m_decodeBuffer

    // Processes packet
    process(decoded);

    m_decodeBuffer.clear();
    newData = false;
}

void SliderBar::transmit(const Response& msg)
{
    uint8_t dataBuffer[64];
    pb_ostream_t stream = pb_ostream_from_buffer(dataBuffer, sizeof(dataBuffer));

    bool status = pb_encode(&stream, Response_fields, &msg);

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

    default:
        // Error: unknown payload
        break;
    }
}

void SliderBar::process(const Request_SetValue& value)
{
    switch (value.which_parameter) {
    case Request_SetValue_position_tag:
        break;

    case Request_SetValue_velocity_tag:
        break;

    case Request_SetValue_P_tag:
        break;

    case Request_SetValue_I_tag:
        break;

    default:
        // Error: Unknown value in setValue
        break;
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
    calibrationData.payload.calibrationData.minPosition = 0;
    calibrationData.payload.calibrationData.maxPosition = 1023;
    calibrationData.payload.calibrationData.maxVelocity = 2500;

    // Send response
    transmit(calibrationData);
}

void SliderBar::calibrate()
{
}

void SliderBar::vibrate(uint32_t duration)
{
}