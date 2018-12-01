#include "settingsdialog.h"
#include "ui_settingsdialog.h"

#include <QMessageBox>
#include <QPushButton>

#include "sliderbarsettings.h"

#include <QDebug>

static const char blankString[] = QT_TRANSLATE_NOOP("SettingsDialog", "N/A");

SettingsDialog::SettingsDialog(SliderBarSettings* settings, QWidget* parent)
    : m_settings(settings)
    , QDialog(parent)
    , ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);

    connect(this, &SettingsDialog::accepted,
            this, &SettingsDialog::saveSettings);

    connect(ui->bRestoreDefaults, &QPushButton::clicked,
            this, &SettingsDialog::resetSettings);
    connect(ui->bCalibrate, &QPushButton::clicked,
            this, &SettingsDialog::requestCalibration);

    loadSettings();
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

int SettingsDialog::execute()
{
    loadSettings();
    return exec();
}

void SettingsDialog::receiveCalibrationData(const protocol::CalibrationData& data)
{
    ui->eMinPos->setValue(data.minimumPosition);
    ui->eMaxPos->setValue(data.maximumPosition);
    ui->eMinVel->setValue(data.minimumVelocity);
    ui->eMaxVel->setValue(data.maximumVeloicty);
}

void SettingsDialog::resetSettings()
{
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setWindowTitle("Restore default settings?");
    msgBox.setText("<b>Are you sure you want to restore default settings?</b>");
    msgBox.setInformativeText("All your current settings will be lost.");
    msgBox.setStandardButtons(QMessageBox::Cancel | QMessageBox::RestoreDefaults);
    msgBox.setDefaultButton(QMessageBox::Cancel);

    if (msgBox.exec() == QMessageBox::RestoreDefaults) {
        m_settings->clear();
        loadSettings();
    }
}

void SettingsDialog::on_listCategories_currentRowChanged(int currentRow)
{
    ui->stackedWidget->setCurrentIndex(currentRow);
}

void SettingsDialog::saveSettings()
{
    // Save serial port configuration
    m_settings->autoconnect(ui->eAutoconnect->isChecked());

    // Save serial protocol configuration
    m_settings->protocolStartFlag(ui->eStartFlag->text().mid(2).toInt(nullptr, 16));
    m_settings->protocolEndFlag(ui->eEndFlag->text().mid(2).toInt(nullptr, 16));
    m_settings->protocolEscapeFlag(ui->eEscapeFlag->text().mid(2).toInt(nullptr, 16));
    m_settings->protocolXORFlag(ui->eXORFlag->text().mid(2).toInt(nullptr, 16));
    m_settings->protocolACKTimeout(ui->eAckTimeOut->value());

    // Save SliderBar calibration configuration
    protocol::CalibrationData calibrationData;
    calibrationData.minimumPosition = ui->eMinPos->value();
    calibrationData.maximumPosition = ui->eMaxPos->value();
    calibrationData.minimumVelocity = ui->eMinVel->value();
    calibrationData.maximumVeloicty = ui->eMaxVel->value();
    m_settings->calibration(calibrationData);
}

void SettingsDialog::loadSettings()
{
    // Autoconnect
    ui->eAutoconnect->setChecked(m_settings->autoconnect());

    // Protocol configuration
    ui->eStartFlag->setValue(m_settings->protocolStartFlag());
    ui->eEndFlag->setValue(m_settings->protocolEndFlag());
    ui->eEscapeFlag->setValue(m_settings->protocolEscapeFlag());
    ui->eXORFlag->setValue(m_settings->protocolXORFlag());
    ui->eAckTimeOut->setValue(m_settings->protocolACKTimeout());

    // SliderBar calibration
    protocol::CalibrationData calibrationData = m_settings->calibration();

    ui->eMinPos->setValue(calibrationData.minimumPosition);
    ui->eMaxPos->setValue(calibrationData.maximumPosition);
    ui->eMinVel->setValue(calibrationData.minimumVelocity);
    ui->eMaxVel->setValue(calibrationData.maximumVeloicty);
}
