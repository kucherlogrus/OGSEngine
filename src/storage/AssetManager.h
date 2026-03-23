#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include <iostream>
#include <atomic>
#include <functional>
#include <concurrent/Queue.h>
#include "storage/Texture.h"
#include "storage/Mesh.h"
#include "storage/FileSystemUtil.h"
#include "storage/ResourceCache.h"
#include "storage/AtlasLoader.h"
#include "render/TextureHandle.h"
#include "json.hpp"

namespace ogs {

using namespace std;



class FileSystemUtil;
class ResourceCache;
class AtlasLoader;

struct ResourceEntry {
    string res_name;
    Resource::ResType type;
};

class AssetManager {

private:
    std::unique_ptr<FileSystemUtil> iofiles;
    std::unique_ptr<ResourceCache>  cache;
    std::unique_ptr<AtlasLoader>    atlasLoader;
    atomic<bool> loading;
    Queue<TextureData> tex_data_queue;
    std::function<TextureHandle(const TextureData&)> onUploadTexture;
    std::function<void(TextureHandle)>               onReleaseTexture;

public:
    explicit AssetManager(std::unique_ptr<FileSystemUtil> fs);
    ~AssetManager();

    //Recources api
    //TODO - add method enTextureAsync
    Texture* genTexture(string name, bool async);
    Texture* genTexture(string name, TextureData* t_data, bool async);
    Texture* getTexture(string name);
    void disposeTexture(string name);


    // TODO: font methods require FreeTypeManager migration from old_in_render
    // FreeTypeFont* genFont(string name, int size, int dpi, bool async);
    // FreeTypeFont* getFont(string name);
    // void disposeFont(string name);


    Mesh* createMeshSimple();
    Mesh* createMesh(string name);

    TextureAtlas* getAtlas(string);
    TextureAtlas* genAtlas(string name, bool async);
    void disposeAtlas(string name);


    //Loaders api
    void loadResourcesList(vector<ResourceEntry> resList);
    void loadResourcesListAsync(vector<ResourceEntry>&& resList);
    void disposeResourcesList(initializer_list<ResourceEntry>&& resList);


    void setTextureCallbacks(
        std::function<TextureHandle(const TextureData&)> upload,
        std::function<void(TextureHandle)>               release);

    //Util api
    void invalidateGLData();
    size_t getMemoryInCache();

    bool checkReady();

    void debug();
};

}

#endif
