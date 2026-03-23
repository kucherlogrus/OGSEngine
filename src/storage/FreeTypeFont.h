#ifndef OGS_FONT_H
#define OGS_FONT_H

#include <string>
#include "storage/Resource.h"
#include "storage/Texture.h"

namespace ogs {

class FreeTypeFont : public Resource {
public:
    const int mask = 0x03; // T_FONT
    std::string fontname;
    std::string fontUrl;
    Texture* font_texture {nullptr};
    float advance[128] {};
    float w[128]       {};
    float h[128]       {};
    float u1[128]      {};
    float u2[128]      {};
    float v1[128]      {};
    float v2[128]      {};
    float off_x[128]   {};
    float off_y[128]   {};
    float color {0};
    float size  {0};

    int getMask() override { return mask; }
};

} // namespace ogs

#endif
