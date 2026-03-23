#ifndef OGSENGINE_WORLD_H
#define OGSENGINE_WORLD_H

#include <vector>
#include <memory>
#include <cstdint>

#include "Camera.h"
#include "GameObject.h"

namespace ogs {

class World {
public:
    World();
    ~World() = default;

    // Lifecycle
    void update(float dt);
    void dispose();

    // Object management
    GameObject& spawn(const std::string& name = "");
    void        destroy(uint32_t id);

    const std::vector<std::unique_ptr<GameObject>>& getObjects() const { return objects; }

    // Camera
    Camera&       getCamera()       { return camera; }
    const Camera& getCamera() const { return camera; }

private:
    Camera camera;
    std::vector<std::unique_ptr<GameObject>> objects;
    uint32_t nextId = 1;
};

} // ogs

#endif //OGSENGINE_WORLD_H