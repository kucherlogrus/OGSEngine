#include "storage/TextureAtlas.h"

using namespace ogs;

TextureAtlas::~TextureAtlas() {
    for (auto it = sp_frames.begin(); it != sp_frames.end(); ++it) {
        delete it->second;
    }
}

void TextureAtlas::addFrame(std::string name, AtlasSpriteFrame* frame) {
    sp_frames[name] = frame;
}

AtlasSpriteFrame* TextureAtlas::getFrame(std::string name) {
    auto res = sp_frames.find(name);
    if (res != sp_frames.end()) {
        return res->second;
    }
    return nullptr;
}
