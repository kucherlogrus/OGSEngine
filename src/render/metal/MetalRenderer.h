//
// Created by Logrus on 22.03.2026.
//

#ifndef OGSENGINE_METALRENDERER_H
#define OGSENGINE_METALRENDERER_H

#include "render/IRenderer.h"

#include <Metal/Metal.hpp>
#include <QuartzCore/QuartzCore.hpp>

namespace ogs {

class MetalRenderer : public IRenderer {
public:
    MetalRenderer() = default;
    ~MetalRenderer() override;

    void init(AppWindow& window) override;
    void beginFrame() override;
    void submit(const RenderQueue& queue) override;
    void endFrame() override;
    void shutdown() override;

    TextureHandle uploadTexture(const TextureData& data) override;
    void          releaseTexture(TextureHandle handle)   override;

private:
    MTL::Device*               device        = nullptr;
    MTL::CommandQueue*         commandQueue  = nullptr;
    MTL::RenderPipelineState*  pipelineState = nullptr;
    MTL::Buffer*               vertexBuffer  = nullptr;
    CA::MetalLayer*            metalLayer    = nullptr;

    // Per-frame state — valid between beginFrame() and endFrame()
    CA::MetalDrawable*         currentDrawable = nullptr;
    MTL::CommandBuffer*        currentCmd      = nullptr;
    MTL::RenderCommandEncoder* currentEncoder  = nullptr;

    void buildPipeline();
    void buildVertexBuffer();
};

} // ogs

#endif //OGSENGINE_METALRENDERER_H