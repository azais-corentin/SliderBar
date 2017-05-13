#include "SerialBus.h"

uint8_t startflag = 0x12;
uint8_t endflag = 0x13;
uint8_t escapeflag = 0x7D;
uint8_t xorflag = 0x20;

SerialBus::SerialBus(HardwareSerial &serial) : m_serial(serial) {}

SerialBus::~SerialBus() {}

bool SerialBus::begin(unsigned long baud) {
    m_serial.begin(baud);
    return true;
}

void SerialBus::send_command(command &cmd) const {
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

void SerialBus::receive() {
    if (!m_serial.available() > 0)
        return;

    if (m_buffer.size() >= MAX_BUFFER_SIZE)
        m_buffer.clear();

    // Read available data
    while (m_serial.available() > 0)
        m_buffer.append(m_serial.read());

    // Makes sure buffer contains the startflag
    if (!m_buffer.contains(startflag)) {
        m_buffer.clear();
        return;
    }

    // Makes sure buffer starts with the startflag
    m_buffer = m_buffer.mid(m_buffer.lastIndexOf(startflag));

    // Wait for the endflag
    if (!m_buffer.contains(endflag))
        return;

    // Makes sure buffer ends with the first endflag
    m_buffer = m_buffer.left(m_buffer.indexOf(endflag) + 1);

    Buffer data = m_buffer.mid(1);
    data.chop(1);

    command received;
    received.type = static_cast<command::command_type>(data.at8(0));
    // received.value = data.at8(1);
}

void SerialBus::encode8(Buffer &packet, uint8_t data, bool escape) {
    if (escape && command::isFlag(data)) {
        packet.append(escapeflag);
        packet.append(data ^ xorflag);
    } else
        packet.append(data);
}

void SerialBus::encode16(Buffer &packet, uint16_t data, bool escape) {
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
