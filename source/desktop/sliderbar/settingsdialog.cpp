#include "settingsdialog.h"
#include "ui_settingsdialog.h"

#include <QPushButton>

static const char blankString[] = QT_TRANSLATE_NOOP("SettingsDialog", "N/A");

SettingsDialog::SettingsDialog(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);

    ui->buttonBox->button(QDialogButtonBox::Reset)->setText("Reset current page");

    connect(this, &SettingsDialog::accepted, this, &SettingsDialog::saveSettings);

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
}

void SettingsDialog::loadSettings()
{
    // Loads serial port configuration
    ui->eAutoconnect->setChecked(m_settings.value("sliderbar/autoconnect", false).toBool());

    // Loads serial protocol information
    ui->eStartFlag->setValue(static_cast<uint8_t>(
        m_settings.value("sliderbar/protocol/startflag",
                         0x12)
            .toInt()));
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
}
