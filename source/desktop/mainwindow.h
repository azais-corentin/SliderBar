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
    void loadSettings();
    void saveSettings();

    void displayCalibrationData(uint32_t minpos, uint32_t maxpos, float maxvel);

private:
    void initialiseActions();

    Ui::MainWindow* ui       = nullptr;
    QAction* m_actionPlugins = nullptr;

    SliderBar* m_sliderbar;
    SerialInterface* m_dataInterface;
};

#endif // MAINWINDOW_H
