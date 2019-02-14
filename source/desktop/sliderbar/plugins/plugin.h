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

    Plugin(std::string name, std::string text);

    bool isValid() { return m_valid; }
    std::string getName() { return m_name; }

    std::map<std::string, std::string> getSettings() { return m_settings; }

    void processPosition(float position);

private:
    void keys(std::string str);
    void setPosition(float x);
    void resetDelta(sol::this_state s);

    std::string m_name;
    std::map<std::string, std::string> m_settings;

    QKeySequence m_activator;
    Activation m_activationMode;

    sol::state m_lua;
    bool m_valid = true;
};

}

#endif // PLUGIN_H
