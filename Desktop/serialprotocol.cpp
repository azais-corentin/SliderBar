#include "serialprotocol.h"

#include <QString>
#include <QMessageBox>
#include <QDebug>

#include "serialpacketdefinition.h"

#define MAX_PACKET_SIZE 1024

SerialProtocol::SerialProtocol()
{
    serial = new QSerialPort;
    m_buffer.clear();

    loadSettings();
    initConnections();
}

SerialProtocol::~SerialProtocol()
{
    delete serial;
}

void SerialProtocol::initConnections()
{
    connect(serial, &QSerialPort::errorOccurred,
        this, &SerialProtocol::handleError);

    connect(serial, &QSerialPort::readyRead, this, &SerialProtocol::readData);
}

void SerialProtocol::loadSettings()
{
    startflag = static_cast<uchar>(m_settings.value("serial/protocol/startflag", 0x12).toInt());
    endflag = static_cast<uchar>(m_settings.value("serial/protocol/endflag", 0x13).toInt());
    escapeflag = static_cast<uchar>(m_settings.value("serial/protocol/escapeflag", 0x7D).toInt());
    xorflag = static_cast<uchar>(m_settings.value("serial/protocol/xorflag", 0x20).toInt());
    datasep = static_cast<uchar>(m_settings.value("serial/protocol/datasep", 0x1D).toInt());

    serial->setPortName(m_settings.value("serial/portname", "COM1").toString());
    serial->setBaudRate(static_cast<QSerialPort::BaudRate>(
            m_settings.value("serial/baudrate", QSerialPort::Baud115200).toInt()),
        QSerialPort::AllDirections);
    serial->setDataBits(static_cast<QSerialPort::DataBits>(
            m_settings.value("serial/databits", QSerialPort::Data8).toInt()));
    serial->setParity(static_cast<QSerialPort::Parity>(
            m_settings.value("serial/parity", QSerialPort::NoParity).toInt()));
    serial->setStopBits(static_cast<QSerialPort::StopBits>(
            m_settings.value("serial/stopbits", QSerialPort::OneStop).toInt()));
    serial->setFlowControl(static_cast<QSerialPort::FlowControl>(
            m_settings.value("serial/flowcontrol", QSerialPort::HardwareControl).toInt()));
}

bool SerialProtocol::openSerialPort()
{
    loadSettings();
    if (serial->isOpen())
    {
        emit statusMessage(QStringLiteral("Serial port: Already connected!"));
        return true;
    }
    if (serial->open(QIODevice::ReadWrite))
    {
        serial->clear(QSerialPort::AllDirections);
        emit statusMessage(QStringLiteral("Serial port: Connected to %1").arg(serial->portName()));
        return true;
    }
    else
    {
        emit statusMessage(QStringLiteral("Serial port error: %1").arg(serial->errorString()));
    }
    return false;
}

void SerialProtocol::closeSerialPort()
{
    if (!serial->isOpen())
        return;
    serial->close();
    emit statusMessage(QStringLiteral("Disconnected!"));
}

void SerialProtocol::writePacket(const command& packet)
{
    /// Packet structure:
    /// http://eli.thegreenplace.net/2009/08/12/framing-in-serial-communications/
    /// [0]         startflag
    /// [1,2]       x (length)
    /// [3,..,x-1]  data
    /// [x]         endflag
    QByteArray packetBytes;
    packetBytes.append(static_cast<uint8_t>(packet.type));

    QByteArray packetData;
    quint16 length = static_cast<quint16>(1 + 2 + packetBytes.size()
            + packetBytes.count(static_cast<char>(startflag))
            + packetBytes.count(static_cast<char>(endflag))
            + packetBytes.count(static_cast<char>(escapeflag)) + 1);
    packetData.resize(length);
    int i = 0, di = 0;

    // Add startflag
    //encode(packet, i, data);
    packetData[i++] = static_cast<char>(startflag);

    // Add length
    packetData[i++] = static_cast<char>(length >> 8);
    packetData[i++] = static_cast<char>(length);
    //packet.replace(i, 2, QString::number(length, 16).toLatin1());

    // Add data
    while (di < packetBytes.size())
    {
        if (packetBytes.at(di) == startflag || packetBytes.at(di) == endflag || packetBytes.at(di) == escapeflag)
        {
            packetData[i++] = static_cast<char>(escapeflag);
            packetData[i++] = static_cast<char>(uData[di] ^ xorflag);
        }
        else
            packetData[i++] = packetBytes.at(di);
        di++;
    }

    // Add endflag
    packetData[i] = static_cast<char>(endflag);

    // Send data
    serial->write(packetData);
    foreach (char ch, packetData)
    {
        qDebug() << QString::number(static_cast<uchar>(ch), 16);
    }
}

void SerialProtocol::readData()
{
    if (m_buffer.size() > MAX_PACKET_SIZE)
    {
        m_buffer.clear();
        qDebug() << "Buffer is full! Clearing.";
    }
    m_buffer.append(serial->readAll());

    if (!m_buffer.contains(static_cast<char>(startflag)))
    {
        m_buffer.clear();
        return;
    }
    m_buffer = m_buffer.mid(m_buffer.indexOf(static_cast<char>(startflag)));

    if (!m_buffer.contains(static_cast<char>(endflag)))
        return;

    m_buffer = m_buffer.left(m_buffer.indexOf(static_cast<char>(endflag)) + 1);
    int i = 1;
    quint16 length = decode(m_buffer, i);
    //quint16 length2 = static_cast<quint16>(m_buffer.at(1) << 8 | m_buffer.at(2));
    if (m_buffer.size() != length)
    {
        qDebug() << "Size mismatch!";
        m_buffer.clear();
        return;
    }

    // Decode packet data
    QByteArray data = m_buffer.mid(3, m_buffer.size() - 4);
    command received;

    i = 0;
    decode(data, i);
    received.type = static_cast<command::command_type>(static_cast<uint8_t>(decode(data, i)));
    received.value = decode(data, i);

    emit packetReady(received);
    m_buffer.clear();
}

void SerialProtocol::handleError(QSerialPort::SerialPortError error)
{
    if (!serial->isOpen())
        emit disconnected();
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
            emit statusMessage(QStringLiteral("Serial port error: %1").arg(serial->errorString()));
            break;
    }
}
