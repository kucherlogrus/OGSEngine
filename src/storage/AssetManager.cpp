#include "storage/AssetManager.h"
#include "monitoring/Logsystem.h"

using namespace ogs;

AssetManager::AssetManager(std::unique_ptr<FileSystemUtil> fs)
    : loading(false)
    , iofiles(std::move(fs))
    , cache(std::make_unique<ResourceCache>())
    , atlasLoader(std::make_unique<AtlasLoader>())
{
    cache->init();
}

AssetManager::~AssetManager() = default;

void AssetManager::setTextureCallbacks(
    std::function<TextureHandle(const TextureData&)> upload,
    std::function<void(TextureHandle)>               release)
{
    onUploadTexture  = std::move(upload);
    onReleaseTexture = std::move(release);
}


Texture* AssetManager::genTexture(const string texturename, bool async = false){
    TextureData* data = nullptr;
    data = iofiles->loadImage(texturename);//TODO - maybe we need move load to another thread
    Texture* texture = new Texture(texturename, data->width, data->height);
    cache->addResource(texturename, texture, Resource::ResType::TEXTURE);
    if(async) {
        tex_data_queue.push(data);
    } else {
        texture->setHandle(onUploadTexture(*data));
        delete data;
    }
    return texture;
}


Texture* AssetManager::genTexture(string texturename, TextureData* t_data, bool async = false) {

    Texture* texture = new Texture(texturename, t_data->width, t_data->height);
    cache->addResource(texturename, texture, Resource::ResType::TEXTURE);
    if(async) {
        tex_data_queue.push(t_data);
    } else {
        texture->setHandle(onUploadTexture(*t_data));
        delete t_data;
    }
    return texture;
}


Texture* AssetManager::getTexture(const string texturename){

	Texture* texture = cache->getTexture(texturename);
    if (texture != nullptr) {
        return texture;
    } else {
        texture = genTexture(texturename, false);
        return texture;
    }
}


void AssetManager::disposeTexture(const string texturename){

    auto holder = cache->getResourceHolderType(Resource::ResType::TEXTURE);
    auto texture = static_cast<Texture*>(holder->getResource(texturename, false));
    if(texture != nullptr){
        TextureHandle handle = texture->getHandle();
        bool release = holder->disposeResource(texturename);
        if(release && handle.isValid()) {
            onReleaseTexture(handle);
        }
    }
}



TextureAtlas* AssetManager::getAtlas(string atlasname) {
    TextureAtlas* atlas = cache->getAtlas(atlasname);
    if (atlas == nullptr) {
        atlas = genAtlas(atlasname, false);
        return atlas; 
    }
    return atlas;
}


TextureAtlas* AssetManager::genAtlas(string atlasname, bool async = false) {
    unique_ptr<FileData> data(iofiles->getDataFromFile(atlasname));
    TextureAtlas* atlas = atlasLoader->genAtlas(atlasname, data.get());
    atlas->setName(atlasname);
    cache->addResource(atlasname, atlas, Resource::ResType::ATLAS);
    Texture* texture = genTexture(atlas->getTextureName(), async);
    atlas->setTexture(texture);
    return atlas;
}


void AssetManager::disposeAtlas(string atlasName){
    auto holder = cache->getResourceHolderType(Resource::ResType::ATLAS);
    auto atlas = static_cast<TextureAtlas*>(holder->getResource(atlasName, false));
    if(atlas != nullptr) {
        Texture* texture = atlas->getTexture();
        bool release = holder->disposeResource(atlasName);
        if(release) {
            disposeTexture(texture->getName());
        }
    }   
}


void AssetManager::invalidateGLData() {
    // TODO: implement via renderer event on context loss (Android)
}


size_t AssetManager::getMemoryInCache(){
    return cache->getMemorySize();
}


Mesh* AssetManager::createMeshSimple(){
    auto mesh = new Mesh;
    auto meshHolder = cache->getResourceHolderType(Resource::ResType::MESH);
    meshHolder->addResource("simple", mesh);
    return mesh;
}


Mesh* AssetManager::createMesh(string name){
    auto mesh = new Mesh;
    auto meshHolder = cache->getResourceHolderType(Resource::ResType::MESH);
    meshHolder->addResource(name, mesh);
    return mesh;
}



void AssetManager::loadResourcesList(vector<ResourceEntry> resList) {
    for (auto& resource : resList) {
        switch (resource.type) {
            case Resource::ResType::TEXTURE:
                genTexture(resource.res_name, true);
                break;
            case Resource::ResType::ATLAS:
                genAtlas(resource.res_name, true);
                break;
            case Resource::ResType::MESH:
                break;
            default:
                logdebug("AssetManager", "loadResourcesList", resource.res_name.c_str(), "can't load");
                break;
        }
    }
    loading = false;
    loginfo("AssetManager", "loadResourcesList", "loading:", "false");
}

void AssetManager::loadResourcesListAsync(vector<ResourceEntry>&& resList) {
    loginfo("AssetManager", "loadResourcesListAsync", "loading:", "true");
    loading = true;
    // TODO: submit via ThreadPoolExecutor once it is injected into AssetManager
    loadResourcesList(resList);
}

void AssetManager::disposeResourcesList(initializer_list<ResourceEntry>&& resList) {
    for (auto& resource : resList) {
        switch (resource.type) {
            case Resource::ResType::TEXTURE:
                disposeTexture(resource.res_name);
                break;
            case Resource::ResType::ATLAS:
                disposeAtlas(resource.res_name);
                break;
            case Resource::ResType::MESH:
                break;
            default:
                logdebug("AssetManager", "disposeResourcesList", resource.res_name.c_str(), "can't dispose");
                break;
        }
    }
}


bool AssetManager::checkReady() {
	
    if(loading.load() || !tex_data_queue.isEmpty()) {
        TextureData* t_data = tex_data_queue.popNoWait();
        if(t_data != nullptr){
            Texture* texture = cache->getTexture(t_data->name);
            texture->setHandle(onUploadTexture(*t_data));
            delete t_data;
        }
        return false;
    }
    return true;
}

void AssetManager::debug() {
    logdebug("AssetManager", "debug");
    unordered_map<int, ResourceHolder*>& map = cache->getHoldersMap();
    for (auto& holder : map) {
        for (auto& entry : holder.second->getAllResources()) {
            int mask = entry.second->getMask();
            if (mask == static_cast<int>(Resource::ResType::TEXTURE)) {
                auto texture = static_cast<Texture*>(entry.second);
                logdebug("AM", "texture:", texture->getName().c_str(),
                         "handle:", texture->getHandle().id,
                         "refs:", texture->referencesCount());
            }
            if (mask == static_cast<int>(Resource::ResType::MESH)) {
                auto mesh = static_cast<Mesh*>(entry.second);
                logdebug("AM", "mesh", "index_size:", mesh->getIndices().size());
            }
            if (mask == static_cast<int>(Resource::ResType::FONT)) {
                auto font = static_cast<FreeTypeFont*>(entry.second);
                logdebug("AM", "font", "name:", font->fontname, "refs:", font->referencesCount());
            }
            if (mask == static_cast<int>(Resource::ResType::ATLAS)) {
                auto atlas = static_cast<TextureAtlas*>(entry.second);
                logdebug("AM", "atlas", "name:", entry.first, "refs:", atlas->referencesCount());
            }
        }
    }
}





