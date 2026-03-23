#ifndef OGSENGINE_IRENDERER_H
#define OGSENGINE_IRENDERER_H

#include "RenderQueue.h"
#include "render/TextureHandle.h"
#include "storage/Texture.h"

namespace ogs {

class AppWindow;

class IRenderer {
public:
    virtual ~IRenderer() = default;

    virtual void init(AppWindow& window) = 0;
    virtual void beginFrame() = 0;
    virtual void submit(const RenderQueue& queue) = 0;
    virtual void endFrame() = 0;
    virtual void shutdown() = 0;

    virtual TextureHandle uploadTexture(const TextureData& data) = 0;
    virtual void          releaseTexture(TextureHandle handle)   = 0;
};

} // ogs

#endif //OGSENGINE_IRENDERER_H