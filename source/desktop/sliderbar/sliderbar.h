#ifndef SLIDERBAR_H
#define SLIDERBAR_H

#include <QObject>

class SliderBar : public QObject {
    Q_OBJECT

public:
    SliderBar(QWidget* parent);
    ~SliderBar() override;

public slots:
    /**
     * @brief Connects the SliderBar.
     */
    void connect();

    /**
     * @brief Show the settings window.
     * @note Blocking function.
     *
     */
    void manageSettings();

    /**
     * @brief Show the plugin management window.
     * @note Blocking function.
     */
    void managePlugins();

    /**
     * @brief Sets the autoconnect setting.
     * @param enabled True if autoconnect should be enabled.
     */
    void autoconnect(bool enabled);

signals:
    void settingsChanged();

private:
    QWidget* m_parent;
};

#endif // SLIDERBAR_H
