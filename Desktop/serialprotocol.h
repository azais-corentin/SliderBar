#ifndef SERIALPROTOCOL_H
#define SERIALPROTOCOL_H

#include <QObject>
#include <QByteArray>
#include <QSerialPort>
#include <QSettings>

#include "serialpacketdefinition.h"

class SerialProtocol : public QObject
{
    Q_OBJECT

public:
    SerialProtocol();
    ~SerialProtocol();

    void initConnections();
    void loadSettings();

    bool openSerialPort();
    void closeSerialPort();

    void writePacket(const command& packet);

    QSerialPort* serial;

signals:
    void packetReady(const command& packet);
    void statusMessage(const QString& message);
    void disconnected();

private slots:
    void readData(); //read data and emits packetReady() once a packet is totally received
    void handleError(QSerialPort::SerialPortError error);

private:
    uchar startflag, endflag, escapeflag, xorflag;

    QSettings m_settings;
    QByteArray m_buffer;
};

#endif // SERIALPROTOCOL_H
