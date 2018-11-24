#include "settingsdialog.h"
#include "ui_settingsdialog.h"

#include <QDebug>
#include <QPushButton>

static const char blankString[] = QT_TRANSLATE_NOOP("SettingsDialog", "N/A");

SettingsDialog::SettingsDialog(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);

    ui->buttonBox->button(QDialogButtonBox::Reset)->setText(" Reset current page ");

    connect(this, &SettingsDialog::accepted, this, &SettingsDialog::saveSettings);
    connect(ui->bCalibrate, &QPushButton::clicked, this, &SettingsDialog::requestCalibration);

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

void SettingsDialog::receiveCalibrationData(uint32_t minpos, uint32_t maxpos, uint32_t minvel, uint32_t maxvel)
{
    ui->eMinPos->setValue(minpos);
    ui->eMaxPos->setValue(maxpos);
    ui->eMinVel->setValue(minvel);
    ui->eMaxVel->setValue(maxvel);
}

void SettingsDialog::on_listCategories_currentRowChanged(int currentRow)
{
    ui->stackedWidget->setCurrentIndex(currentRow);
}

void SettingsDialog::saveSettings()
{
    // Save serial port configuration
    m_settings.setValue("sliderbar/autoconnect", ui->eAutoconnect->isChecked());

    // Save serial protocol configuration
    m_settings.setValue("sliderbar/protocol/startflag",
                        static_cast<uint8_t>(ui->eStartFlag->text().mid(2).toInt(nullptr, 16)));
    m_settings.setValue("sliderbar/protocol/endflag",
                        static_cast<uint8_t>(ui->eEndFlag->text().mid(2).toInt(nullptr, 16)));
    m_settings.setValue("sliderbar/protocol/escapeflag",
                        static_cast<uint8_t>(ui->eEscapeFlag->text().mid(2).toInt(nullptr, 16)));
    m_settings.setValue("sliderbar/protocol/xorflag",
                        static_cast<uint8_t>(ui->eXORFlag->text().mid(2).toInt(nullptr, 16)));
    m_settings.setValue("sliderbar/protocol/ackflag",
                        static_cast<uint8_t>(ui->eACKFlag->text().mid(2).toInt(nullptr, 16)));
    m_settings.setValue("sliderbar/protocol/nackflag",
                        static_cast<uint8_t>(ui->eNACKFlag->text().mid(2).toInt(nullptr, 16)));
    m_settings.setValue("sliderbar/protocol/acktimeout",
                        static_cast<uint8_t>(ui->eAckTimeOut->value()));

    // Save SliderBar calibration configuration
    m_settings.setValue("sliderbar/calibration/minpos",
                        static_cast<uint32_t>(ui->eMinPos->value()));
    m_settings.setValue("sliderbar/calibration/maxpos",
                        static_cast<uint32_t>(ui->eMaxPos->value()));
    m_settings.setValue("sliderbar/calibration/minvel",
                        static_cast<uint32_t>(ui->eMinVel->value()));
    m_settings.setValue("sliderbar/calibration/maxvel",
                        static_cast<uint32_t>(ui->eMaxVel->value()));
}

void SettingsDialog::loadSettings()
{
    // Load serial port configuration
    ui->eAutoconnect->setChecked(m_settings.value("sliderbar/autoconnect", false).toBool());

    // Load serial protocol configuration
    ui->eStartFlag->setValue(static_cast<uint8_t>(
        m_settings.value("sliderbar/protocol/startflag", 0x12).toInt()));
    ui->eEndFlag->setValue(static_cast<uint8_t>(
        m_settings.value("sliderbar/protocol/endflag", 0x13).toInt()));
    ui->eEscapeFlag->setValue(static_cast<uint8_t>(
        m_settings.value("sliderbar/protocol/escapeflag", 0x7D).toInt()));
    ui->eXORFlag->setValue(static_cast<uint8_t>(
        m_settings.value("sliderbar/protocol/xorflag", 0x20).toInt()));
    ui->eACKFlag->setValue(static_cast<uint8_t>(
        m_settings.value("sliderbar/protocol/ackflag", 0xFB).toInt()));
    ui->eNACKFlag->setValue(static_cast<uint8_t>(
        m_settings.value("sliderbar/protocol/nackflag", 0xFC).toInt()));
    ui->eAckTimeOut->setValue(static_cast<uint8_t>(
        m_settings.value("sliderbar/protocol/acktimeout", 50).toInt()));

    // Load SliderBar calibration configuration
    ui->eMinPos->setValue(m_settings.value("sliderbar/calibration/minpos", 0).toUInt());
    ui->eMaxPos->setValue(m_settings.value("sliderbar/calibration/maxpos", 0).toUInt());
    ui->eMinVel->setValue(m_settings.value("sliderbar/calibration/minvel", 0).toUInt());
    ui->eMaxVel->setValue(m_settings.value("sliderbar/calibration/maxvel", 0).toUInt());
}
