#ifndef SERIALSETTINGS_H
#define SERIALSETTINGS_H

#include <QDialog>
#include <QtSerialPort/QSerialPort>

QT_USE_NAMESPACE

QT_BEGIN_NAMESPACE

namespace Ui {
class SerialSettings;
}

class QIntValidator;

QT_END_NAMESPACE

class SerialSettings : public QDialog
{
    Q_OBJECT

public:
    struct Settings
    {
        QString name;
        qint32 baudRate;
        QString stringBaudRate;
        QSerialPort::DataBits dataBits;
        QString stringDataBits;
        QSerialPort::Parity parity;
        QString stringParity;
        QSerialPort::StopBits stopBits;
        QString stringStopBits;
        QSerialPort::FlowControl flowControl;
        QString stringFlowControl;
        bool localEchoEnabled;
    };

    explicit SerialSettings(QWidget* parent = nullptr);
    ~SerialSettings();

    Settings settings() const;

private slots:
    void showPortInfo(int idx);
    void apply();
    void checkCustomBaudRatePolicy(int idx);
    void checkCustomDevicePathPolicy(int idx);

private:
    void fillPortsParameters();
    void fillPortsInfo();
    void updateSettings();

private:
    Ui::SerialSettings* ui;
    Settings currentSettings;
    QIntValidator* intValidator;
};

#endif // SERIALSETTINGS_H
