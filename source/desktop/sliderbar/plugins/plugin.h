#ifndef PLUGIN_H
#define PLUGIN_H

#include <string>

class Plugin {
public:
    Plugin(std::string script);

    bool isValid();
    void execute();
};

#endif // PLUGIN_H
