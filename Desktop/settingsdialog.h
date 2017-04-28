#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QSettings>
#include <QDialog>
#include <QtSerialPort/QSerialPort>

QT_USE_NAMESPACE

QT_BEGIN_NAMESPACE

namespace Ui {
class SettingsDialog;
}

class QIntValidator;

QT_END_NAMESPACE

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget* parent = 0);
    ~SettingsDialog();

public slots:
    int execute();

private slots:
    void showPortInfo(int idx);
    void checkCustomBaudRatePolicy(int idx);
    void checkCustomDevicePathPolicy(int idx);
    void on_listCategories_currentRowChanged(int currentRow);

private:

    void fillPortsParameters();
    void fillPortsInfo();

    void saveSettings();
    void loadSettings();

    Ui::SettingsDialog* ui;
    QSettings m_Settings;
    QIntValidator* m_pIntValidator;
};

#endif // SETTINGSDIALOG_H
