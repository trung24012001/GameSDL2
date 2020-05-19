#ifndef TIMER_H
#define TIMER_H

#include "Common.h"

class Timer
{
public:
    Timer();
    ~Timer();

    void start();
    void stop();
    void paused();
    void unpaused();
    int get_ticks();
    bool is_started();
    bool is_paused();

private:
    int start_tick_;
    int paused_tick_;
    int time_immortal_;
    bool is_paused_;
    bool is_started_;

};

#endif // TIMER_H
