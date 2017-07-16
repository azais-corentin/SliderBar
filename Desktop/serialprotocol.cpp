#include "serialprotocol.h"

#include <QDebug>

#include <QString>
#include <QTimer>

#include "serialpacketdefinition.h"

#define MAX_PACKET_SIZE 10

SerialProtocol::SerialProtocol()
{
    m_serial = new QSerialPort(this);
    m_buffer.clear();

    m_pTimerAck = new QTimer(this);
    m_pTimerAck->setSingleShot(true);

    loadSettings();
    initConnections();
}

SerialProtocol::~SerialProtocol()
{
    delete m_serial;
}

void SerialProtocol::initConnections()
{
    connect(m_serial, static_cast<void (QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error),
        this, &SerialProtocol::handleError);
    connect(m_serial, &QSerialPort::readyRead, this, &SerialProtocol::readData);

    connect(m_pTimerAck, &QTimer::timeout, this, &SerialProtocol::handleAckTimeout);
}

void SerialProtocol::loadSettings()
{
    startflag = static_cast<uint8_t>(m_settings.value("serial/protocol/startflag", 0x12).toInt());
    endflag = static_cast<uint8_t>(m_settings.value("serial/protocol/endflag", 0x13).toInt());
    escapeflag = static_cast<uint8_t>(m_settings.value("serial/protocol/escapeflag", 0x7D).toInt());
    xorflag = static_cast<uint8_t>(m_settings.value("serial/protocol/xorflag", 0x20).toInt());
    ackflag = static_cast<uint8_t>(m_settings.value("serial/protocol/ackflag", 0xFB).toInt());

    m_serial->setPortName(m_settings.value("serial/portname", "COM1").toString());
    m_serial->setBaudRate(static_cast<QSerialPort::BaudRate>(
            m_settings.value("serial/baudrate", QSerialPort::Baud115200).toInt()),
        QSerialPort::AllDirections);
    m_serial->setDataBits(static_cast<QSerialPort::DataBits>(
            m_settings.value("serial/databits", QSerialPort::Data8).toInt()));
    m_serial->setParity(static_cast<QSerialPort::Parity>(
            m_settings.value("serial/parity", QSerialPort::NoParity).toInt()));
    m_serial->setStopBits(static_cast<QSerialPort::StopBits>(
            m_settings.value("serial/stopbits", QSerialPort::OneStop).toInt()));
    m_serial->setFlowControl(static_cast<QSerialPort::FlowControl>(
            m_settings.value("serial/flowcontrol", QSerialPort::HardwareControl).toInt()));
    m_pTimerAck->setInterval(m_settings.value("serial/protocol/acktimeout", 15).toInt());
}

bool SerialProtocol::openSerialPort()
{
    loadSettings();
    if (m_serial->isOpen())
    {
        emit statusMessage(QStringLiteral("Serial port: Already connected!"));
        emit serialConnected();
        return true;
    }
    if (m_serial->open(QIODevice::ReadWrite))
    {
        m_serial->clear(QSerialPort::AllDirections);
        emit statusMessage(QStringLiteral("Serial port: Connected to %1").arg(m_serial->portName()));
        emit serialConnected();
        return true;
    }
    else
    {
        emit statusMessage(QStringLiteral("Serial port error: %1").arg(m_serial->errorString()));
    }
    emit serialDisconnected();
    return false;
}

void SerialProtocol::closeSerialPort()
{
    if (!m_serial->isOpen())
        return;
    m_serial->flush();
    m_serial->close();
    emit serialDisconnected();
    emit statusMessage(QStringLiteral("Serial port: Disconnected!"));
}

void SerialProtocol::writePacket(const command& packet)
{
    /// http://eli.thegreenplace.net/2009/08/12/framing-in-serial-communications/
    /// Packet structure / protocol framing:
    /// name:       byte(s)     escaped
    /// -------------------------------
    /// startflag:  1           false
    /// data:       3 to 6      true
    /// CRC:        1 to 2      true
    /// endflag:    1           false

    if (!m_serial->isOpen())
        openSerialPort();

    // Compute CRC on unescaped packet bytes
    QByteArray packetBytes;
    packetBytes.append(static_cast<char>(static_cast<uint8_t>(packet.type)));
    packetBytes.append(static_cast<char>(static_cast<uint8_t>(packet.value >> 8)));
    packetBytes.append(static_cast<char>(static_cast<uint8_t>(packet.value)));
    uint8_t crc = CRC::compute(packetBytes, 3);

    QByteArray packetData;
    int i = 0;

    // Add startflag
    encode8(packetData, i, startflag, false);
    // Add data
    encode8(packetData, i, static_cast<uint8_t>(packet.type));
    encode16(packetData, i, packet.value);
    // Add CRC
    encode8(packetData, i, crc);
    // Add endflag
    encode8(packetData, i, endflag, false);

    // Send data
    m_serial->write(packetData);

    // If we need an Ack
    if (packet.type < command::FORC_POSITION &&
        packet.type != command::FORS_POSITION)
    {
        m_remainingAck.enqueue(packet);
        if (!m_pTimerAck->isActive())
            m_pTimerAck->start();
    }
}

