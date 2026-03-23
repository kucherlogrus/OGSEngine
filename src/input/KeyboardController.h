//
// Created by logrus on 25.04.19.
//

#ifndef OGSENGINE_KEYBOARDCONTROLLER_H
#define OGSENGINE_KEYBOARDCONTROLLER_H
#include <mutex>
#include "vector"
#include "Input.h"

namespace ogs {

class InputProcessor;
struct KeyboardEvent;


class KeyboardController {

    std::mutex t_lock;
    int currIndex = 0;
    std::vector<KeyboardEvent> _key_events;

public:
    void handleKeyboardEvent(KeyboardMap::KeyCode code, int scancode, int action, int mods);
    void dispatchEvents(InputProcessor* processor);

    KeyboardController();
};

};
#endif //OGSENGINE_KEYBOARDCONTROLLER_H
