#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class SliderBar;

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

private:
    void initialiseActions();

    Ui::MainWindow* ui       = nullptr;
    QAction* m_actionPlugins = nullptr;

    SliderBar* m_sliderbar;
};

#endif // MAINWINDOW_H
