//
// Created by Logrus on 22.03.2026.
//

#ifndef OGSENGINE_METALWINDOWHELPER_H
#define OGSENGINE_METALWINDOWHELPER_H

struct GLFWwindow;

namespace ogs {

// Attaches a CAMetalLayer to the NSWindow obtained from a GLFW window.
// Returns a pointer to the CAMetalLayer, or nullptr on failure.
// Must be called after glfwCreateWindow().
void* attachMetalLayer(GLFWwindow* glfwWindow);

} // ogs

#endif //OGSENGINE_METALWINDOWHELPER_H