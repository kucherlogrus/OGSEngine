//
// Created by Logrus on 22.03.2026.
//

#include "RendererFactory.h"

#ifdef MACOS
#include "metal/MetalRenderer.h"
#endif

namespace ogs {

std::unique_ptr<IRenderer> RendererFactory::create() {
#ifdef MACOS
    return std::make_unique<MetalRenderer>();
#else
    return nullptr;
#endif
}

} // ogs