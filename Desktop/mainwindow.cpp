#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "serialprotocol.h"

#include <QDebug>
#include <QLabel>
#include <QKeySequence>
#include <QTimer>

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //m_pSystemKeyboardHook = SystemKeyboardHook::instance();
    //m_pSystemKeyboardHook->setConnected(true);

    ui->setupUi(this);
    m_pStatus = new QLabel;
    ui->statusBar->addPermanentWidget(m_pStatus, 0);
    enableConnect();

    m_pSerial = new SerialProtocol;
    m_pSettingsDialog = new SettingsDialog(this);
    m_pSignalMapper = new QSignalMapper(this);

    m_pTimerStatusMessage = new QTimer;
    m_pTimerStatusMessage->setSingleShot(true);
    m_pTimerStatusMessage->setInterval(2000);

    initConnections();
    loadSettings();

    if (m_settings.value("serial/autoconnect", false).toBool())
        openSerialPort();

    //Setup plot
    ui->dataPlot->clearGraphs();
    // Position
    ui->dataPlot->addGraph();
    ui->dataPlot->graph(0)->setPen(QPen(QColor(255, 110, 40)));
    ui->dataPlot->graph(0)->setName("Position");
    // Target position
    ui->dataPlot->addGraph();
    ui->dataPlot->graph(1)->setPen(QPen(QColor(40, 200, 40)));
    ui->dataPlot->graph(1)->setName("Target position");
    // Estimated position
    ui->dataPlot->addGraph();
    ui->dataPlot->graph(2)->setPen(QPen(QColor(0, 0, 255)));
    ui->dataPlot->graph(2)->setName("Estimated position");
    // Estimated velocity
    ui->dataPlot->addGraph();
    ui->dataPlot->graph(3)->setPen(QPen(QColor(70, 0, 70)));
    ui->dataPlot->graph(3)->setName("Estimated velocity");

    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%z");
    ui->dataPlot->xAxis->setTicker(timeTicker);
    ui->dataPlot->yAxis->setRange(0, 100);
    ui->dataPlot->legend->setVisible(true);

    loadPlugins();
}

MainWindow::~MainWindow()
{
    delete ui;
    if (m_pStatus)
        delete m_pStatus;
    if (m_pSettingsDialog)
        delete m_pSettingsDialog;
    if (m_pTimerStatusMessage)
        delete m_pTimerStatusMessage;

    if (m_pSystemKeyboardHook)
        delete m_pSystemKeyboardHook;
    if (m_pSerial)
        delete m_pSerial;
}

void MainWindow::openSerialPort()
{
    m_pSerial->openSerialPort();
}

void MainWindow::closeSerialPort()
{
    m_pSerial->closeSerialPort();
}

void MainWindow::disableConnect()
{
    ui->actionConnect->setEnabled(false);
    ui->actionDisconnect->setEnabled(true);
}

void MainWindow::enableConnect()
{
    ui->actionConnect->setEnabled(true);
    ui->actionDisconnect->setEnabled(false);
}

void MainWindow::receivePacket(const command& packet)
{
    m_count++;
    static QTime time(QTime::currentTime());

    switch (packet.type)
    {
        case command::FORC_POSITION:
        {
            m_sliderPos = 0.0975879 * double(packet.value) + 0.2651662;
            double key = time.elapsed() / 1000.0;
            ui->progressBar->setValue(int(m_sliderPos));
            ui->dataPlot->graph(0)->addData(key, m_sliderPos);
            ui->dataPlot->graph(1)->addData(key, 0.0975879 * double(ui->eProgress->value()) + 0.2651662);
            ui->dataPlot->xAxis->setRange(key, 4, Qt::AlignRight);
            ui->dataPlot->replot();

            emit eventDispatch(SLIDER_POSITION, m_sliderPos);
            emit eventDispatch(SLIDER_DELTA, float(m_sliderPos) - m_sliderLastPosition);
            qDebug() << "delta:" << float(m_sliderPos) - m_sliderLastPosition;
            m_sliderLastPosition = float(m_sliderPos);

            break;
        }
        case command::FORC_EST_POS:
        {
            double key = time.elapsed() / 1000.0;
            ui->dataPlot->graph(2)->addData(key, 0.0975879 * double(packet.value) + 0.2651662);
            ui->dataPlot->xAxis->setRange(key, 4, Qt::AlignRight);
            ui->dataPlot->replot();
            break;
        }
        case command::FORC_EST_VEL:
        {
            double key = time.elapsed() / 1000.0;
            ui->dataPlot->graph(3)->addData(key, 0.0975879 * double(packet.value));
            ui->dataPlot->xAxis->setRange(key, 4, Qt::AlignRight);
            ui->dataPlot->replot();
            break;
        }
        default:
            qDebug() << "Error: Packet not meant for computer!";
            break;
    }
    showStatusMessage(QString::number(m_count));
}

int MainWindow::showConfiguration()
{
    int execute = m_pSettingsDialog->execute();
    loadSettings();
    return execute;
}

void MainWindow::loadSettings()
{
    // Load serial port configuration
    m_pSerial->loadSettings();
    ui->actionAuto_connect->setChecked(m_settings.value("serial/autoconnect", false).toBool());
}

void MainWindow::toggleAutoconnect()
{
    m_settings.setValue("serial/autoconnect", ui->actionAuto_connect->isChecked());
}

