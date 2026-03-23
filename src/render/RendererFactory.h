//
// Created by Logrus on 22.03.2026.
//

#ifndef OGSENGINE_RENDERERFACTORY_H
#define OGSENGINE_RENDERERFACTORY_H

#include <memory>
#include "IRenderer.h"

namespace ogs {

class RendererFactory {
public:
    static std::unique_ptr<IRenderer> create();
};

} // ogs

#endif //OGSENGINE_RENDERERFACTORY_H