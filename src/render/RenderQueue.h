#ifndef OGSENGINE_RENDERQUEUE_H
#define OGSENGINE_RENDERQUEUE_H

#include <vector>
#include <cstdint>
#include <glm/glm.hpp>

#include "world/GameObject.h"

namespace ogs {

struct RenderCommand {
    MeshHandle     mesh;
    MaterialHandle material;
    glm::mat4      transform;
    float          distanceToCamera; // used for sorting
};

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

} // ogs

#endif //OGSENGINE_RENDERQUEUE_H