//
// Created by Logrus on 22.03.2026.
//

#include "window.h"

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

    void DesktopWindow::createWindow() {
        loginfo("DesktopWindow", "createWindow", "Creating window");
        if (!glfwInit()) {
            logerror("DesktopWindow", "glfwInit", "Failed to initialize GLFW");
            return;
        }
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        window = glfwCreateWindow(800, 600, "OGS Engine", nullptr, nullptr);
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

} // ogs