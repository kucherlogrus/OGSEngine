//
// Created by Logrus on 22.03.2026.
//

#ifndef OGSENGINE_IRENDERER_H
#define OGSENGINE_IRENDERER_H

namespace ogs {

class AppWindow;

class IRenderer {
public:
    virtual ~IRenderer() = default;

    virtual void init(AppWindow& window) = 0;
    virtual void beginFrame() = 0;
    virtual void endFrame() = 0;
    virtual void shutdown() = 0;
};

} // ogs

#endif //OGSENGINE_IRENDERER_H