void MainWindow::initConnections()
{
    connect(ui->actionConnect, &QAction::triggered, this, &MainWindow::openSerialPort);
    connect(ui->actionDisconnect, &QAction::triggered, this, &MainWindow::closeSerialPort);
    connect(ui->actionExit, &QAction::triggered, this, &MainWindow::close);
    connect(ui->actionSettings, &QAction::triggered, this, &MainWindow::showConfiguration);
    connect(ui->actionAuto_connect, &QAction::triggered, this, &MainWindow::toggleAutoconnect);

    connect(m_pSerial, &SerialProtocol::serialConnected, this, &MainWindow::disableConnect);
    connect(m_pSerial, &SerialProtocol::serialDisconnected, this, &MainWindow::enableConnect);
    connect(m_pSerial, &SerialProtocol::statusMessage, this, &MainWindow::showStatusMessage);
    connect(m_pSerial, &SerialProtocol::packetReady, this, &MainWindow::receivePacket);

    connect(m_pTimerStatusMessage, &QTimer::timeout, this, &MainWindow::clearStatusMessage);
    //connect(m_pSystemKeyboardHook, &SystemKeyboardHook::keyPressed, this, &MainWindow::handleKeyPressed);
}

void MainWindow::showStatusMessage(const QString& message)
{
    m_pStatus->setText(message);
    m_pTimerStatusMessage->start(1000 + message.split(' ').length() * 800);
}

void MainWindow::clearStatusMessage()
{
    m_pStatus->clear();
}

void MainWindow::on_eProgress_valueChanged(int value)
{
    command cmd;
    cmd.type = command::FORS_POSITION;
    cmd.value = static_cast<uint16_t>(value);
    m_pSerial->writePacket(cmd);
}

void MainWindow::handleKeyPressed(DWORD key)
{
    QKeySequence seq(key);
    showStatusMessage("Key pressed: " + seq.toString() + ", " + QString::number(key));
}

void MainWindow::on_bAddResist_clicked()
{
    command cmd;
    cmd.type = command::FORS_RESIST_AT;
    cmd.value = static_cast<uint16_t>(ui->eResistAt->value());
    m_pSerial->writePacket(cmd);
}

void MainWindow::on_bClearResists_clicked()
{
    command cmd;
    cmd.type = command::FORS_RESIST_CLEAR;
    m_pSerial->writePacket(cmd);
}

void MainWindow::on_bDisablePID_clicked()
{
    command cmd;
    cmd.type = command::FORS_STOP_PID;
    m_pSerial->writePacket(cmd);
}

void MainWindow::on_bEnablePID_clicked()
{
    command cmd;
    cmd.type = command::FORS_START_PID;
    m_pSerial->writePacket(cmd);
}

void MainWindow::on_bUpdatePID_clicked()
{
    double pg = ui->epGain->value();
    double ig = ui->eiGain->value();
    double dg = ui->edGain->value();

    command pcmd;
    pcmd.type = command::FORS_PID_P;
    pcmd.value = static_cast<uint16_t>(pg * 327.68);
    m_pSerial->writePacket(pcmd);

    command icmd;
    icmd.type = command::FORS_PID_I;
    icmd.value = static_cast<uint16_t>(ig * 327.68);
    m_pSerial->writePacket(icmd);

    command dcmd;
    dcmd.type = command::FORS_PID_D;
    dcmd.value = static_cast<uint16_t>(dg * 327.68);
    m_pSerial->writePacket(dcmd);
}

void MainWindow::selectPlugin(int i)
{
    auto plugin = m_pluginList.at(i);
    bool checked = ui->menuPlugins->actions().at(i)->isChecked();
    //FIXME bug at the connect
    if (checked)
        connect(this, &MainWindow::eventDispatch, plugin, &SliderInterface::processEvent);
    else
        disconnect(this, &MainWindow::eventDispatch, plugin, &SliderInterface::processEvent);
}

void MainWindow::loadPlugins()
{
    qDebug() << "Loading plugins...";

    m_PluginsDir = QDir(qApp->applicationDirPath());

#if defined(Q_OS_WIN)
    if (m_PluginsDir.dirName().toLower() == "debug" || m_PluginsDir.dirName().toLower() == "release")
        m_PluginsDir.cdUp();
#elif defined(Q_OS_MAC)
    if (pluginsDir.dirName() == "MacOS")
    {
        pluginsDir.cdUp();
        pluginsDir.cdUp();
        pluginsDir.cdUp();
    }
#endif
    m_PluginsDir.cd("plugins");

    m_currentPluginIndex = 0;
    ui->menuPlugins->clear();
    int i = 0;

    foreach (QString fileName, m_PluginsDir.entryList(QDir::Files))
    {
        QPluginLoader loader(m_PluginsDir.absoluteFilePath(fileName));
        QObject* plugin = loader.instance();
        if (plugin)
        {
            SliderInterface* iPlugin = qobject_cast<SliderInterface*>(plugin);
            if (iPlugin)
            {
                //TODO Implement refreshing of the plugins
                //IMPL Clear WebsiteList and relaunch function
                QAction* pluginAction = ui->menuPlugins->addAction(loader.metaData()
                        .value("MetaData").toObject().value("name").toString(), m_pSignalMapper, SLOT(map()));
                QString tooltip = loader.metaData().value("MetaData")
                    .toObject().value("description").toString();
                pluginAction->setWhatsThis(tooltip);
                pluginAction->setToolTip(tooltip);
                pluginAction->setCheckable(true);

                m_pSignalMapper->setMapping(ui->menuPlugins->actions().at(i), i);
                m_pluginList.append(iPlugin);
                m_pluginFileNames += fileName;
                i++;
            }
        }
    }

    if ( i <= 0)
    {
        qWarning() << "No plugin found";
        m_currentPluginIndex = -1;
        return;
    }
    connect(m_pSignalMapper, SIGNAL(mapped(int)),
        this, SLOT(selectPlugin(int)), Qt::UniqueConnection);
}
