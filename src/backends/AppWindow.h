//
// Created by Logrus on 22.03.2026.
//

#ifndef OGSENGINE_APPWINDOW_H
#define OGSENGINE_APPWINDOW_H

#include "input/Input.h"

namespace ogs {

class AppWindow {
public:
    virtual ~AppWindow() = default;
    virtual void createWindow(int=-1, int=-1) = 0;
    virtual void setInputHandler(Input* input) = 0;
    virtual void pollEvents() = 0;
    virtual bool shouldClose() const = 0;
    virtual void* getNativeHandle() const = 0;
    virtual void* getMetalLayer() const { return nullptr; }
};

} // ogs

#endif //OGSENGINE_APPWINDOW_H