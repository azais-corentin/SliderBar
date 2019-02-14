#include "plugin.h"

#include <iostream>

#include <QDebug>

#include "../hardwareio/keyboard.h"
#include "../hardwareio/mouse.h"

namespace sliderbar {

Plugin::Plugin(std::string name, std::string text)
{
    m_name = name;

    m_lua.open_libraries(sol::lib::base);

    // Setup script functions
    m_lua.set_function("keyPress", &Keyboard::Output::press);
    m_lua.set_function("keyDown", &Keyboard::Output::down);
    m_lua.set_function("keyUp", &Keyboard::Output::up);
    m_lua.set_function("mouseScroll", &Mouse::Output::scroll);

    m_lua.set_function("setPosition", &Plugin::setPosition, this);

    // Setup activation
    m_lua["Hold"]   = Activation::Hold;
    m_lua["Toggle"] = Activation::Toggle;

    // Setup variables
    m_lua["isAutoMoving"] = false;
    m_lua["position"]     = 0.;
    m_lua["delta"]        = 0.;

    // Load, execute and validate script
    try {
        auto bad_code_result = m_lua.script(text, &sol::script_default_on_error);
    } catch (const sol::error& e) {
        qDebug() << e.what();
        m_valid = false;
        return;
    }

    // Get plugin activator
    std::string activator;
    sol::tie(activator, m_activationMode) = m_lua["activator"]();
    m_activator                           = QKeySequence::fromString(QString::fromStdString(activator));

    // Get plugin settings
    sol::table settings = m_lua["settings"]();
    settings.for_each([&](std::pair<sol::object, sol::object> kvp) {
        std::string name, desc;
        name = kvp.first.as<std::string>();
        desc = kvp.second.as<std::string>();
        m_settings.insert(std::make_pair(name, desc));
    });

    qDebug() << "Plugin" << QString::fromStdString(name) << "successfully loaded";
}

void Plugin::processPosition(float position)
{
    // Store the new position
    m_lua["position"] = position;
    // Execute the callback
    m_lua["onPosition"]();
}

void Plugin::keys(std::string str)
{
    // Here, request for the key(s) contained in str to be pressed
    qDebug() << "key(" << QString::fromStdString(str) << ") called!";
}

void Plugin::setPosition(float x)
{
    // Here, request for the sliderbar position to be set to x
    qDebug() << "setPosition(" << x << ") called!";
}

}
