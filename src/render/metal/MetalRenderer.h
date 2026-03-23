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
    void endFrame() override;
    void shutdown() override;

private:
    MTL::Device*              device        = nullptr;
    MTL::CommandQueue*        commandQueue  = nullptr;
    MTL::RenderPipelineState* pipelineState = nullptr;
    MTL::Buffer*              vertexBuffer  = nullptr;
    CA::MetalLayer*           metalLayer    = nullptr;

    void buildPipeline();
    void buildVertexBuffer();
};

} // ogs

#endif //OGSENGINE_METALRENDERER_H