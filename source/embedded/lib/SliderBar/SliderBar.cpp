#include "SliderBar.h"

#include <Buffer.hpp>
#include <protocol/protodec.h>

SliderBar* g_sliderbar_ptr = nullptr;

SliderBar::SliderBar()
{
    m_buffer = new Buffer<protocol::MAX_PACKET_SIZE>();

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

void SliderBar::receive(uint8_t* buf, uint8_t len)
{
    if (m_buffer->full())
        m_buffer->clear();

    m_buffer->append(buf, len);
    newData = true;
}

void SliderBar::decode()
{
    if (!newData)
        return;

    // Makes sure buffer contains at least 1 startflag & 1 endflag
    if (!m_buffer->contains(protocol::startflag) || !m_buffer->contains(protocol::endflag)) {
        m_buffer->clear();
        return;
    }

    // Makes sure buffer starts with the first startflag
    m_buffer->mid(m_buffer->indexOf(protocol::startflag));

    // Makes sure buffer ends with the last endflag
    m_buffer->resize(m_buffer->lastIndexOf(protocol::endflag) + 1);

    // Packet is complete -- remove startflag and endflag
    // TODO: Assume that there are multiple command packets in one buffer: split
    // the buffer with start/end flags and process each buffer independantly.
    m_buffer->mid(1);
    m_buffer->chop(1);

    // Decode packet using protodec:
    protocol::command received = protocol::decode(*m_buffer);

    // Send Acknowledgement for command types other than position
    // (avoids overhead)
    //using command_type = protocol::command::command_type;

    // TODO: Add the Ack to the queue.
    /*if (received.type < FORC_POSITION && received.type != FORS_POSITION)
        sendAck();*/

    // TODO: Add the command to a queue; process the queue in the run() loop.

    m_buffer->clear();
    newData = false;
}