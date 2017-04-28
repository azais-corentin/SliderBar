#include "serialprotocol.h"

#include <QString>
#include <QMessageBox>
#include <QDebug>

#define MAX_PACKET_SIZE 1024

SerialProtocol::SerialProtocol()
{
    serial = new QSerialPort;

    loadSettings();

    m_buffer.clear();

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
        emit statusMessage(QStringLiteral("Serial port: Connected to %1").arg(serial->portName()));
        return true;
    }
    else
    {
        emit statusMessage(QStringLiteral("Serial port: Error %1").arg(serial->errorString()));
    }
    return false;
}

void SerialProtocol::closeSerialPort()
{
    if (serial->isOpen())
        serial->close();
    emit statusMessage(QStringLiteral("Disconnected!"));
}

void SerialProtocol::writeData(const QByteArray& data)
{
    const uchar* uData = convertByteArray(data);
    /// Packet structure:
    /// http://eli.thegreenplace.net/2009/08/12/framing-in-serial-communications/
    /// [0]         startflag
    /// [1,2]       x (length)
    /// [3,..,x-1]  data
    /// [x]         endflag
    QByteArray packet;
    quint16 length = static_cast<quint16>(1 + 2 + data.size() + data.count(static_cast<char>(startflag)) +
            data.count(static_cast<char>(endflag)) + data.count(static_cast<char>(escapeflag)) + 1);
    packet.resize(length);
    int i = 0, di = 0;

    // Add startflag
    packet[i++] = static_cast<char>(startflag);

    // Add length
    packet[i++] = static_cast<char>(length >> 8);
    packet[i++] = static_cast<char>(length);
    //packet.replace(i, 2, QString::number(length, 16).toLatin1());

    // Add data
    while (di < data.size())
    {
        if (data.at(di) == startflag || data.at(di) == endflag || data.at(di) == escapeflag)
        {
            packet[i++] = static_cast<char>(escapeflag);
            packet[i++] = static_cast<char>(uData[di] ^ xorflag);
        }
        else
            packet[i++] = data.at(di);
        di++;
    }

    // Add endflag
    packet[i] = static_cast<char>(endflag);
    foreach (char ch, packet)
    {
        qDebug() << QString::number(static_cast<uchar>(ch), 16);
    }
}

void SerialProtocol::writeData(const QVector<uchar>& data)
{
    QByteArray convertedData(reinterpret_cast<const char*>(data.data()), data.size());
    writeData(convertedData);
}

const uchar* SerialProtocol::convertByteArray(const QByteArray& data)
{
    return reinterpret_cast<const uchar*>(data.constData());
}

void SerialProtocol::readData()
{
    if (m_buffer.size() > MAX_PACKET_SIZE)
    {
        m_buffer.clear();
        qDebug() << "Buffer is full! Clearing.";
        failedPackets++;
    }
    m_buffer.append(serial->readAll());

    /*if (m_buffer.contains(0x12))
    {
        bool finished = true;
        QList<QByteArray> list;
        int i = -1;
        foreach (char ch, m_buffer)
        {
            switch (static_cast<uchar>(ch))
            {
                case 0x12:
                    i++;
                    finished = false;
                    list.append(QByteArray(1, 0x12));
                    break;
                case 0x13:
                    finished = true;
                    list[i].append(0x13);
                    break;
                default:
                    if (!finished)
                        list[i].append(ch);
                    break;
            }
        }
        qDebug() << list;
    }*/

    if (!m_buffer.contains(static_cast<char>(startflag)))
    {
        failedPackets++;
        m_buffer.clear();
        return;
    }
    m_buffer = m_buffer.mid(m_buffer.indexOf(static_cast<char>(startflag)));

    if (!m_buffer.contains(0x13))
        return;

    qDebug() << "Full packet received!";
    totalPackets++;

    m_buffer = m_buffer.left(m_buffer.indexOf(0x13) + 1);

    quint16 length = static_cast<quint16>(m_buffer.at(1) << 8 | m_buffer.at(2));
    if (m_buffer.size() != length)
    {
        qDebug() << "Size mismatch!";
        m_buffer.clear();
        failedPackets++;
        return;
    }

    QByteArray data = m_buffer.mid(3, m_buffer.size() - 4);

    qDebug() << "Valid packed received!";

    // Decode packet data
    qDebug() << data;
    emit packetReady(data);
    m_buffer.clear();
    statusMessage(QStringLiteral("Failure percentage: %1")
        .arg(100. * (double) failedPackets / (double) totalPackets));
}

void SerialProtocol::handleError()
{
    if (!serial->isOpen())
        emit disconnected();
    switch (serial->error())
    {
        case QSerialPort::NoError:
            return;
        case QSerialPort::ResourceError:
            emit statusMessage(QStringLiteral("Serial port error: Device was unexpectedly removed."));
            break;
        default:
            emit statusMessage(QStringLiteral("Serial port error: %1").arg(serial->errorString()));
            break;
    }
}
