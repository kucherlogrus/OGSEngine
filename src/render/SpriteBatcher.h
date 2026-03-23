#ifndef OGSENGINE_SPRITEBATCHER_H
#define OGSENGINE_SPRITEBATCHER_H

#include <cstdint>
#include <vector>
#include <glm/glm.hpp>

#include "render/RenderQueue.h"
#include "render/TextureHandle.h"

namespace ogs {

// CPU-side 2D sprite batcher.
// Accumulates quads per-texture, flushes a SpriteBatchCommand into RenderQueue
// when the texture changes or end() is called.
//
// Usage per frame:
//   batcher.begin(projectionMatrix);
//   batcher.draw(tex, x, y, w, h);
//   batcher.draw(tex, x, y, w, h, u1, v1, u2, v2, alpha);
//   batcher.end(queue);  // appends to queue.transparent
class SpriteBatcher {
public:
    // Full draw call — explicit UV region and alpha
    void draw(TextureHandle texture,
              float x, float y, float width, float height,
              float u1, float v1, float u2, float v2,
              float alpha = 1.0f);

    // Convenience: full texture, no region
    void draw(TextureHandle texture,
              float x, float y, float width, float height,
              float alpha = 1.0f);

    void begin(const glm::mat4& projection);

    // Flush remaining sprites into queue.transparent
    void end(RenderQueue& queue);

private:
    glm::mat4    projection;
    TextureHandle currentTexture{};

    std::vector<SpriteVertex>      vertices;
    std::vector<uint32_t>          indices;
    std::vector<SpriteBatchCommand> pendingBatches;
    uint32_t                       vertexCount = 0;

    void flush(RenderQueue& queue);
    void pushQuad(float x, float y, float w, float h,
                  float u1, float v1, float u2, float v2,
                  float alpha);
};

} // namespace ogs

#endif //OGSENGINE_SPRITEBATCHER_H