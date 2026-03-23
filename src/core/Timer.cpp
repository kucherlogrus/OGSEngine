#include "core/Timer.h"
#include <chrono>


using namespace ogs;

void timer::update_timer(timer::game_timer & nowtime) {
    using namespace std::chrono;
    auto now = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    update_timer(now,  nowtime);
}


void timer::update_timer(long long nowtime, timer::game_timer& timer){
    timer.gametime += nowtime - timer.currenttime;
    timer.msdelta = nowtime - timer.currenttime;
    timer.delta = timer.msdelta / 1000;
    timer.currenttime = nowtime;
}
