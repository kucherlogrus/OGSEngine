#include <string>
#include "ResourceHolder.h"

using namespace ogs;

void ResourceHolder::deleteAllResource() {
    std::lock_guard guard(lock);
    for( auto it = res_map.begin(); it != res_map.end(); ) {
        delete it->second;
        it = res_map.erase(it);
    }
};

Resource* ResourceHolder::getResource(string name, bool addRef) {
    std::lock_guard guard(lock);
    auto res = res_map.find(name);
    if (res != res_map.end()) {
        if(addRef) {
            res->second->addReference();
        }
        return res->second;
    }
    return nullptr;
}

void ResourceHolder::addResource(string name, Resource* res) {
    std::lock_guard guard(lock);
    res_map[name] = res;
}

bool ResourceHolder::disposeResource(string name){
    std::lock_guard guard(lock);
    auto it = res_map.find(name);
    if (it == res_map.end()) {
        return false;
    }
    Resource* res = it->second;
    if (res->referencesCount() == 1) {
        res_map.erase(it);
        res->releaseReference(); // safe: map no longer holds the pointer
        return true;
    }
    res->releaseReference();
    return false;
};
