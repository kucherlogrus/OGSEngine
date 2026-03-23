#ifndef OGS_INPUT_PROCESSOR_H
#define OGS_INPUT_PROCESSOR_H

#include "Input.h"

namespace ogs {

class Input;
struct TouchEvent;
struct KeyboardEvent;
struct MouseEvent;


class InputProcessor {

public:

    InputProcessor(){};

    virtual ~InputProcessor(){};

    virtual void touchDown(const TouchEvent&) {};

    virtual void touchUp(const TouchEvent&) {};

    virtual void move(const TouchEvent& ) {};

    virtual void mouseMove(const MouseEvent& ) {};

    virtual void scrollMove(const MouseEvent& ) {};

    virtual void keyboardPress(const KeyboardEvent&) {};

    virtual void keyboardRelease(const KeyboardEvent&) {};
};


class DefaultInputProcessor : public InputProcessor {

public:
    DefaultInputProcessor(){};
    virtual ~DefaultInputProcessor(){};
    virtual void touchDown(const TouchEvent&) override;
    virtual void touchUp(const TouchEvent&) override;
    virtual void move(const TouchEvent& ) override;
    virtual void keyboardPress(const KeyboardEvent&) override ;
    virtual void keyboardRelease(const KeyboardEvent&) override ;
    virtual void mouseMove(const MouseEvent& ) override;
    virtual void scrollMove(const MouseEvent& ) override;
};


}

#endif
