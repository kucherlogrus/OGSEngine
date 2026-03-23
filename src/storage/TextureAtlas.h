#ifndef OGS_TEXTURE_ATLAS_H
#define OGS_TEXTURE_ATLAS_H

#include <string>
#include <vector>
#include <unordered_map>
#include "storage/Resource.h"
#include "storage/Texture.h"

namespace ogs {

class AtlasSpriteFrame {
public:
    std::string frame_name;
    float frameWidth  {0};
    float frameHeight {0};
    std::vector<float> x_frames;
    std::vector<float> y_frames;

    const std::vector<float>& getXframes() { return x_frames; }
    const std::vector<float>& getYframes() { return y_frames; }
};


class TextureAtlas : public Resource {
private:
    const int mask = 0x04; // T_ATLAS
    Texture* texture {nullptr};
    std::string atlasname;
    std::string texturename;
    std::unordered_map<std::string, AtlasSpriteFrame*> sp_frames;

public:
    TextureAtlas() = default;
    ~TextureAtlas();

    AtlasSpriteFrame* getFrame(std::string name);
    void addFrame(std::string name, AtlasSpriteFrame* frame);

    Texture* getTexture()                       { return texture; }
    std::string& getTextureName()               { return texturename; }
    std::string& getName()                      { return atlasname; }
    void setTextureName(std::string&& name)     { texturename = std::move(name); }
    void setTexture(Texture* tex)               { texture = tex; }
    void setName(std::string name)              { atlasname = name; }
    const std::unordered_map<std::string, AtlasSpriteFrame*>& getMap() { return sp_frames; }
    int getMask() override                      { return mask; }
};

} // namespace ogs

#endif
