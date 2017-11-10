#include "SerialBus.h"

uint8_t startflag = 0x12;
uint8_t endflag = 0x13;
uint8_t escapeflag = 0x7D;
uint8_t xorflag = 0x20;
uint8_t ackflag = 0xFB;

SerialBus::SerialBus(HardwareSerial &serial) : m_serial(serial) {}

SerialBus::~SerialBus() {}

bool SerialBus::begin(unsigned long baud) {
    m_serial.begin(baud);
    m_buffer.clear();
    return true;
}

void SerialBus::receivePacket() {
    if (!(m_serial.available() > 0))
        return;

    if (m_buffer.size() >= MAX_BUFFER_SIZE)
        m_buffer.clear();

    // Read available data
    while (m_serial.available())
        m_buffer.append8(m_serial.read());

    // Makes sure buffer contains the startflag
    if (!m_buffer.contains(startflag)) {
        m_buffer.clear();
        return;
    }

    // Makes sure buffer starts with the last startflag
    m_buffer = m_buffer.mid(m_buffer.lastIndexOf(startflag));

    // Wait for the endflag
    if (!m_buffer.contains(endflag))
        return;

    // Makes sure buffer ends with the first endflag
    m_buffer = m_buffer.left(m_buffer.indexOf(endflag) + 1);

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
    uint8_t crc_computed =
        CRC::compute(receivedBytes.data(), receivedBytes.size());

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
    if (received.type < command::FORC_POSITION &&
        received.type != command::FORS_POSITION)
        sendAck();

    //Process the received packet
    m_receiver(received);

    m_buffer.clear();
}

void SerialBus::sendPacket(const command &cmd) const {
    /// http://eli.thegreenplace.net/2009/08/12/framing-in-serial-communications/
    /// Packet structure / protocol framing:
    /// name:       byte(s)     escaped
    /// -------------------------------
    /// startflag:  1           false
    /// data:       3 to 6      true
    /// CRC:        1 to 2      true
    /// endflag:    1           false

    // Compute CRC on unescaped packet bytes
    Buffer packetBytes;
    encode8(packetBytes, static_cast<uint8_t>(cmd.type), false);
    encode16(packetBytes, cmd.value, false);
    uint8_t crc = CRC::compute(packetBytes.data(), 3);

    Buffer packet;
    // Add startflag
    encode8(packet, startflag, false);
    // Add data
    encode8(packet, static_cast<uint8_t>(cmd.type));
    encode16(packet, cmd.value);
    // Add crc
    encode8(packet, crc);
    // Add endflag
    encode8(packet, endflag, false);

    // Send data
    m_serial.write(packet.data(), packet.size());
}

void SerialBus::sendAck() const {
    Buffer ack;
    encode8(ack, startflag, false);
    encode8(ack, ackflag, false);
    encode8(ack, endflag, false);
    m_serial.write(ack.data(), ack.size());
}

void SerialBus::encode8(Buffer &packet, const uint8_t data, bool escape) const {
    if (escape && command::isFlag(data)) {
        packet.append8(escapeflag);
        packet.append8(data ^ xorflag);
    } else
        packet.append8(data);
}

void SerialBus::encode16(Buffer &packet, const uint16_t data,
                         bool escape) const {
    encode8(packet, static_cast<uint8_t>(data >> 8), escape);
    encode8(packet, static_cast<uint8_t>(data), escape);
}

uint8_t SerialBus::decode8(const Buffer &packet, int &i) {
    if (command::isFlag(packet.at8(i++)))
        return packet.at8(i++) ^ xorflag;
    return packet.at8(i - 1);
}

uint16_t SerialBus::decode16(const Buffer &packet, int &i) {
    uint8_t b1, b2;
    b1 = decode8(packet, i);
    b2 = decode8(packet, i);

    return static_cast<uint16_t>((b1 << 8) | (b2 & 0xff));
}
