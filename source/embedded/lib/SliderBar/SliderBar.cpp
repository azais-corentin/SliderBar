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

    Example msg;
    msg.value = 1255;
    encode(msg);

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
    Example decoded     = Example_init_zero;
    pb_istream_t stream = pb_istream_from_buffer(m_decodeBuffer.data(), m_decodeBuffer.size() - 4);
    bool status         = pb_decode(&stream, Example_fields, &decoded);

    if (!status) {
        m_decodeBuffer.clear();
        newData = false;
        return;
    }

    // verify crc32

    //protocol::command received = protocol::decode(*m_decodeBuffer);

    // Send Acknowledgement for command types other than position
    // (avoids overhead)
    //using command_type = protocol::command::command_type;

    // TODO: Add the Ack to the queue.
    /*if (received.type < FORC_POSITION && received.type != FORS_POSITION)
        sendAck();*/

    // TODO: Add the command to a queue; process the queue in the run() loop.

    m_decodeBuffer.clear();
    newData = false;
}

void SliderBar::encode(const Example& msg)
{
    uint8_t dataBuffer[64];
    pb_ostream_t stream = pb_ostream_from_buffer(dataBuffer, sizeof(dataBuffer));

    bool status = pb_encode(&stream, Example_fields, &msg);

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