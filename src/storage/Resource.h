#ifndef RESOURCE_H
#define RESOURCE_H

#include "core/Reference.h"
#include "monitoring/Logsystem.h"

namespace ogs {

class Resource : public Reference {

public:
    Resource() = default;
    virtual ~Resource() = default;

    enum class ResType {
        TEXTURE = 1,
        MESH    = 2,
        FONT    = 3,
        ATLAS   = 4,
        NOTSUPPORT
    };

    virtual int getMask() = 0;
};


}
#endif
