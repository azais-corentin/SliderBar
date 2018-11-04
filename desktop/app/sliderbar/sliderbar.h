#ifndef SLIDERBAR_H
#define SLIDERBAR_H

#include <QObject>

class SliderBar : public QObject {
    Q_OBJECT

public:
    SliderBar();
    ~SliderBar();

public slots:
    /**
     * @brief Connects the SliderBar.
     */
    void connect();

    /**
     * @brief Show the settings window.
     *
     */
    void settings();

    /**
     * @brief autoconnect
     * @param enabled
     */
    void autoconnect(bool enabled);
signals:

private:
};

#endif // SLIDERBAR_H
