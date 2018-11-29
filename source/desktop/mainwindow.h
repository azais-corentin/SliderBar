#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class SliderBar;
class SerialInterface;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

public slots:
    void loadQuickSettings();
    void saveQuickSettings();

    void handleActionConnect();

    void connected();
    void disconnected();

private:
    void initialiseConnections();

    Ui::MainWindow* ui       = nullptr;
    QAction* m_actionPlugins = nullptr;

    SliderBar* m_sliderbar;
    bool m_connected = false;

    SerialInterface* m_dataInterface;
};

#endif // MAINWINDOW_H
