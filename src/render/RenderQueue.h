#ifndef OGSENGINE_RENDERQUEUE_H
#define OGSENGINE_RENDERQUEUE_H

#include <vector>
#include <variant>
#include <cstdint>
#include <glm/glm.hpp>

#include "world/GameObject.h"
#include "render/TextureHandle.h"

namespace ogs {

// -----------------------------------------------------------------------
// 3D mesh command
// -----------------------------------------------------------------------
struct Mesh3DCommand {
    MeshHandle     mesh;
    MaterialHandle material;
    glm::mat4      transform;
};

// -----------------------------------------------------------------------
// 2D sprite batch command
// One command == one draw call (all sprites share a single texture).
// SpriteBatcher accumulates quads and flushes a new command on texture change.
// -----------------------------------------------------------------------
struct SpriteVertex {
    float x, y;   // screen-space position
    float u, v;   // texture coordinates
    float alpha;
};

struct SpriteBatchCommand {
    TextureHandle            texture;
    std::vector<SpriteVertex> vertices; // 4 vertices per sprite
    std::vector<uint32_t>    indices;   // 6 indices per sprite (two triangles)
    glm::mat4                projection;
};

// -----------------------------------------------------------------------
// Unified render command
// -----------------------------------------------------------------------
struct RenderCommand {
    std::variant<Mesh3DCommand, SpriteBatchCommand> data;
    float distanceToCamera = 0.0f; // used for sorting
};

// -----------------------------------------------------------------------
// RenderQueue
// -----------------------------------------------------------------------
struct RenderQueue {
    std::vector<RenderCommand> opaque;      // sorted front-to-back (early-z)
    std::vector<RenderCommand> transparent; // sorted back-to-front (blending)

    void clear() {
        opaque.clear();
        transparent.clear();
    }

    bool empty() const {
        return opaque.empty() && transparent.empty();
    }
};

} // namespace ogs

#endif //OGSENGINE_RENDERQUEUE_H