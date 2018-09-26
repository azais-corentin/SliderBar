#include "settingsdialog.h"
#include "ui_settingsdialog.h"

#include <QDebug>
#include <QtSerialPort/QSerialPortInfo>
#include <QIntValidator>
#include <QLineEdit>

static const char blankString[] = QT_TRANSLATE_NOOP("SettingsDialog", "N/A");

SettingsDialog::SettingsDialog(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);

    m_pIntValidator = new QIntValidator(0, 4000000, this);
    ui->eBaudrate->setInsertPolicy(QComboBox::NoInsert);

    connect(this, &SettingsDialog::accepted, this, &SettingsDialog::saveSettings);
    connect(ui->eSerialPortInfoList, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
        this, &SettingsDialog::showPortInfo);
    connect(ui->eBaudrate,  static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
        this, &SettingsDialog::checkCustomBaudRatePolicy);
    connect(ui->eSerialPortInfoList,  static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
        this, &SettingsDialog::checkCustomDevicePathPolicy);

    fillPortsParameters();
    fillPortsInfo();
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

void SettingsDialog::showPortInfo(int idx)
{
    if (idx == -1)
        return;

    QStringList list = ui->eSerialPortInfoList->itemData(idx).toStringList();
    ui->leDescription->setText(tr("Description: %1").arg(list.count() > 1 ? list.at(1) : tr(blankString)));
    ui->leManufacturer->setText(tr("Manufacturer: %1").arg(list.count() > 2 ? list.at(2) : tr(blankString)));
    ui->leSerialnumber->setText(tr("Serial number: %1").arg(list.count() > 3 ? list.at(3) : tr(blankString)));
    ui->leLocation->setText(tr("Location: %1").arg(list.count() > 4 ? list.at(4) : tr(blankString)));
    ui->leVid->setText(tr("Vendor Identifier: %1").arg(list.count() > 5 ? list.at(5) : tr(blankString)));
    ui->lePid->setText(tr("Product Identifier: %1").arg(list.count() > 6 ? list.at(6) : tr(blankString)));
}

void SettingsDialog::checkCustomBaudRatePolicy(int idx)
{
    bool isCustomBaudRate = !ui->eBaudrate->itemData(idx).isValid();
    ui->eBaudrate->setEditable(isCustomBaudRate);
    if (isCustomBaudRate)
    {
        ui->eBaudrate->clearEditText();
        QLineEdit* edit = ui->eBaudrate->lineEdit();
        edit->setValidator(m_pIntValidator);
    }
}

void SettingsDialog::checkCustomDevicePathPolicy(int idx)
{
    bool isCustomPath = !ui->eSerialPortInfoList->itemData(idx).isValid();
    ui->eSerialPortInfoList->setEditable(isCustomPath);
    if (isCustomPath)
        ui->eSerialPortInfoList->clearEditText();
}

void SettingsDialog::fillPortsParameters()
{
    ui->eBaudrate->addItem(QStringLiteral("9600"), QSerialPort::Baud9600);
    ui->eBaudrate->addItem(QStringLiteral("19200"), QSerialPort::Baud19200);
    ui->eBaudrate->addItem(QStringLiteral("38400"), QSerialPort::Baud38400);
    ui->eBaudrate->addItem(QStringLiteral("115200"), QSerialPort::Baud115200);
    ui->eBaudrate->addItem(tr("Custom"));

    ui->eDatabits->addItem(QStringLiteral("5"), QSerialPort::Data5);
    ui->eDatabits->addItem(QStringLiteral("6"), QSerialPort::Data6);
    ui->eDatabits->addItem(QStringLiteral("7"), QSerialPort::Data7);
    ui->eDatabits->addItem(QStringLiteral("8"), QSerialPort::Data8);
    ui->eDatabits->setCurrentIndex(3);

    ui->eParity->addItem(tr("None"), QSerialPort::NoParity);
    ui->eParity->addItem(tr("Even"), QSerialPort::EvenParity);
    ui->eParity->addItem(tr("Odd"), QSerialPort::OddParity);
    ui->eParity->addItem(tr("Mark"), QSerialPort::MarkParity);
    ui->eParity->addItem(tr("Space"), QSerialPort::SpaceParity);

    ui->eStopbits->addItem(QStringLiteral("1"), QSerialPort::OneStop);
#ifdef Q_OS_WIN
    ui->eStopbits->addItem(tr("1.5"), QSerialPort::OneAndHalfStop);
#endif
    ui->eStopbits->addItem(QStringLiteral("2"), QSerialPort::TwoStop);

    ui->eFlowcontrol->addItem(tr("None"), QSerialPort::NoFlowControl);
    ui->eFlowcontrol->addItem(tr("RTS/CTS"), QSerialPort::HardwareControl);
    ui->eFlowcontrol->addItem(tr("XON/XOFF"), QSerialPort::SoftwareControl);
}

void SettingsDialog::fillPortsInfo()
{
    ui->eSerialPortInfoList->clear();
    QString description;
    QString manufacturer;
    QString serialNumber;
    const auto infos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo& info : infos)
    {
        QStringList list;
        description = info.description();
        manufacturer = info.manufacturer();
        serialNumber = info.serialNumber();
        list << info.portName()
            << (!description.isEmpty() ? description : blankString)
            << (!manufacturer.isEmpty() ? manufacturer : blankString)
            << (!serialNumber.isEmpty() ? serialNumber : blankString)
            << info.systemLocation()
            << (info.vendorIdentifier() ? QString::number(info.vendorIdentifier(), 16) : blankString)
            << (info.productIdentifier() ? QString::number(info.productIdentifier(), 16) : blankString);

        ui->eSerialPortInfoList->addItem(list.first(), list);
    }

    ui->eSerialPortInfoList->addItem(tr("Custom"));
}

