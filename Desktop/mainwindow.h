#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QSettings>
#include <QtSerialPort/QSerialPortInfo>
#include <QElapsedTimer>

#include "settingsdialog.h"
#include "serialpacketdefinition.h"

#include "systemkeyboardhook.h"

namespace Ui {
class MainWindow;
}

class SerialProtocol;
class QLabel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void openSerialPort();
    void closeSerialPort();
    void disableConnect();
    void enableConnect();
    void writePacket(command& packet);
    void receivePacket(const command& packet);

    int showConfiguration();

    void on_eProgress_valueChanged(int value);
    void handleKeyPressed(DWORD key);

private:
    void loadSettings();
    void toggleAutoconnect();

    void initConnections();

    void showStatusMessage(const QString& message);

    Ui::MainWindow* ui;
    QLabel* m_pStatus;

    SystemKeyboardHook* m_pSystemKeyboardHook;

    SettingsDialog* m_pSettingsDialog;
    QSettings m_settings;

    SerialProtocol* m_pSerial;
    uint16_t m_sliderPos;
};

#endif // MAINWINDOW_H
