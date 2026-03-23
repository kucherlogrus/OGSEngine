#ifndef OGS_RESOURCEHOLDER_H
#define OGS_RESOURCEHOLDER_H


#include <unordered_map>
#include <mutex>
#include "Resource.h"

namespace ogs {

class ResourceHolder {

private:
    std::mutex lock;
    std::unordered_map<string, Resource*> res_map;

public:
    ~ResourceHolder() { deleteAllResource(); }

    Resource* getResource(string name, bool addRef = true);
    bool disposeResource(string name);
    void addResource(string name, Resource* res);
    std::unordered_map<string, Resource*> getAllResources() { return res_map;};
    void deleteAllResource();
};

}
#endif
