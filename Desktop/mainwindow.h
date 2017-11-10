#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDir>
#include <QSignalMapper>
#include <QSettings>
#include <QtSerialPort/QSerialPortInfo>

#include "settingsdialog.h"
#include "sliderinterface.h"
#include "serialpacketdefinition.h"

#include "systemkeyboardhook.h"

namespace Ui {
    class MainWindow;
}

class SerialProtocol;
class QLabel;
class QTimer;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

signals:
    void eventDispatch(SliderEventType type, QVariant value);

private slots:
    void openSerialPort();
    void closeSerialPort();
    void disableConnect();
    void enableConnect();
    void receivePacket(const command& packet);
    void clearStatusMessage();

    int showConfiguration();

    void on_eProgress_valueChanged(int value);
    void handleKeyPressed(DWORD key);

    void on_bAddResist_clicked();
    void on_bClearResists_clicked();
    void on_bDisablePID_clicked();
    void on_bEnablePID_clicked();
    void on_bUpdatePID_clicked();

    void selectPlugin(int i);
    void loadPlugins();

private:
    void loadSettings();
    void toggleAutoconnect();

    void initConnections();

    void showStatusMessage(const QString& message);

    Ui::MainWindow* ui = nullptr;
    QLabel* m_pStatus = nullptr;
    QTimer* m_pTimerStatusMessage = nullptr;

    QDir m_PluginsDir;
    QStringList m_pluginFileNames;
    QList<SliderInterface*> m_pluginList;
    int m_currentPluginIndex;
    QSignalMapper* m_pSignalMapper = nullptr;

    SystemKeyboardHook* m_pSystemKeyboardHook = nullptr;

    SettingsDialog* m_pSettingsDialog = nullptr;
    QSettings m_settings;

    SerialProtocol* m_pSerial = nullptr;
    double m_sliderPos = 0;
    uint32_t m_count = 0;

    float m_sliderLastPosition = 0.f;
};

#endif // MAINWINDOW_H
