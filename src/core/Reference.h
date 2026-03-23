
#ifndef OGS_REFERENCE_H
#define OGS_REFERENCE_H

#include "memory/memory.h"

namespace ogs {

class Reference {

private:
    int reference = 0;

public:

    Reference() : reference(1) {};

    virtual ~Reference() = default;

    void addReference(){
        ++reference;
    };

    void releaseReference() {
        --reference;
        if (reference == 0) {
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

    [[nodiscard]] int referencesCount() const { return reference; };

    virtual bool isPoolable() { return false; };

};


};

#endif //REFERENCE_H
