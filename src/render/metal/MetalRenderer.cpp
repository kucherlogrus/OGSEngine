//
// Created by Logrus on 22.03.2026.
//

#include "MetalRenderer.h"
#include "backends/AppWindow.h"
#include "monitoring/Logsystem.h"
#include "render/TextureHandle.h"
#include "storage/Texture.h"

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
    currentDrawable = metalLayer->nextDrawable();
    if (!currentDrawable) return;

    MTL::RenderPassDescriptor* pass = MTL::RenderPassDescriptor::alloc()->init();
    auto* colorAttachment = pass->colorAttachments()->object(0);
    colorAttachment->setTexture(currentDrawable->texture());
    colorAttachment->setLoadAction(MTL::LoadActionClear);
    colorAttachment->setClearColor(MTL::ClearColor(0.1, 0.1, 0.1, 1.0));
    colorAttachment->setStoreAction(MTL::StoreActionStore);

    currentCmd     = commandQueue->commandBuffer();
    currentEncoder = currentCmd->renderCommandEncoder(pass);
    currentEncoder->setRenderPipelineState(pipelineState);

    pass->release();
}

void MetalRenderer::submit(const RenderQueue& queue) {
    if (!currentEncoder) return;

    // TODO: when real mesh/material system exists, iterate queue.opaque and queue.transparent
    // and dispatch actual draw calls per RenderCommand.
    // For now draw the static test geometry so the screen is not blank.
    (void)queue;
    currentEncoder->setVertexBuffer(vertexBuffer, 0, 0);
    currentEncoder->drawPrimitives(MTL::PrimitiveTypeTriangle, NS::UInteger(0), NS::UInteger(6));
}

void MetalRenderer::endFrame() {
    if (!currentEncoder) return;

    currentEncoder->endEncoding();
    currentCmd->presentDrawable(currentDrawable);
    currentCmd->commit();

    currentEncoder  = nullptr;
    currentCmd      = nullptr;
    currentDrawable = nullptr;
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

TextureHandle MetalRenderer::uploadTexture(const TextureData& data) {
    // TODO: create MTLTexture from data
    return TextureHandle{};
}

void MetalRenderer::releaseTexture(TextureHandle handle) {
    // TODO: release MTLTexture by handle
}

} // ogs