void SettingsDialog::saveSettings()
{
    // Save serial port configuration
    m_settings.setValue("serial/portname", ui->eSerialPortInfoList->currentText());
    if (ui->eBaudrate->currentIndex() == 4)
        m_settings.setValue("serial/baudrate", static_cast<QSerialPort::BaudRate>(
                ui->eBaudrate->currentText().toInt()));
    else
        m_settings.setValue("serial/baudrate", static_cast<QSerialPort::BaudRate>(
                ui->eBaudrate->itemData(ui->eBaudrate->currentIndex()).toInt()));
    m_settings.setValue("serial/databits", static_cast<QSerialPort::DataBits>(
            ui->eDatabits->itemData(ui->eDatabits->currentIndex()).toInt()));
    m_settings.setValue("serial/parity", static_cast<QSerialPort::Parity>(
            ui->eParity->itemData(ui->eParity->currentIndex()).toInt()));
    m_settings.setValue("serial/stopbits", static_cast<QSerialPort::StopBits>(
            ui->eStopbits->itemData(ui->eStopbits->currentIndex()).toInt()));
    m_settings.setValue("serial/flowcontrol", static_cast<QSerialPort::FlowControl>(
            ui->eFlowcontrol->itemData(ui->eFlowcontrol->currentIndex()).toInt()));
    m_settings.setValue("serial/autoconnect", ui->eAutoconnect->isChecked());

    // Save serial protocol configuration
    m_settings.setValue("serial/protocol/startflag",
        static_cast<uint8_t>(ui->eStartFlag->text().mid(2).toInt(nullptr, 16)));
    m_settings.setValue("serial/protocol/endflag",
        static_cast<uint8_t>(ui->eEndFlag->text().mid(2).toInt(nullptr, 16)));
    m_settings.setValue("serial/protocol/escapeflag",
        static_cast<uint8_t>(ui->eEscapeFlag->text().mid(2).toInt(nullptr, 16)));
    m_settings.setValue("serial/protocol/xorflag",
        static_cast<uint8_t>(ui->eXORFlag->text().mid(2).toInt(nullptr, 16)));
    m_settings.setValue("serial/protocol/ackflag",
        static_cast<uint8_t>(ui->eACKFlag->text().mid(2).toInt(nullptr, 16)));
    m_settings.setValue("serial/protocol/nackflag",
        static_cast<uint8_t>(ui->eNACKFlag->text().mid(2).toInt(nullptr, 16)));
    m_settings.setValue("serial/protocol/acktimeout",
        static_cast<uint8_t>(ui->eAckTimeOut->value()));
}

void SettingsDialog::loadSettings()
{
    // Loads serial port configuration
    ui->eSerialPortInfoList->setCurrentText(
        m_settings.value("serial/portname", "COM1").toString());
    int iBaudrate = ui->eBaudrate->findData(static_cast<QSerialPort::BaudRate>(
                m_settings.value("serial/baudrate", 115200).toInt()));
    if (iBaudrate != -1)
        ui->eBaudrate->setCurrentIndex(iBaudrate);
    else
    {
        ui->eBaudrate->setCurrentIndex(4);
        ui->eBaudrate->setCurrentText(QString::number(
                m_settings.value("serial/baudrate", 115200).toInt()));
    }
    int iDatabits = ui->eDatabits->findData(static_cast<QSerialPort::DataBits>(
                m_settings.value("serial/databits", QSerialPort::Data8).toInt()));
    ui->eDatabits->setCurrentIndex(iDatabits);
    int iParity = ui->eParity->findData(static_cast<QSerialPort::Parity>(
                m_settings.value("serial/parity", QSerialPort::NoParity).toInt()));
    ui->eParity->setCurrentIndex(iParity);
    int iStopbits = ui->eStopbits->findData(static_cast<QSerialPort::StopBits>(
                m_settings.value("serial/stopbits", QSerialPort::OneStop).toInt()));
    ui->eStopbits->setCurrentIndex(iStopbits);
    int iFlowcontrol = ui->eFlowcontrol->findData(static_cast<QSerialPort::FlowControl>(
                m_settings.value("serial/flowcontrol", QSerialPort::HardwareControl).toInt()));
    ui->eFlowcontrol->setCurrentIndex(iFlowcontrol);
    ui->eAutoconnect->setChecked(m_settings.value("serial/autoconnect", false).toBool());

    // Loads serial protocol information
    ui->eStartFlag->setValue(static_cast<uint8_t>(
            m_settings.value("serial/protocol/startflag", 0x12).toInt()));
    ui->eEndFlag->setValue(static_cast<uint8_t>(
            m_settings.value("serial/protocol/endflag", 0x13).toInt()));
    ui->eEscapeFlag->setValue(static_cast<uint8_t>(
            m_settings.value("serial/protocol/escapeflag", 0x7D).toInt()));
    ui->eXORFlag->setValue(static_cast<uint8_t>(
            m_settings.value("serial/protocol/xorflag", 0x20).toInt()));
    ui->eACKFlag->setValue(static_cast<uint8_t>(
            m_settings.value("serial/protocol/ackflag", 0xFB).toInt()));
    ui->eNACKFlag->setValue(static_cast<uint8_t>(
            m_settings.value("serial/protocol/nackflag", 0xFC).toInt()));
    ui->eAckTimeOut->setValue(static_cast<uint8_t>(
            m_settings.value("serial/protocol/acktimeout", 50).toInt()));
}
