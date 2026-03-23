//
// Created by logrus on 25.04.19.
//

#include "KeyboardController.h"

using namespace ogs;

void KeyboardController::handleKeyboardEvent(KeyboardMap::KeyCode code, int scancode, int action, int mods) {
    std::lock_guard guard(t_lock);
    if(currIndex > 10) {
        return;
    }
    loginfo("KeyboardController", "handleKeyboardEvent", currIndex, static_cast<int>(code));
    _key_events[currIndex].setVars(code, scancode, action, mods);
    loginfo("KeyboardController", "handleKeyboardEvent", currIndex, static_cast<int>(_key_events[currIndex].code));
    currIndex++;

}

void KeyboardController::dispatchEvents(InputProcessor* processor) {
    std::lock_guard guard(t_lock);
    for (int i = 0; i < currIndex; i++) {
        loginfo("KeyboardController", "dispatchEvents", currIndex, static_cast<int>(_key_events[i].code));
        if (_key_events[i].action == 1) {
            processor->keyboardPress(_key_events[i]);
        } else {
            processor->keyboardRelease(_key_events[i]);
        }
    }
    currIndex = 0;
}
KeyboardController::KeyboardController() {
    _key_events.reserve(10);
    for (int i=0; i < 10; i++) {
        KeyboardEvent event {KeyboardMap::KeyCode::KEY_NONE, -1, -1, -1};
        _key_events.push_back(event);
    }
}

