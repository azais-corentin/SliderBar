#include "plugin.h"

#include <iostream>

#include <QDebug>

Plugin::Plugin(std::string name, std::string text)
{
    m_name = name;

    m_lua.open_libraries(sol::lib::base);

    // Setup script functions
    m_lua.set_function("keys", &Plugin::keys, this);
    m_lua.set_function("setPosition", &Plugin::setPosition, this);

    // Setup variables
    m_lua["position"] = 0.;
    m_lua["delta"]    = 0.;

    // Load and execute script
    m_valid = m_lua.script(text).valid();

    // Get plugin activator
    sol::function activatorfct = m_lua["activator"];
    qDebug() << "activator:" << QString::fromStdString(activatorfct());
}

void Plugin::processPosition(float position)
{
    // Set the new position
    m_lua["position"] = position;
    // Callback
    m_lua["onPosition"]();
}

void Plugin::keys(std::string str)
{
    // Here, request for the key(s) contained in str to be pressed
    qDebug() << "key(" << QString::fromStdString(str) << ") called!";
}

void Plugin::setPosition(float x)
{
    // Here, request for the position to be set to x
    qDebug() << "setPosition(" << x << ") called!";
}
