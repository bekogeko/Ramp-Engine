//
// Created by Bekir Gulestan on 10/28/25.
//

#ifndef RAY_GAME_TIMER_H
#define RAY_GAME_TIMER_H
#include <chrono>

class Timer {
public:
    Timer(const char* name);
    ~Timer();
    void Stop();

private:
    const char* m_name;
    std::chrono::time_point<std::chrono::steady_clock> m_startTime;
    bool m_stopped = false;

};
#endif //RAY_GAME_TIMER_H