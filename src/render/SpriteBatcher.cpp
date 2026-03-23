#include "SpriteBatcher.h"

namespace ogs {

void SpriteBatcher::begin(const glm::mat4& proj) {
    projection     = proj;
    vertexCount    = 0;
    currentTexture = TextureHandle{};
    vertices.clear();
    indices.clear();
    pendingBatches.clear();
}

void SpriteBatcher::draw(TextureHandle texture,
                         float x, float y, float width, float height,
                         float u1, float v1, float u2, float v2,
                         float alpha)
{
    if (!currentTexture.isValid()) {
        currentTexture = texture;
    }

    if (currentTexture.id != texture.id) {
        // Texture changed — flush current accumulation into pending list
        pendingBatches.push_back(SpriteBatchCommand{
            .texture    = currentTexture,
            .vertices   = vertices,
            .indices    = indices,
            .projection = projection,
        });
        vertices.clear();
        indices.clear();
        vertexCount    = 0;
        currentTexture = texture;
    }

    pushQuad(x, y, width, height, u1, v1, u2, v2, alpha);
}

void SpriteBatcher::draw(TextureHandle texture,
                         float x, float y, float width, float height,
                         float alpha)
{
    draw(texture, x, y, width, height, 0.0f, 0.0f, 1.0f, 1.0f, alpha);
}

void SpriteBatcher::end(RenderQueue& queue) {
    // Flush whatever remains in the current accumulation
    if (vertexCount > 0 && currentTexture.isValid()) {
        pendingBatches.push_back(SpriteBatchCommand{
            .texture    = currentTexture,
            .vertices   = vertices,
            .indices    = indices,
            .projection = projection,
        });
    }

    // Push all batches into the transparent queue (sprites are alpha-blended)
    for (auto& batch : pendingBatches) {
        RenderCommand cmd;
        cmd.data             = std::move(batch);
        cmd.distanceToCamera = 0.0f;
        queue.transparent.push_back(std::move(cmd));
    }

    pendingBatches.clear();
    vertices.clear();
    indices.clear();
    vertexCount    = 0;
    currentTexture = TextureHandle{};
}

void SpriteBatcher::pushQuad(float x, float y, float w, float h,
                              float u1, float v1, float u2, float v2,
                              float alpha)
{
    uint32_t base = vertexCount;

    vertices.push_back({ x,     y,     u1, v1, alpha });
    vertices.push_back({ x + w, y,     u2, v1, alpha });
    vertices.push_back({ x + w, y + h, u2, v2, alpha });
    vertices.push_back({ x,     y + h, u1, v2, alpha });

    indices.push_back(base + 0);
    indices.push_back(base + 1);
    indices.push_back(base + 2);
    indices.push_back(base + 0);
    indices.push_back(base + 2);
    indices.push_back(base + 3);

    vertexCount += 4;
}

} // namespace ogs