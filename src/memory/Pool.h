#ifndef OGS_POOL_H
#define OGS_POOL_H

#include <vector>
#include <monitoring/Logsystem.h>
#include "Macroses.h"
#include "Allocator.h"

namespace ogs {

using namespace std;

class Pool {

    using pointer = void*;

public:

    Pool(size_t size_mem, int reserve_size = 8): size_mem(size_mem) {
        pool.reserve(reserve_size);
        for(int i = 0 ; i<reserve_size; i++){
            pointer ret = ALLOCATOR->alloc(size_mem);
            pool.push_back(ret);
        }
    };

    ~Pool() {
        free_memory();
    }

    template<typename T>
    T* obtain(T* type){
        if(sizeof(T) != size_mem) {
            logerror("Pool", "obtain", "error obtain", sizeof(T), size_mem);
        }
        if(pool.size() == 0) {
            type = static_cast<T*>(ALLOCATOR->alloc(size_mem));

        } else {
            type = static_cast<T*>(pool[pool.size() - 1]);
            pool.pop_back();
        }
        ::new(type) T;
        return type;
    };

    void release(pointer obj){
        pool.push_back(obj);
    };

    void free_memory() {
        for(pointer obj : pool) {
            ALLOCATOR->dealloc(obj, size_mem);
        }
        pool.clear();
    }

    void debug() {
        logdebug("Pool", "debug","pooling has:", pool.size(), "objects");
    }

private:
    Pool() = delete;
    Pool(const Pool& p) = delete;
    vector<pointer> pool;
    size_t size_mem;
};

};




#endif //GAME_POOL_H
