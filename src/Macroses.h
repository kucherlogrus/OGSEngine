#ifndef OGS_MACROSES_H
#define OGS_MACROSES_H

#define T_TEXTURE 0x01
#define T_MESH 0x02
#define T_FONT 0x03
#define T_ATLAS 0x04

#define DEFAULT_ANIMATION_TIME  32 // 16ms * 2 frames
#define CORE Engine::getInstance()
#define RENDER Engine::getInstance()->getRenderer()
#define INPUT Input::getInstance()
#define FPSCOUNTER Engine::getInstance()->getCounter()
#define ASSETS AssetManager::getInstance()
#define TASKEXECUTOR Engine::getInstance()->getExecutor()
#define ALLOCATOR Allocator::getInstance()
#define POOLMANAGER PoolManager::getInstance()
#define LOGTEST(n) logdebug("test" , __FILE__, __LINE__, n);

#define POOLABLE(TYPE)                          \
static TYPE* create() {                         \
    TYPE* object = nullptr;                     \
    object = POOLMANAGER->objectObtain(object); \
    return object;                              \
};                                              \
size_t _getTypeSize() override {                         \
    return sizeof(*this);                       \
};                                              \
bool isPoolable() override { return true; };

#endif //MACROSES_H

