#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <Qt>

namespace Keyboard {

class Output {
public:
    static void press(Qt::Key key);
    static void down(Qt::Key key);
    static void up(Qt::Key key);
};

}

#endif // KEYBOARD_H
