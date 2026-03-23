//
// Created by Logrus on 22.03.2026.
//

#ifndef OGSENGINE_OGSENGINE_H
#define OGSENGINE_OGSENGINE_H
#include <memory>
#include "backends/AppWindow.h"
#include "concurrent/ThreadPoolExecutor.h"
#include "core/Timer.h"
#include "memory/PoolManager.h"
#include "monitoring/FPSCounter.h"
#include "input/Input.h"


namespace ogs {

class IRenderer;

enum class AppState {
    RUN, PAUSE, CLOSE
};

enum class AppTYPE {
    EDITOR, GAME
};

class OGSEngine {

private:
    FPSCounter counter;
    timer::game_timer timer;
    AppState state = AppState::RUN;
    AppTYPE apptype = AppTYPE::GAME;
    // Renderer *renderer;
    // AssetManager *assets;
    // LogicDispatcher* logicDispatcher;
    ThreadPoolExecutor* executor;
    // Scene* currentScene;
    // Scene* nextScene;
    PoolManager* poolManager;
    std::unique_ptr<AppWindow> windowManager;
    std::unique_ptr<IRenderer> renderer;
    std::unique_ptr<Input> inputHandler;
    bool init;
    void gameMainloop();
    void editorMainloop();

public:
    OGSEngine();
    ~OGSEngine();

    // Scene *getScene() { return currentScene; }


    bool isInit(){return init;};

    FPSCounter &getCounter() { return counter; };

    ThreadPoolExecutor* getExecutor() { return executor;};

    PoolManager* getPoolManager() { return poolManager; };

    // LogicDispatcher* getLogicDispatcher() { return logicDispatcher;}

    void changeState(AppState newState) { state = newState; };

    void requestloop();

    void tick();

    void initCoreSystems(int=-1, int=-1);

    void checkScenes();

    void invalidateGL();

    void resizeWindow(float w, float h);

};
}
#endif //OGSENGINE_OGSENGINE_H