#include "OGSEngine.h"
#include "monitoring/Logsystem.h"

int main(int argc, char *argv[]) {
    ogs::loginfo("Game", "Start init engine");
    auto engine = std::make_unique<ogs::OGSEngine>();
    engine->initCoreSystems();
    engine->requestloop();
    return 0;
}
