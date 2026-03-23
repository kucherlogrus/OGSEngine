#include "FPSCounter.h"
#include "Logsystem.h"


using namespace ogs;

void FPSCounter::update(float msdelta) {
    fps_counter++;
    delta += msdelta;
    if(tick_active) {
        tick_active = false;
    }
    if(delta >= 1000){
        tick += 1;
        tick_active = true;
        if(tick == max_tick_count) {
            tick = 0;
        }
        fps = fps_counter;
        fps_counter = 0;
        delta = 0;
        if(activeLogging){
            loginfo("FPSCounter", "fps", fps);
        }
    }
}

bool FPSCounter::getTick(int num) {
    return tick == num && tick_active;
}


