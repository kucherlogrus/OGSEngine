#ifndef OGSENGINE_METALRENDERER_H
#define OGSENGINE_METALRENDERER_H

#include "render/IRenderer.h"

#include <unordered_map>
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
    MTL::Device*               device             = nullptr;
    MTL::CommandQueue*         commandQueue       = nullptr;
    CA::MetalLayer*            metalLayer         = nullptr;

    // Triangle test pipeline (existing)
    MTL::RenderPipelineState*  pipelineState      = nullptr;
    MTL::Buffer*               vertexBuffer       = nullptr;

    // Sprite pipeline
    MTL::RenderPipelineState*  spritePipelineState = nullptr;
    MTL::SamplerState*         spriteSampler       = nullptr;

    // Texture registry: handle.id → MTLTexture*
    std::unordered_map<uint32_t, MTL::Texture*> textures;
    uint32_t nextTextureId = 1;

    // Per-frame state — valid between beginFrame() and endFrame()
    CA::MetalDrawable*         currentDrawable = nullptr;
    MTL::CommandBuffer*        currentCmd      = nullptr;
    MTL::RenderCommandEncoder* currentEncoder  = nullptr;

    void buildPipeline();
    void buildVertexBuffer();
    void buildSpritePipeline();
    void buildSpriteSampler();

    void submitSpriteBatch(const SpriteBatchCommand& batch);
};

} // ogs

#endif //OGSENGINE_METALRENDERER_H