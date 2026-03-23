#include "World.h"

#include <algorithm>
#include "monitoring/Logsystem.h"

namespace ogs {

World::World() {
    camera.setPerspective(60.0f, 16.0f / 9.0f, 0.1f, 1000.0f);
    camera.lookAt({0.0f, 0.0f, 3.0f}, {0.0f, 0.0f, 0.0f});
    // To switch to orthographic:
    // camera.setOrthographic(5.0f, 16.0f / 9.0f, 0.1f, 1000.0f);
    loginfo("World", "World created");
}

void World::update(float dt) {
    // Future: physics step, animation, AI, etc.
    (void)dt;
}

void World::dispose() {
    objects.clear();
    loginfo("World", "World disposed");
}

GameObject& World::spawn(const std::string& name) {
    auto obj = std::make_unique<GameObject>(nextId++, name.empty() ? "GameObject" : name);
    auto& ref = *obj;
    objects.push_back(std::move(obj));
    loginfo("World", "spawn", ref.name.c_str(), ref.id);
    return ref;
}

void World::destroy(uint32_t id) {
    auto it = std::find_if(objects.begin(), objects.end(),
        [id](const std::unique_ptr<GameObject>& o) { return o->id == id; });
    if (it != objects.end()) {
        loginfo("World", "destroy", (*it)->name.c_str(), id);
        objects.erase(it);
    }
}

} // ogs