#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QSettings>

#include "protocol/protocol_structures.h"

QT_USE_NAMESPACE

QT_BEGIN_NAMESPACE

class QAbstractButton;
class SliderBarSettings;

namespace Ui {
class SettingsDialog;
}

QT_END_NAMESPACE

class SettingsDialog : public QDialog {
    Q_OBJECT

public:
    explicit SettingsDialog(SliderBarSettings* settings, QWidget* parent = nullptr);
    ~SettingsDialog() override;

public slots:
    int execute();
    void receiveCalibrationData(const protocol::CalibrationData& data);

    void resetSettings();

signals:
    void requestCalibration();

private slots:
    void on_listCategories_currentRowChanged(int currentRow);

private:
    void saveSettings();
    void loadSettings();

    Ui::SettingsDialog* ui;
    SliderBarSettings* m_settings;
};

#endif // SETTINGSDIALOG_H