void SerialProtocol::readData()
{
    if (m_buffer.size() >= MAX_PACKET_SIZE)
    {
        m_buffer.clear();
        qDebug() << "Buffer is full! Clearing.";
    }
    m_buffer.append(m_serial->readAll());

    if (!m_buffer.contains(static_cast<char>(startflag)))
    {
        m_buffer.clear();
        return;
    }

    // Makes sure buffer starts with the first startflag
    m_buffer = m_buffer.mid(m_buffer.indexOf(static_cast<char>(startflag)));

    // Wait for the endflag
    if (!m_buffer.contains(static_cast<char>(endflag)))
        return;

    // Makes sure buffer ends with the last endflag
    m_buffer = m_buffer.left(m_buffer.lastIndexOf(static_cast<char>(endflag)) + 1);

    // Process packet(s)
    m_buffer.chop(1);
    auto m_bufferpackets = m_buffer.split(static_cast<char>(endflag));

    for (auto data : m_bufferpackets)
    {
        data = data.mid(1);
        int i = 0;

        // Check if packet is ACK
        if (data.size() == 1)
        {
            uint8_t value = decode8(data, i, false);
            if (value == ackflag)
            {
                m_remainingAck.dequeue();
                if (m_remainingAck.isEmpty())
                    m_pTimerAck->stop();
                else
                    m_pTimerAck->start();

                m_buffer.clear();
            }
            continue;
        }

        QByteArray receivedBytes;

        // Extracts data & CRC
        uint8_t type = decode8(data, i);
        uint16_t value = decode16(data, i);
        uint8_t crc_received = decode8(data, i);

        // Computes CRC
        i = 0;
        encode8(receivedBytes, i, type, false);
        encode16(receivedBytes, i, value, false);
        uint8_t crc_computed = CRC::compute(receivedBytes, 3);

        // Verify CRC
        if (crc_computed != crc_received)
        {
            qDebug() << "CRC Mismatch !";
            m_buffer.clear();
            return;
        }

        // Process received data
        command received;
        received.type = static_cast<command::command_type>(type);
        received.value = value;

        emit packetReady(received);

    }

    m_buffer.clear();
}

void SerialProtocol::handleError(QSerialPort::SerialPortError error)
{
    if (!m_serial->isOpen())
        emit serialDisconnected();
    switch (error)
    {
        case QSerialPort::NoError:
            return;
        case QSerialPort::WriteError:
        case QSerialPort::ReadError:
        case QSerialPort::ResourceError:
            emit statusMessage(QStringLiteral("Serial port error: Device was unexpectedly removed."));
            break;
        default:
            emit statusMessage(QStringLiteral("Serial port error: %1").arg(m_serial->errorString()));
            break;
    }
}

void SerialProtocol::handleAckTimeout()
{
    statusMessage("Serial error: Ack missing!");
    writePacket(m_remainingAck.head());
    m_remainingAck.dequeue();
}

uint8_t SerialProtocol::decode8(const QByteArray& packet, int& i, bool escape)
{
    if (escape)
    {
        if (isFlag(static_cast<uint8_t>(packet.at(i++))))
            return static_cast<uint8_t>(packet.at(i++)) ^ xorflag;
        return static_cast<uint8_t>(packet.at(i - 1));
    }
    return static_cast<uint8_t>(packet.at(i++));
}

uint16_t SerialProtocol::decode16(const QByteArray& packet, int& i, bool escape)
{
    uint8_t b1, b2;
    b1 = decode8(packet, i, escape);
    b2 = decode8(packet, i, escape);

    return static_cast<uint16_t>((b1 << 8) | (b2 & 0xff));
}

void SerialProtocol::encode8(QByteArray& packet, int& i, const uint8_t& ch, bool escape)
{
    if (escape && isFlag(ch))
    {
        packet[i++] = static_cast<char>(escapeflag);
        packet[i++] = static_cast<char>(ch ^ xorflag);
        return;
    }
    packet[i++] = static_cast<char>(ch);
}

void SerialProtocol::encode16(QByteArray& packet, int& i, const uint16_t& ch, bool escape)
{
    encode8(packet, i, static_cast<uint8_t>(ch >> 8), escape);
    encode8(packet, i, static_cast<uint8_t>(ch), escape);
}
