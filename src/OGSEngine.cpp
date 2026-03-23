
#include "OGSEngine.h"

#include "render/RendererFactory.h"
#include "render/RenderQueue.h"

#if defined(MACOS) || defined(WINDOWS) || defined(LINUX)
#include "backends/desktop/window.h"
#include "backends/desktop/DesktopFileSystem.h"
#endif

using namespace ogs;

OGSEngine::OGSEngine():
 // renderer(nullptr),
 // assets(nullptr),
 // currentScene(nullptr),
 // nextScene(nullptr),
 // logicDispatcher(nullptr),
 executor(nullptr),
 poolManager(nullptr),
 init(false)
{
    loginfo("OGSEngine","OGSEngine construct");
#if defined(MACOS) || defined(WINDOWS) || defined(LINUX)
    windowManager = std::make_unique<DesktopWindow>();
#endif
    Allocator::getInstance();

}

OGSEngine::~OGSEngine(){
    loginfo("OGSEngine","OGSEngine destruct");
    delete executor;
    delete poolManager;
}

void OGSEngine::initCoreSystems(int width, int height){
    inputHandler = std::make_unique<Input>();
    windowManager->createWindow(width, height);
    windowManager->setInputHandler(inputHandler.get());
    executor = new ThreadPoolExecutor(2);
    Allocator::getInstance();
    poolManager = PoolManager::getInstance();
    world    = std::make_unique<World>();
    renderer = RendererFactory::create();
    if (renderer) renderer->init(*windowManager);

#if defined(MACOS) || defined(WINDOWS) || defined(LINUX)
    assetManager = std::make_unique<AssetManager>(std::make_unique<DesktopFileSystem>());
#endif
    if (assetManager && renderer) {
        assetManager->setTextureCallbacks(
            [this](const TextureData& d) { return renderer->uploadTexture(d); },
            [this](TextureHandle h)      { renderer->releaseTexture(h); }
        );
    }

    counter.setActiveLogging(false);
}


void OGSEngine::tick(){
    windowManager->pollEvents();
    if (windowManager->shouldClose()) {
        state = AppState::CLOSE;
        return;
    }
    inputHandler->inputProcess();
    timer::update_timer(timer);
    world->update(timer.msdelta);
    RenderQueue queue = extractor.extract(*world);
    renderer->beginFrame();
    renderer->submit(queue);
    renderer->endFrame();
    counter.update(timer.msdelta);
}

void OGSEngine::editorMainloop() {
    std::thread refresh_thread;
    refresh_thread = std::thread(
            [&]() {
                std::chrono::milliseconds time(50);
                while (state == AppState::RUN) {
                    std::this_thread::sleep_for(time);
                    // glfwPostEmptyEvent();
                }
            }
    );
    while(state != AppState::CLOSE) {
        // Input::getInstance()->inputProcess();
        // timer::update_timer(timer);
        // currentScene->update(timer.msdelta);
        // renderer->clear();
        // currentScene->render(renderer);
        // renderer->flush();
        // renderer->swap();
        // counter.update(timer.msdelta);
        // glfwWaitEvents();
    }
    refresh_thread.join();

};

void OGSEngine::gameMainloop() {
    while(true){
        switch (state) {
            case AppState::RUN:
                tick();
                break;
            case AppState::CLOSE:
                return;
            default:
                break;
        }
    }
};


void OGSEngine::requestloop(){
    return gameMainloop();
}

void OGSEngine::invalidateGL(){
    // AssetManager::getInstance()->invalidateGLData();
};

void OGSEngine::resizeWindow(float w, float h) {
    loginfo("OGSEngine", "resizeWindow", w, h);
    // renderer->resize(w,h);
    // if (currentScene != nullptr){
    //     //currentScene->getCamera()->resizeEvent(w,h);
    //     currentScene->getCamera()->resizeEvent();
    // }
}


