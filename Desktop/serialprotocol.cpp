#include "serialprotocol.h"

#include <QString>
#include <QMessageBox>
#include <QDebug>

#include "serialpacketdefinition.h"

#define MAX_PACKET_SIZE 10

SerialProtocol::SerialProtocol()
{
    m_serial = new QSerialPort(this);
    m_buffer.clear();

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
}

void SerialProtocol::loadSettings()
{
    startflag = static_cast<uint8_t>(m_settings.value("serial/protocol/startflag", 0x12).toInt());
    endflag = static_cast<uint8_t>(m_settings.value("serial/protocol/endflag", 0x13).toInt());
    escapeflag = static_cast<uint8_t>(m_settings.value("serial/protocol/escapeflag", 0x7D).toInt());
    xorflag = static_cast<uint8_t>(m_settings.value("serial/protocol/xorflag", 0x20).toInt());

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
}

void SerialProtocol::readData()
{
    if (m_buffer.size() >= MAX_PACKET_SIZE)
    {
        m_buffer.clear();
        qDebug() << "Buffer is full! Clearing.";
    }
    m_buffer.append(m_serial->readAll());

    qDebug() << m_buffer;

    if (!m_buffer.contains(static_cast<char>(startflag)))
    {
        m_buffer.clear();
        return;
    }

    // Makes sure buffer starts with the last startflag
    m_buffer = m_buffer.mid(m_buffer.lastIndexOf(static_cast<char>(startflag)));

    // Wait for the endflag
    if (!m_buffer.contains(static_cast<char>(endflag)))
        return;

    // Makes sure buffer ends with the first endflag
    m_buffer = m_buffer.left(m_buffer.indexOf(static_cast<char>(endflag)) + 1);

    // Packets is complete
    QByteArray data = m_buffer.mid(1);
    data.chop(1);
    int i = 0;
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


    // Emit received data
    command received;
    received.type = static_cast<command::command_type>(type);
    received.value = value;

    emit packetReady(received);
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

uint8_t SerialProtocol::decode8(const QByteArray& packet, int& i)
{
    if (isFlag(static_cast<uint8_t>(packet.at(i++))))
        return static_cast<uint8_t>(packet.at(i++)) ^ xorflag;
    return static_cast<uint8_t>(packet.at(i - 1));
}

uint16_t SerialProtocol::decode16(const QByteArray& packet, int& i)
{
    uint8_t b1, b2;
    b1 = decode8(packet, i);
    b2 = decode8(packet, i);

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
