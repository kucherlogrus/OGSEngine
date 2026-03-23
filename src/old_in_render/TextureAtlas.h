#ifndef OGS_TEXTURE_ATLAS_H
#define OGS_TEXTURE_ATLAS_H

#include <string>
#include <vector>
#include <unordered_map>
#include <storage/Resource.h>
#include "Texture.h"

using namespace std;

namespace ogs {

class AtlasSpriteFrame {
public:
    string frame_name;
    float frameWidth;
    float frameHeight;
    vector<float> x_frames;
    vector<float> y_frames;

    const vector<float>& getXframes(){ return x_frames;};
    const vector<float>& getYframes(){ return y_frames;};
};


class TextureAtlas : public Resource {

private:

    const int mask = T_ATLAS;

    Texture* texture;
    string atlasname;
    string texturename;
    unordered_map<string, AtlasSpriteFrame*> sp_frames;

public:
    TextureAtlas(): texture(nullptr){};
    ~TextureAtlas();
    AtlasSpriteFrame* getFrame(std::string);
    Texture* getTexture() { return texture; };
    string& getTextureName() { return texturename; };
    string& getName() { return atlasname; };
    void setTextureName(string&& name){ texturename = name; };
    void setTexture(Texture* tex) { texture = tex; };
    void setName(string name) { atlasname = name;};
    void addFrame(string name, AtlasSpriteFrame* frame);
    const unordered_map<string, AtlasSpriteFrame*>& getMap(){ return sp_frames;};
    virtual const int getMask() override { return mask; }

    

};

}
#endif
