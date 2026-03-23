//
// Created by Logrus on 22.03.2026.
//

#include "MetalWindowHelper.h"

#define GLFW_EXPOSE_NATIVE_COCOA
#include "glfw3.h"
#include "glfw3native.h"

#import <Cocoa/Cocoa.h>
#import <Metal/Metal.h>
#import <QuartzCore/CAMetalLayer.h>

namespace ogs {

void* attachMetalLayer(GLFWwindow* glfwWindow) {
    NSWindow* nsWindow = glfwGetCocoaWindow(glfwWindow);
    if (!nsWindow) {
        return nullptr;
    }

    NSView* contentView = [nsWindow contentView];

    CAMetalLayer* metalLayer = [CAMetalLayer layer];
    metalLayer.device = MTLCreateSystemDefaultDevice();
    metalLayer.pixelFormat = MTLPixelFormatBGRA8Unorm;

    [contentView setLayer:metalLayer];
    [contentView setWantsLayer:YES];

    return (__bridge void*)metalLayer;
}

} // ogs