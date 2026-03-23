#ifndef OGS_TOUCH_INPUT_CONTROLLER_H
#define OGS_TOUCH_INPUT_CONTROLLER_H

#include "Input.h"
#include <mutex>
#define MAX_TOUCHES 10

namespace ogs {

    struct TouchEvent;

    class TouchInputController {
        friend class Input;

    public:
        TouchInputController();
        ~TouchInputController();
        void insertTouchEvent(int code, int, float, float);
        void insertMoveEvent(int, float, float);

    private:
        TouchEvent* _moveEvents[MAX_TOUCHES];
        TouchEvent* _event_a[MAX_TOUCHES];
        TouchEvent* _event_b[MAX_TOUCHES];
        TouchEvent* active_event;
        int currIndex = 0;
        int saveIndex = 0;
        int activeTouches = 0;
        TouchEvent* savePointer = nullptr;        
        void swapActivePointer();
        std::mutex t_lock;
    };

}



#endif

