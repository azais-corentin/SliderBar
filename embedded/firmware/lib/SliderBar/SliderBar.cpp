#include "SliderBar.h"

#include "protocol/protodef.h"

SliderBar* g_sliderbar_ptr = nullptr;

SliderBar::SliderBar()
{
    g_sliderbar_ptr = this;
}

SliderBar::~SliderBar()
{
}

void SliderBar::run()
{
    while (1) {
        // Decode received data
        decode();
        // Execute commands
    }
}

void SliderBar::receive(uint8_t* buf, uint32_t* len)
{
    if (m_buffer.full())
        m_buffer.clear();

    m_buffer.append(buf, *len);
}

void SliderBar::decode()
{
    // Makes sure buffer contains at least 1 startflag & 1 endflag
    if (!m_buffer.contains(startflag) || !m_buffer.contains(endflag)) {
        m_buffer.clear();
        return;
    }

    // Makes sure buffer starts with the first startflag
    m_buffer = m_buffer.mid(m_buffer.indexOf(startflag));

    // Makes sure buffer ends with the last endflag
    m_buffer = m_buffer.left(m_buffer.lastIndexOf(endflag) + 1);

    // Packet is complete -- remove startflag and endflag
    Buffer data = m_buffer.mid(1);
    data.chop(1);
    int i = 0;

    // Extract data and CRC
    uint8_t type = decode8(data, i);
    uint16_t value = decode16(data, i);
    uint8_t crc_received = decode8(data, i);

    // Computes CRC
    Buffer receivedBytes;
    encode8(receivedBytes, type, false);
    encode16(receivedBytes, value, false);
    uint8_t crc_computed = CRC::compute(receivedBytes.data(), receivedBytes.size());

    // Compares CRC (computed vs. received)
    if (crc_computed != crc_received) {
        m_buffer.clear();
        return;
    }

    command received;
    received.type = static_cast<command::command_type>(type);
    received.value = value;

    // Send Acknowledgement for command types other than position
    // (avoids overhead)
    if (received.type < command::FORC_POSITION && received.type != command::FORS_POSITION)
        sendAck();

    //Process the received packet
    m_receiver(received);

    m_buffer.clear();
}