#ifndef OGSENGINE_RENDEREXTRACTOR_H
#define OGSENGINE_RENDEREXTRACTOR_H

#include "RenderQueue.h"
#include "world/World.h"

namespace ogs {

// Stateless — extracts a RenderQueue from World state each frame.
// Owns no data: World is the authority, Renderer is the consumer.
class RenderExtractor {
public:
    // Performs frustum culling and builds RenderQueue for one frame.
    RenderQueue extract(const World& world) const;
};

} // ogs

#endif //OGSENGINE_RENDEREXTRACTOR_H