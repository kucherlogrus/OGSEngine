//
// Created by Logrus on 22.03.2026.
//

#include "MetalRenderer.h"
#include "backends/AppWindow.h"
#include "monitoring/Logsystem.h"

namespace ogs {

// Vertex layout: x, y, r, g, b
struct Vertex {
    float x, y;
    float r, g, b;
};

// Two triangles, different colors
static const Vertex triangleVertices[] = {
    // Triangle 1 — red, top-left area
    { -0.8f,  0.8f,  1.0f, 0.2f, 0.2f },
    { -0.2f,  0.8f,  1.0f, 0.2f, 0.2f },
    { -0.5f,  0.2f,  1.0f, 0.2f, 0.2f },

    // Triangle 2 — blue, bottom-right area
    {  0.2f, -0.2f,  0.2f, 0.4f, 1.0f },
    {  0.8f, -0.2f,  0.2f, 0.4f, 1.0f },
    {  0.5f, -0.8f,  0.2f, 0.4f, 1.0f },
};

void MetalRenderer::init(AppWindow& window) {
    device = MTL::CreateSystemDefaultDevice();
    if (!device) {
        logerror("MetalRenderer", "init", "Failed to create Metal device");
        return;
    }

    metalLayer = reinterpret_cast<CA::MetalLayer*>(window.getMetalLayer());
    if (!metalLayer) {
        logerror("MetalRenderer", "init", "No Metal layer on window");
        return;
    }
    metalLayer->setDevice(device);
    metalLayer->setPixelFormat(MTL::PixelFormatBGRA8Unorm);

    commandQueue = device->newCommandQueue();

    buildPipeline();
    buildVertexBuffer();

    loginfo("MetalRenderer", "init", "Metal renderer initialized");
}

void MetalRenderer::buildPipeline() {
    NS::Error* error = nullptr;

    // Load shaders from the default library (compiled .metal files)
    MTL::Library* library = device->newDefaultLibrary();
    if (!library) {
        logerror("MetalRenderer", "buildPipeline", "Failed to load default shader library");
        return;
    }

    MTL::Function* vertexFn   = library->newFunction(
        NS::String::string("vertex_main", NS::UTF8StringEncoding));
    MTL::Function* fragmentFn = library->newFunction(
        NS::String::string("fragment_main", NS::UTF8StringEncoding));

    MTL::RenderPipelineDescriptor* desc = MTL::RenderPipelineDescriptor::alloc()->init();
    desc->setVertexFunction(vertexFn);
    desc->setFragmentFunction(fragmentFn);
    desc->colorAttachments()->object(0)->setPixelFormat(MTL::PixelFormatBGRA8Unorm);

    pipelineState = device->newRenderPipelineState(desc, &error);
    if (!pipelineState) {
        logerror("MetalRenderer", "buildPipeline", "Failed to create pipeline state");
    }

    desc->release();
    vertexFn->release();
    fragmentFn->release();
    library->release();
}

void MetalRenderer::buildVertexBuffer() {
    vertexBuffer = device->newBuffer(
        triangleVertices,
        sizeof(triangleVertices),
        MTL::ResourceStorageModeShared
    );
}

void MetalRenderer::beginFrame() {
    CA::MetalDrawable* drawable = metalLayer->nextDrawable();
    if (!drawable) return;

    MTL::RenderPassDescriptor* pass = MTL::RenderPassDescriptor::alloc()->init();
    auto* colorAttachment = pass->colorAttachments()->object(0);
    colorAttachment->setTexture(drawable->texture());
    colorAttachment->setLoadAction(MTL::LoadActionClear);
    colorAttachment->setClearColor(MTL::ClearColor(0.1, 0.1, 0.1, 1.0)); // dark grey background
    colorAttachment->setStoreAction(MTL::StoreActionStore);

    MTL::CommandBuffer*        cmd      = commandQueue->commandBuffer();
    MTL::RenderCommandEncoder* encoder  = cmd->renderCommandEncoder(pass);

    encoder->setRenderPipelineState(pipelineState);
    encoder->setVertexBuffer(vertexBuffer, 0, 0);
    // Draw 6 vertices = 2 triangles
    encoder->drawPrimitives(MTL::PrimitiveTypeTriangle, NS::UInteger(0), NS::UInteger(6));

    encoder->endEncoding();
    cmd->presentDrawable(drawable);
    cmd->commit();

    pass->release();
}

void MetalRenderer::endFrame() {
    // frame finalized in beginFrame via commit
}

void MetalRenderer::shutdown() {
    if (vertexBuffer)  { vertexBuffer->release();  vertexBuffer  = nullptr; }
    if (pipelineState) { pipelineState->release(); pipelineState = nullptr; }
    if (commandQueue)  { commandQueue->release();  commandQueue  = nullptr; }
    if (device)        { device->release();        device        = nullptr; }
}

MetalRenderer::~MetalRenderer() {
    shutdown();
}

} // ogs