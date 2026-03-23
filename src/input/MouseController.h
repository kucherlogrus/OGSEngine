//
// Created by logrus on 02.05.19.
//

#ifndef OGSENGINE_MOUSECONTROLLER_H
#define OGSENGINE_MOUSECONTROLLER_H
#include "Input.h"
#include <mutex>
#include "vector"

namespace ogs {

class InputProcessor;
struct MouseEvent;

class MouseController {

    std::mutex t_lock;
    int currIndex = 0;
    std::vector<MouseEvent> _mouse_events;

public:
    void handleMouseEvent(int type, float x, float y);
    void dispatchEvents(InputProcessor* processor);

    MouseController();

};

}


#endif //OGSENGINE_MOUSECONTROLLER_H
