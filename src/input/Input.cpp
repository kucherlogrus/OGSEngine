#include "Input.h"

using namespace ogs;

Input* Input::p_input = nullptr;

Input* Input::getInstance() {
    if (!p_input) {
        p_input = new Input();
        p_input->init();
        return p_input;
    }
    return p_input;
}

Input::Input():
inputProcessor(nullptr),
touchController(nullptr),
keyboardConroller(nullptr),
mouseController(nullptr)
{

}

Input::~Input() {
    delete inputProcessor;
    delete touchController;
    delete keyboardConroller;
    delete mouseController;
}

void Input::init(){
    touchController = new TouchInputController;
    keyboardConroller = new KeyboardController;
    mouseController = new MouseController;
    setInputProcessor(new DefaultInputProcessor);

}

void Input::setInputProcessor(InputProcessor* newProcessor) {
    if( inputProcessor != nullptr) {
        delete inputProcessor;
    }    
    inputProcessor = newProcessor;
}

void Input::inputProcess() {

    mouseController->dispatchEvents(inputProcessor);

    keyboardConroller->dispatchEvents(inputProcessor);

    touchController->swapActivePointer();

    for(int i = 0; i < touchController->saveIndex; i++) {
        const TouchEvent* event = touchController->savePointer++;
        switch( event->type){
            case TouchType::Down :
                if(inputProcessor != nullptr) {
                    inputProcessor->touchDown(*event);
                }
                break;
            case TouchType::Up :
                if(inputProcessor != nullptr) {
                    inputProcessor->touchUp(*event);
                }
                break;
            default:
                break;
        }
    }

    if(touchController->activeTouches == 0) {
        return;
    }

    for(int i = 0; i < MAX_TOUCHES; i++) {
        TouchEvent* event = touchController->_moveEvents[0];
        if(event->type == TouchType::Move){
            inputProcessor->move(*event);
        }
    }
 
}

void Input::handleTouchStart(int id, float x, float y) {
    touchController->insertTouchEvent(0, id, x, y);
}
void Input::handleTouchEnd(int id, float x, float y ) {
    touchController->insertTouchEvent(1, id, x, y);
}
void Input::handleTouchMove(int id, float x, float y ) {
    touchController->insertMoveEvent(id, x, y);
}

void Input::handleMove(MouseType type, float x, float y) {
    if(type == MouseType::MOVE){
        mouseController->handleMouseEvent(1, x, y);
        return;
    }
    if(type == MouseType::SCROLL){
        mouseController->handleMouseEvent(2, x, y);
        return;
    }

}

void Input::handleKeyboardEvent(KeyboardMap::KeyCode code, int scancode, int action, int mods) {
    keyboardConroller->handleKeyboardEvent(code, scancode, action, mods);
}




