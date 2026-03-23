#ifndef OGS_RESOURCECACHE_H
#define OGS_RESOURCECACHE_H


#include "storage/Mesh.h"
#include "storage/FreeTypeFont.h"
#include "storage/TextureAtlas.h"
#include "ResourceHolder.h"
#include "AssetManager.h"
#include "Resource.h"

namespace ogs {


class AssetManager;
class FreeTypeFont;

class ResourceCache {

public:
    ~ResourceCache();

    void init();

    void dispose() {};

    void showStats() {};

    size_t getMemorySize() { return cache_size; };

    ResourceHolder* getResourceHolderType(Resource::ResType);
    ResourceHolder* getResourceHolderMask(Resource*);

    void addResource(string name, Resource *, Resource::ResType);
    void addResource(string name, Resource *);

    Texture*         getTexture(string texturename);
    FreeTypeFont*    getFont(string fontname);
    TextureAtlas*    getAtlas(string atlasname);
    std::unordered_map<int, ResourceHolder*>& getHoldersMap() { return res_types_map;};

private:

    size_t cache_size = 0;
    std::mutex sizeLock;

    void changeSize(size_t, bool);

    void increaseSize(size_t size);

    void decreaseSize(size_t size);

    std::unordered_map<int, ResourceHolder*> res_types_map;
};

}
#endif //OGSENGINE_RESOURCECACHE_H
