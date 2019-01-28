#ifndef PLUGIN_H
#define PLUGIN_H

#include <string>

#include <sol.hpp>

class Plugin {
public:
    Plugin(std::string name, std::string text);

    bool isValid() { return m_valid; }
    std::string getName() { return m_name; }

    void processPosition(float position);

private:
    void keys(std::string str);
    void setPosition(float x);
    void resetDelta(sol::this_state s);

    std::string m_name;

    sol::state m_lua;
    bool m_valid = false;
};

#endif // PLUGIN_H
