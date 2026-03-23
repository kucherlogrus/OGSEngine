//
// Created by logrus on 20.01.17.
//

#ifndef POOLMANAGER_H
#define POOLMANAGER_H

#include "Pool.h"
#include <unordered_map>

namespace ogs {

class Reference;

class PoolManager {

private:

    static PoolManager *instance;

    unordered_map<int, Pool*> polls_map;

    Pool* getPool(int size) {
        auto pool_pair = polls_map.find(size);
        if (pool_pair != polls_map.end()) {
            return pool_pair->second;
        }
        Pool* pool = new Pool(size);
        polls_map[size] = pool;
        return pool;
    };

public:

    static PoolManager *getInstance();


    ~PoolManager() {
        for(auto pool_pair : polls_map) {
            delete pool_pair.second;
        }
    };

    //api //TODO - temporary off custom allocation
/*
    template<typename T>
    T* objectObtain(T* object) {
        int obj_size = sizeof(T);
        auto pool = getPool(obj_size);
        return pool->obtain(object);
    };

    template<typename T, typename Size>
    T* objectObtainBySize(T* object) {
        int obj_size = sizeof(T);
        auto pool = getPool(Size);
        return pool->obtain(object);
    };

    template<typename T, class = typename std::enable_if<is_base_of<Reference, T>::value>::type*>
    void objectRelease(T* object) {
        size_t obj_size = object->_getTypeSize();
        auto pool = getPool(obj_size);
        object->~T();
        pool->release(object);
    };

    template<typename T, class = typename std::enable_if<is_base_of<Reference, T>::value>::type*, typename Size>
    void objectReleaseBySize(T* object) {
        auto pool = getPool(Size);
        object->~T();
        pool->release(object);
    };
*/

    template<typename T>
    T* objectObtain(T* object) {
        T* obj = new T;
        return obj;
    };

    template<typename T>
    void objectRelease(T* object) {
        delete object;
    };


    void reserveSizeInPool(size_t size, int rev_count);
    void reserveSizeInAllocator(size_t size, int chunkCount);
    void reserveSizeInPoolAndAllocator(size_t size, int rev_count, int chunkCount);
};


};

#endif //POOLMANAGER_H
