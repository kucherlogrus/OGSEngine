#ifndef OGSENGINE_TIMER_H
#define OGSENGINE_TIMER_H

namespace ogs::timer {

struct game_timer {
    long long currenttime = 0;
    long gametime = 0;
    float msdelta = 0;
    float delta = 0;
};


void update_timer(game_timer &nowtime);

void update_timer(long long nowtime, game_timer &timer);

}

#endif //OGSENGINE_TIMER_H
