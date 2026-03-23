#ifndef OGS_FPSCOUNTER_H
#define OGS_FPSCOUNTER_H


namespace ogs {

    class FPSCounter {

    private:
        int fps = 0;
        int fps_counter = 0;
        float delta = 0;
        int tick = 0;
        int max_tick_count = 10;
        bool tick_active = false;
        bool activeLogging = false;

    public:
        FPSCounter() = default;

        ~FPSCounter() = default;

        void update(float dt);
        bool getTick(int num);
        void setActiveLogging(bool state) { activeLogging = state; };
        void setMaxTickCount(int count) { max_tick_count = count;}
        int getFps(){ return fps;};

    };

}

#endif
