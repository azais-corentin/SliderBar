#ifndef SERIALPROTOCOL_H
#define SERIALPROTOCOL_H

#include <QObject>
#include <QByteArray>
#include <QSerialPort>
#include <QSettings>
#include <QQueue>

#include "serialpacketdefinition.h"

class QTimer;

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

signals:
    void packetReady(const command& packet);
    void statusMessage(const QString& message);
    void serialDisconnected();
    void serialConnected();

private slots:
    void readData(); //read data and emits packetReady() once a packet is totally received
    void handleError(QSerialPort::SerialPortError error);
    void handleAckTimeout();

private:
    QSerialPort* m_serial;
    uint8_t startflag, endflag, escapeflag, xorflag, ackflag;
    QSettings m_settings;

    QQueue<command> m_remainingAck;
    QTimer* m_pTimerAck;

    // Encoding, decoding
    QByteArray m_buffer;
    bool isFlag(uint8_t ch) { return (ch == startflag) | (ch == endflag) | (ch == escapeflag); }
    uint8_t decode8(const QByteArray& packet, int& i);
    uint16_t decode16(const QByteArray& packet, int& i);
    void encode8(QByteArray& packet, int& i, const uint8_t& ch, bool escape = true);
    void encode16(QByteArray& packet, int& i, const uint16_t& ch, bool escape = true);
};

#endif // SERIALPROTOCOL_H
