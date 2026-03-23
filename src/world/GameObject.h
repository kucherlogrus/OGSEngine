#ifndef OGSENGINE_GAMEOBJECT_H
#define OGSENGINE_GAMEOBJECT_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <cstdint>

namespace ogs {

// Placeholder handles — will be replaced by real asset handles later
using MeshHandle     = uint32_t;
using MaterialHandle = uint32_t;

static constexpr MeshHandle     NULL_MESH     = 0;
static constexpr MaterialHandle NULL_MATERIAL = 0;

struct Transform {
    glm::vec3 position = {0.0f, 0.0f, 0.0f};
    glm::vec3 rotation = {0.0f, 0.0f, 0.0f}; // Euler angles, degrees
    glm::vec3 scale    = {1.0f, 1.0f, 1.0f};

    glm::mat4 matrix() const {
        glm::mat4 m = glm::mat4(1.0f);
        m = glm::translate(m, position);
        m = glm::rotate(m, glm::radians(rotation.y), {0,1,0});
        m = glm::rotate(m, glm::radians(rotation.x), {1,0,0});
        m = glm::rotate(m, glm::radians(rotation.z), {0,0,1});
        m = glm::scale(m, scale);
        return m;
    }
};

struct BoundingSphere {
    glm::vec3 center = {0.0f, 0.0f, 0.0f}; // local space center
    float     radius = 1.0f;

    // Returns world-space center given a transform
    glm::vec3 worldCenter(const Transform& t) const {
        return t.position + center;
    }

    float worldRadius(const Transform& t) const {
        float maxScale = glm::max(glm::max(t.scale.x, t.scale.y), t.scale.z);
        return radius * maxScale;
    }
};

struct GameObject {
    uint32_t       id       = 0;
    std::string    name;
    Transform      transform;
    BoundingSphere bounds;
    MeshHandle     mesh     = NULL_MESH;
    MaterialHandle material = NULL_MATERIAL;
    bool           visible  = true;   // manual hide/show
    bool           isStatic = false;  // hint for future batching/BVH

    explicit GameObject(uint32_t id, std::string name = ""): id(id), name(std::move(name)) {}
};

} // ogs

#endif //OGSENGINE_GAMEOBJECT_H