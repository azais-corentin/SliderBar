#ifndef MOUSE_H
#define MOUSE_H

#include <Qt>

namespace Mouse {

class Output {
public:
    static void scroll(int delta);
};

}

#endif // MOUSE_H
