#ifndef OGS_TEXTURE_HANDLE_H
#define OGS_TEXTURE_HANDLE_H

#include <cstdint>

namespace ogs {

struct TextureHandle {
    uint32_t id {0};
    bool isValid() const { return id != 0; }
};

} // namespace ogs

#endif
