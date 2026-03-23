
#ifndef OGS_REFERENCE_H
#define OGS_REFERENCE_H

#include <atomic>
#include "memory/memory.h"

namespace ogs {

class Reference {

private:
    std::atomic<int> reference{0};

public:

    Reference() : reference(1) {};
    Reference(const Reference&)            = delete;
    Reference& operator=(const Reference&) = delete;

    virtual ~Reference() = default;

    void addReference(){
        reference.fetch_add(1, std::memory_order_relaxed);
    };

    void releaseReference() {
        if (reference.fetch_sub(1, std::memory_order_acq_rel) == 1) {
            if(isPoolable()){
                PoolManager::getInstance()->objectRelease(this);
            } else {
                delete this;
            }
        }
    };

    virtual size_t _getTypeSize() {
        return sizeof(*this);
    };

    [[nodiscard]] int referencesCount() const { return reference.load(std::memory_order_relaxed); };

    virtual bool isPoolable() { return false; };

};


};

#endif //REFERENCE_H
