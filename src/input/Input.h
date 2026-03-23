#ifndef OGS_INPUT_H
#define OGS_INPUT_H

#include <vector>
#include "monitoring/Logsystem.h"
#include "TouchInputController.h"
#include "InputProcessor.h"
#include "KeyboardCodes.h"
#include "KeyboardController.h"
#include "MouseController.h"


namespace ogs {

class InputProcessor;
class TouchInputController;
class KeyboardController;
class MouseController;

class Input {

public:

    enum class TouchType {
        Down,
        Up,
        Move,
        OFF,
    };

    enum class MouseType {
        NONE,
        MOVE,
        SCROLL
    };

    static Input *getInstance();

    void inputProcess();

    void handleTouchStart(int id, float x, float y);

    void handleTouchEnd(int id, float x, float y);

    void handleTouchMove(int id, float x, float y);

    void handleMove(MouseType type, float x, float y);

    void handleKeyboardEvent(KeyboardMap::KeyboardMap::KeyCode code, int scancode, int action, int mods );

    InputProcessor* getInputProcessor(){ return inputProcessor; };
    void setInputProcessor(InputProcessor* newProcessor);

    std::string getCharByCode(KeyboardMap::KeyboardMap::KeyCode code) {
        return keyboardMap.getChar(code);
    }

    void resizeWindow( int width, int height);

private:
    Input();
    ~Input();
    static Input *p_input;
    bool pressed = false;

    void init();

    InputProcessor* inputProcessor;
    TouchInputController* touchController;
    KeyboardController* keyboardConroller;
    MouseController* mouseController;
    KeyboardMap keyboardMap;
};

struct TouchEvent {
    Input::TouchType type;
    int id;
    float x;
    float y;
    float xd;
    float yd;

    TouchEvent() : type(Input::TouchType::OFF), id(-1), x(0), y(0), xd(0), yd(0) {};

    TouchEvent(Input::TouchType _type, int _id, float _x, float _y):
            type(_type), id(_id), x(_x), y(_y), xd(0), yd(0) {};

    TouchEvent(const TouchEvent& ev) {
        type = ev.type;
        id = ev.id;
        x = ev.x;
        y = ev.y;
        xd = ev.xd;
        yd = ev.yd;
    }

    TouchEvent operator=(const TouchEvent& ev) {
        type = ev.type;
        id = ev.id;
        x = ev.x;
        y = ev.y;
        xd = ev.xd;
        yd = ev.yd;
        return *this;
    }

};

struct KeyboardEvent {
    KeyboardMap::KeyCode code = KeyboardMap::KeyCode::KEY_NONE;
    int scancode {0};
    int action {0};
    int mods {0};

    KeyboardEvent() {
        code = KeyboardMap::KeyCode::KEY_NONE;
        scancode = -1;
        action = -1;
        mods = -1;
    }

    explicit KeyboardEvent(KeyboardMap::KeyCode _code, int scan, int act,  int _mods) {
        code = _code;
        scancode = scan;
        action = act;
        mods = _mods;
    }

    explicit KeyboardEvent(const KeyboardEvent& event) {
        code = event.code;
        scancode = event.scancode;
        action = event.action;
        mods = event.mods;
    }

    void operator=(const KeyboardEvent& event) {
        code = event.code;
        scancode = event.scancode;
        action = event.action;
        mods = event.mods;
    }

    void setVars(KeyboardMap::KeyCode _code, int _scan, int _act, int _mods) {
        code = _code;
        scancode = _scan;
        action = _act;
        mods = _mods;
    }

};

struct MouseEvent {
    Input::MouseType type;
    float x;
    float y;

    MouseEvent() {
        type = Input::MouseType::NONE;
        x = -1;
        y = -1;
    }

    MouseEvent(Input::MouseType _type, float x, float y) :type(_type), x(x), y(y) {}

    MouseEvent (const MouseEvent& ev) {
        type = ev.type;
        x = ev.x;
        y = ev.y;
    }

    MouseEvent operator=(const MouseEvent& ev) {
        type = ev.type;
        x = ev.x;
        y = ev.y;
        return *this;
    }

    void setVars(Input::MouseType _type, float _x, float _y) {
        type = _type;
        x = _x;
        y = _y;
    }
};

}

#endif
