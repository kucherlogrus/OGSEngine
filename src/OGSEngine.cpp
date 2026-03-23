
#include "OGSEngine.h"


#include "render/RendererFactory.h"

#if defined(MACOS) || defined(WINDOWS) || defined(LINUX)
#include "backends/desktop/window.h"
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
    // if(currentScene != nullptr) {
    //     logicDispatcher->clear();
    //     currentScene->dispose();
    //
    //     delete currentScene;
    // }
    //
    // if( nextScene != nullptr) {
    //     delete nextScene;
    // }
    ALLOCATOR->debug();
    // ASSETS->debug();
    // delete assets;
    // delete logicDispatcher;
    delete executor;
    delete poolManager;
    // delete renderer;
}

void OGSEngine::initCoreSystems(int width, int height){
    inputHandler = std::make_unique<Input>();
    windowManager->createWindow(width, height);
    windowManager->setInputHandler(inputHandler.get());
    executor = new ThreadPoolExecutor(2);
    Allocator::getInstance();
    poolManager = PoolManager::getInstance();
    // assets = AssetManager::getInstance();
    renderer = RendererFactory::create();
    if (renderer) renderer->init(*windowManager);
    // logicDispatcher = new LogicDispatcher;
    counter.setActiveLogging(false);

}

// void OGSEngine::setScene(Scene* new_scene){
//     if(currentScene == nullptr) {
//         currentScene = new_scene;
//         currentScene->init();
//     } else {
//         nextScene = new_scene;
//     }
// }
//
// void OGSEngine::checkScenes() {
//     //TODO - check scene to obtain/release semantic
//     if(nextScene != nullptr) {
//         logicDispatcher->clear();
//         nextScene->init();
//         currentScene->dispose();
//         delete currentScene;
//         currentScene = nextScene;
//         nextScene = nullptr;
//     }
// }


void OGSEngine::tick(){
    windowManager->pollEvents();
    if (windowManager->shouldClose()) {
        state = AppState::CLOSE;
        return;
    }
    inputHandler->inputProcess();
    timer::update_timer(timer);
    renderer->beginFrame();
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


