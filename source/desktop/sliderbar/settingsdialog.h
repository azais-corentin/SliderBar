#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QSettings>
#include <QDialog>

QT_USE_NAMESPACE

QT_BEGIN_NAMESPACE

namespace Ui {
class SettingsDialog;
}

class QIntValidator;

QT_END_NAMESPACE

class SettingsDialog : public QDialog {
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = nullptr);
    ~SettingsDialog();

public slots:
    int execute();

private slots:
    void on_listCategories_currentRowChanged(int currentRow);

private:
    void saveSettings();
    void loadSettings();

    Ui::SettingsDialog *ui;
    QSettings m_settings;
};

#endif // SETTINGSDIALOG_H
