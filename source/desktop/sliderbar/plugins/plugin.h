#ifndef PLUGIN_H
#define PLUGIN_H

#include <map>
#include <string>

#include <sol.hpp>

#include <QKeySequence>

namespace sliderbar {

class Plugin {
public:
    enum class Activation : uint8_t {
        Hold,
        Toggle
    };

    struct Setting {
        std::string name;
        std::string description;
        sol::type type;
    };

    Plugin(std::string name, std::string text);
    void init();

    bool isValid() { return m_valid; }
    std::string getName() { return m_name; }

    std::vector<Setting> getSettings() { return m_settings; }
    template <class T>
    void setSetting(std::string name, T value);
    template <class T>
    T getSetting(std::string name);

    void processPosition(float position);

private:
    void keys(std::string str);
    void setPosition(float x);
    void resetDelta(sol::this_state s);

    void debug(std::string msg);

    std::string m_name;
    std::vector<Setting> m_settings;

    QKeySequence m_activator;
    Activation m_activationMode;

    sol::state m_lua;
    bool m_valid = true;
};

}

#include "plugin_impl.tpp"

#endif // PLUGIN_H
