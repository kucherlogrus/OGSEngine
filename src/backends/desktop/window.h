//
// Created by Logrus on 22.03.2026.
//

#ifndef OGSENGINE_DESKTOP_WINDOW_H
#define OGSENGINE_DESKTOP_WINDOW_H

#include "backends/AppWindow.h"
#include "glfw3.h"

namespace ogs {

class DesktopWindow : public AppWindow {
public:
    DesktopWindow();
    ~DesktopWindow() override;

    void createWindow() override;
    void pollEvents() override;
    bool shouldClose() const override;
    void* getNativeHandle() const override;
    void* getMetalLayer() const { return metalLayer; }

private:
    GLFWwindow* window;
    void* metalLayer = nullptr; // CAMetalLayer* on macOS, nullptr on other platforms
};

} // ogs

#endif //OGSENGINE_DESKTOP_WINDOW_H