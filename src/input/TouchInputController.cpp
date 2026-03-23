#include "TouchInputController.h"


using namespace ogs;

TouchInputController::TouchInputController() {
    for(int i = 0; i < MAX_TOUCHES; i++){
        _event_a[i] = new TouchEvent;
        _event_b[i] = new TouchEvent;
        _moveEvents[i] = new TouchEvent { Input::TouchType::OFF , i, 0.0, 0.0};
    }
};

TouchInputController::~TouchInputController() {
    for(int i = 0; i < MAX_TOUCHES; i++){
        delete _event_a[i];
        delete _event_b[i];
        delete _moveEvents[i];
    }
};

void  TouchInputController::insertTouchEvent(int code, int id, float x, float y ){
    std::lock_guard guard(t_lock);
    if(currIndex >= MAX_TOUCHES) {
        return;
    } 
    Input::TouchType type;
    if (code == 0) {
        activeTouches++;
        type = Input::TouchType::Down;
    } else {
        activeTouches--;
        type = Input::TouchType::Up;
    }

    TouchEvent* event = active_event + currIndex;
    event->type = type;
    event->id = id;
    event->x = x;
    event->y = y;
    if(type == Input::TouchType::Down) {       
        _moveEvents[id]->type = Input::TouchType::Move;
        _moveEvents[id]->x = x;
        _moveEvents[id]->y = y;         
    } else {
        _moveEvents[id]->type = Input::TouchType::OFF;
        _moveEvents[id]->x = 0;
        _moveEvents[id]->y = 0;
        _moveEvents[id]->xd = 0;
        _moveEvents[id]->yd = 0;
    }
    currIndex++;
}

void TouchInputController::insertMoveEvent(int id, float x, float y ){
    std::lock_guard guard(t_lock);
    if(_moveEvents[id]->type != Input::TouchType::Move) {
        logerror("TouchInputController", "insertMoveEvent", "type != TouchType::Move");
        return; //something wrong, move event can be only after down event
    }
    float tmpX = _moveEvents[id]->x;
    float tmpY = _moveEvents[id]->y;
    _moveEvents[id]->x = x;
    _moveEvents[id]->y = y;
    _moveEvents[id]->xd = x - tmpX;
    _moveEvents[id]->yd = y - tmpY;
}


void TouchInputController::swapActivePointer() {
    std::lock_guard guard(t_lock);
    saveIndex = currIndex;
    currIndex = 0;
    savePointer = active_event;   
    active_event = active_event == _event_a[0] ? _event_b[0] : _event_a[0];
}
