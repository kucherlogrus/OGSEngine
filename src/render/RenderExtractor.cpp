#include "RenderExtractor.h"

#include <algorithm>
#include <glm/glm.hpp>

namespace ogs {

RenderQueue RenderExtractor::extract(const World& world) const {
    RenderQueue queue;
    const Camera&  camera  = world.getCamera();
    const Frustum& frustum = camera.getFrustum();
    const glm::vec3& camPos = camera.getPosition();

    for (const auto& objPtr : world.getObjects()) {
        const GameObject& obj = *objPtr;

        // Skip manually hidden objects and objects without mesh
        if (!obj.visible || obj.mesh == NULL_MESH)
            continue;

        // Frustum culling via bounding sphere
        glm::vec3 worldCenter = obj.bounds.worldCenter(obj.transform);
        float     worldRadius = obj.bounds.worldRadius(obj.transform);

        if (!frustum.containsSphere(worldCenter, worldRadius))
            continue;

        float distSq = glm::dot(worldCenter - camPos, worldCenter - camPos);

        RenderCommand cmd;
        cmd.data = Mesh3DCommand{
            .mesh      = obj.mesh,
            .material  = obj.material,
            .transform = obj.transform.matrix(),
        };
        cmd.distanceToCamera = distSq;

        // TODO: determine transparency from material
        // For now all objects go to opaque
        queue.opaque.push_back(std::move(cmd));
    }

    // Opaque: front-to-back (minimizes overdraw via early-z)
    std::sort(queue.opaque.begin(), queue.opaque.end(),
        [](const RenderCommand& a, const RenderCommand& b) {
            return a.distanceToCamera < b.distanceToCamera;
        });

    // Transparent: back-to-front (correct alpha blending)
    std::sort(queue.transparent.begin(), queue.transparent.end(),
        [](const RenderCommand& a, const RenderCommand& b) {
            return a.distanceToCamera > b.distanceToCamera;
        });

    return queue;
}

} // ogs