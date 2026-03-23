//
// Created by logrus on 02.05.19.
//

#include "MouseController.h"
using namespace ogs;

void MouseController::handleMouseEvent(int type, float x, float y) {
    std::lock_guard guard(t_lock);
    if(currIndex > 10) {
        return;
    }
    Input::MouseType m_type = type == 1 ? Input::MouseType::MOVE : Input::MouseType::SCROLL;
    _mouse_events[currIndex].setVars(m_type, x,y);
    currIndex++;
}

void MouseController::dispatchEvents(InputProcessor* processor) {
    std::lock_guard guard(t_lock);
    for (int i = 0; i < currIndex; i++) {
        if (_mouse_events[i].type == Input::MouseType::MOVE) {
            processor->mouseMove(_mouse_events[i]);
        }

        if (_mouse_events[i].type == Input::MouseType::SCROLL) {
            processor->scrollMove(_mouse_events[i]);
        }
    }
    currIndex = 0;
}

MouseController::MouseController() {
    _mouse_events.reserve(10);
    for (int i=0; i < 10; i++) {
        MouseEvent event {Input::MouseType::NONE, -1.f, -1.f};
        _mouse_events.push_back(event);
    }
}