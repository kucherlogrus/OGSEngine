#ifndef OGS_TEXTURE_H
#define OGS_TEXTURE_H

#include <iostream>
#include <storage/Resource.h>
#include "backends/GLConfig.h"
#include "Macroses.h"

namespace ogs {

struct TextureData {
        int width {0};
        int height {0};
        unsigned char *pixdata {nullptr};
        size_t pixlen {0};
        int format {0};
        std::string name {0};
};

class Texture : public Resource {

private:

    const int mask = T_TEXTURE ;
    std::string name {0};
    int width {0};
    int height {0};

    int textureID {0};

    int format {0};

    Texture() = default;

public:
    Texture(std::string, int, int, int);

    ~Texture() = default;

    std::string& getName() { return name; }

    int getWidth() { return width; }

    int getHeight() { return height; }

    void setTextureID(int id) { textureID = id; }

    int getTextureID() { return textureID; }

    int getFormat() { return format; }

    const int getMask() override { return mask; }

};


static std::ostream &operator<<(std::ostream &os, Texture* texture) {
        os << "name " << texture->getName().c_str() << ", " << "id " << texture->getTextureID() << ", ";
        os << "width " << texture->getWidth() << ", " << "height " << texture->getHeight();
        return os;
};

}


#endif

