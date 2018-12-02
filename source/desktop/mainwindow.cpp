#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSettings>

#include "sliderbar/manager.h"
#include "sliderbar/serialinterface.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->statusBar->addPermanentWidget(ui->lStatus);
    // Removes the separators
    ui->statusBar->setStyleSheet("QStatusBar::item {border: none;}");

    m_actionPlugins = new QAction("Plugins");
    ui->menuBar->addAction(m_actionPlugins);

    m_sliderbar = new SliderBar::Manager(this);

    loadQuickSettings();
    initialiseConnections();

    m_sliderbar->initialiseConnections();
}

MainWindow::~MainWindow()
{
    delete ui;

    delete m_actionPlugins;
    delete m_sliderbar;
    delete m_dataInterface;
}

void MainWindow::loadQuickSettings()
{
    ui->actionAutoconnect->setChecked(m_sliderbar->settings()->autoconnect());
}

void MainWindow::saveQuickSettings()
{
    m_sliderbar->settings()->autoconnect(ui->actionAutoconnect->isChecked());
}

void MainWindow::handleActionConnect()
{
    if (m_connected)
        m_sliderbar->disconnect();
    else
        m_sliderbar->connect();
}

void MainWindow::connected()
{
    m_connected = true;
    ui->actionConnect->setText("Disconnect");
}

void MainWindow::disconnected()
{
    m_connected = false;
    ui->actionConnect->setText("Connect");
}

void MainWindow::initialiseConnections()
{
    connect(ui->actionExit, &QAction::triggered,
            this, &MainWindow::close);

    // SliderBar actions
    connect(ui->actionConnect, &QAction::triggered,
            this, &MainWindow::handleActionConnect);
    connect(m_actionPlugins, &QAction::triggered,
            m_sliderbar, &SliderBar::Manager::managePlugins);

    connect(m_sliderbar, &SliderBar::Manager::connected,
            this, &MainWindow::connected);
    connect(m_sliderbar, &SliderBar::Manager::disconnected,
            this, &MainWindow::disconnected);

    // Settings
    connect(ui->actionSettings, &QAction::triggered,
            m_sliderbar->settings(), &SliderBar::Settings::showSettings);

    // Quick settings
    connect(m_sliderbar->settings(), &SliderBar::Settings::settingsChanged,
            this, &MainWindow::loadQuickSettings);
    connect(ui->actionAutoconnect, &QAction::triggered,
            this, &MainWindow::saveQuickSettings);
}
