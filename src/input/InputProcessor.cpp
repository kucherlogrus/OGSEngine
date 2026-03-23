#include "InputProcessor.h"
#include "OGSEngine.h"

using namespace ogs;

void DefaultInputProcessor::touchDown(const TouchEvent& event){
    // auto scene = OGSEngine::getInstance()->getScene();
    // if(scene != nullptr) {
    //     scene->onTouchBegin(event);
    // }
}

void DefaultInputProcessor::touchUp(const TouchEvent& event){
    // auto scene = OGSEngine::getInstance()->getScene();
    // if(scene != nullptr) {
    //     scene->onTouchEnd(event);
    // }
}

void DefaultInputProcessor::move(const TouchEvent& event){
    // auto scene = OGSEngine::getInstance()->getScene();
    // if(scene != nullptr) {
    //     scene->onTouchMove(event);
    // }
}

void DefaultInputProcessor::keyboardPress(const KeyboardEvent & event) {
    // auto scene = Engine::getInstance()->getScene();
    // if(scene != nullptr) {
    //     scene->keyPress(event);
    // }
}

void DefaultInputProcessor::keyboardRelease(const KeyboardEvent & event) {
    // auto scene = OGSEngine::getInstance()->getScene();
    // if(scene != nullptr) {
    //     scene->keyRelease(event);
    // }
}

void DefaultInputProcessor::mouseMove(const MouseEvent & event) {
    // auto scene = OGSEngine::getInstance()->getScene();
    // if(scene != nullptr) {
    //     scene->handleMouseMove(event);
    // }
}

void DefaultInputProcessor::scrollMove(const MouseEvent & event) {
    // auto scene = OGSEngine::getInstance()->getScene();
    // if(scene != nullptr) {
    //     scene->handleMouseScroll(event);
    // }
}
