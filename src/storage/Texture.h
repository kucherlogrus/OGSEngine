#ifndef OGS_TEXTURE_H
#define OGS_TEXTURE_H

#include <string>
#include "storage/Resource.h"
#include "render/PixelFormat.h"
#include "render/TextureHandle.h"

namespace ogs {

struct TextureData {
    int width          {0};
    int height         {0};
    unsigned char* pixdata {nullptr};
    size_t pixlen      {0};
    PixelFormat format {PixelFormat::Unknown};
    std::string name;

    ~TextureData() {
        free(pixdata);
    }
};

class Texture : public Resource {
private:
    const int mask = 0x01; // T_TEXTURE
    std::string name;
    int width    {0};
    int height   {0};
    TextureHandle handle;

    Texture() = default;

public:
    Texture(std::string name, int w, int h);
    ~Texture() = default;

    std::string& getName()                      { return name; }
    int getWidth()                              { return width; }
    int getHeight()                             { return height; }
    void setHandle(TextureHandle h)             { handle = h; }
    TextureHandle getHandle()                   { return handle; }
    int getMask() override                      { return mask; }
};

} // namespace ogs

#endif
