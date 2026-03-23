//
// Created by Logrus on 22.03.2026.
//

#include "window.h"

#include <input/Input.h>

#include "monitoring/Logsystem.h"

#ifdef MACOS
#include "backends/macos/MetalWindowHelper.h"
#endif

namespace ogs {

    DesktopWindow::DesktopWindow() : window(nullptr) {
    }

    DesktopWindow::~DesktopWindow() {
        if (window) {
            glfwDestroyWindow(window);
        }
        glfwTerminate();
    }

    void DesktopWindow::createWindow(int w, int h) {
        loginfo("DesktopWindow", "createWindow", "Creating window");
        if (!glfwInit()) {
            logerror("DesktopWindow", "glfwInit", "Failed to initialize GLFW");
            return;
        }
        monitor = glfwGetPrimaryMonitor();
        glfwGetMonitorWorkarea(monitor, &xpos_work, &ypos_work, &wh_work, &ht_work);
        loginfo("DesktopWindow", "createWindow", "glfwGetMonitorWorkarea", xpos_work, ypos_work, wh_work, ht_work);
        vidmode = glfwGetVideoMode(monitor);
        glfwGetMonitorContentScale(monitor, &xscale, &yscale);
        loginfo("DesktopWindow", "init", "glfwGetMonitorContentScale", xscale, yscale);
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        if(w > 0 && h > 0) {
            window = glfwCreateWindow(w, h, "OGS_Engine", NULL , NULL);
        } else {
            glfwWindowHint(GLFW_RED_BITS, vidmode->redBits);
            glfwWindowHint(GLFW_GREEN_BITS, vidmode->greenBits);
            glfwWindowHint(GLFW_BLUE_BITS, vidmode->blueBits);
            glfwWindowHint(GLFW_REFRESH_RATE, vidmode->refreshRate);
            loginfo("GLController", "init", vidmode->width, vidmode->height);
            window = glfwCreateWindow(vidmode->width, vidmode->height, "OGS_Engine", monitor, NULL);
            glfwMaximizeWindow(window);
        }
        if (!window) {
            logerror("DesktopWindow", "glfwCreateWindow", "Failed to create window");
            glfwTerminate();
            return;
        }

#ifdef MACOS
        metalLayer = attachMetalLayer(window);
        if (!metalLayer) {
            logerror("DesktopWindow", "attachMetalLayer", "Failed to attach Metal layer");
        } else
        {
            loginfo("DesktopWindow", "attachMetalLayer", "Successfully attached Metal layer");
        }
#endif
    }

    void DesktopWindow::pollEvents() {
        glfwPollEvents();
    }

    bool DesktopWindow::shouldClose() const {
        return window && glfwWindowShouldClose(window);
    }

    void* DesktopWindow::getNativeHandle() const {
        return window;
    }

    void DesktopWindow::setInputHandler(Input* input) {
        inputHandler = input;
        if (window && inputHandler) {
            registerCallbacks();
        }
    }

    void DesktopWindow::registerCallbacks() {
        glfwSetWindowUserPointer(window, this);

        glfwSetKeyCallback(window, [](GLFWwindow* w, int key, int scancode, int action, int mods) {
            auto* self = static_cast<DesktopWindow*>(glfwGetWindowUserPointer(w));
            if (!self->inputHandler) return;
            auto it = self->g_KeyboardMap.find(key);
            KeyboardMap::KeyCode code = (it != self->g_KeyboardMap.end()) ? it->second : KeyboardMap::KeyCode::KEY_NONE;
            self->inputHandler->handleKeyboardEvent(code, scancode, action, mods);
        });

        glfwSetCursorPosCallback(window, [](GLFWwindow* w, double xpos, double ypos) {
            auto* self = static_cast<DesktopWindow*>(glfwGetWindowUserPointer(w));
            if (self->inputHandler)
                self->inputHandler->handleMove(Input::MouseType::MOVE,static_cast<float>(xpos), static_cast<float>(ypos));
        });

        glfwSetScrollCallback(window, [](GLFWwindow* w, double xoffset, double yoffset) {
            auto* self = static_cast<DesktopWindow*>(glfwGetWindowUserPointer(w));
            if (self->inputHandler)
                self->inputHandler->handleMove(Input::MouseType::SCROLL, static_cast<float>(xoffset), static_cast<float>(yoffset));
        });

        glfwSetMouseButtonCallback(window, [](GLFWwindow* w, int button, int action, int mods) {
            auto* self = static_cast<DesktopWindow*>(glfwGetWindowUserPointer(w));
            if (!self->inputHandler) return;
            double xpos, ypos;
            glfwGetCursorPos(w, &xpos, &ypos);
            if (action == GLFW_PRESS) {
                self->inputHandler->pressCounter++;
                self->inputHandler->pressButtonNum = button;
                self->inputHandler->handleTouchStart(button,
                    static_cast<float>(xpos), static_cast<float>(ypos));
            } else if (action == GLFW_RELEASE) {
                self->inputHandler->pressCounter--;
                if (self->inputHandler->pressButtonNum == button)
                    self->inputHandler->pressButtonNum = -1;
                self->inputHandler->handleTouchEnd(button,static_cast<float>(xpos), static_cast<float>(ypos));
            }
        });
    }

} // ogs