#include "storage/ResourceCache.h"

using namespace ogs;

ResourceCache::~ResourceCache() {
    for (auto& pair : res_types_map) {
        delete pair.second;
    }
}

void ResourceCache::init(){
    res_types_map[static_cast<int>(Resource::ResType::TEXTURE)] = new ResourceHolder;
    res_types_map[static_cast<int>(Resource::ResType::MESH)]    = new ResourceHolder;
    res_types_map[static_cast<int>(Resource::ResType::FONT)]    = new ResourceHolder;
    res_types_map[static_cast<int>(Resource::ResType::ATLAS)]   = new ResourceHolder;
}

void ResourceCache::addResource(string  name, Resource* res , Resource::ResType type){
    auto holder = getResourceHolderType(type);
    holder->addResource(name,res);
}

void ResourceCache::addResource(string  name, Resource* res){
    auto holder = getResourceHolderMask(res);
    holder->addResource(name,res);
}


ResourceHolder* ResourceCache::getResourceHolderType(Resource::ResType type) {
    int int_type = static_cast<int>(type);
    auto holder = res_types_map.find(int_type);
    return holder->second;
}

ResourceHolder* ResourceCache::getResourceHolderMask(Resource* res) {
    auto holder = res_types_map.find(res->getMask());
    return holder->second;
}

void ResourceCache::changeSize(size_t size , bool add){
    lock_guard<mutex> guard(sizeLock);
    add ? cache_size += size : cache_size -= size;
}

void ResourceCache::increaseSize(size_t size) {
    lock_guard<mutex> guard(sizeLock);
    cache_size += size;
}

void ResourceCache::decreaseSize(size_t size) {
    lock_guard<mutex> guard(sizeLock);
    cache_size -= size;
}

Texture* ResourceCache::getTexture(string texturename){
    auto holder = res_types_map[static_cast<int>(Resource::ResType::TEXTURE)];
    auto resource = holder->getResource(texturename);
    if(resource == nullptr) {
        return nullptr;
    }
    return static_cast<Texture*>(resource);
}

FreeTypeFont* ResourceCache::getFont(string fontname){
    auto holder = res_types_map[static_cast<int>(Resource::ResType::FONT)];
    auto resource = holder->getResource(fontname);
    if(resource == nullptr) {
        return nullptr;
    }
    return static_cast<FreeTypeFont*>(resource);
}

TextureAtlas* ResourceCache::getAtlas(string atlasname) {
    auto holder = res_types_map[static_cast<int>(Resource::ResType::ATLAS)];
    auto resource = holder->getResource(atlasname);
    if(resource == nullptr) {
        return nullptr;
    }
    return static_cast<TextureAtlas*>(resource);
}




