#include "PoolManager.h"

using namespace ogs;

PoolManager* PoolManager::instance = nullptr;

PoolManager* PoolManager::getInstance() {
    if (!instance){
        instance = new PoolManager;
        return instance;
    }
    return instance;
}


void PoolManager::reserveSizeInPool(size_t size, int rev_count){
    auto pool_pair = polls_map.find(size);
    if (pool_pair != polls_map.end()) {
        Pool* oldPool = pool_pair->second;
        delete oldPool;       
    }
    Pool* pool = new Pool(size, rev_count);
    polls_map[size] = pool;
}

void PoolManager::reserveSizeInAllocator(size_t size, int chunkCount){
    ALLOCATOR->createCustomPage(size, chunkCount);
}

void PoolManager::reserveSizeInPoolAndAllocator(size_t size,int rev_count, int chunkCount){
    ALLOCATOR->createCustomPage(size, chunkCount);
    reserveSizeInPool(size, rev_count);
}

