//
// Created by Bekir Gulestan on 10/28/25.
//
#include "Engine/Profiling/Timer.h"

#include <iostream>
#include <ostream>


Timer::Timer(const char *name):m_name(name),m_stopped(false) {
    m_startTime = std::chrono::high_resolution_clock::now();
}

Timer::~Timer() {
    if (!m_stopped) {
        m_stopped = true;
    }
    Stop();
}

void Timer::Stop() {
    auto endTime = std::chrono::high_resolution_clock::now();

    long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_startTime).time_since_epoch().count();
    long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTime).time_since_epoch().count();

    std::cout << m_name << ": " << (end - start) << " microseconds\n";

}
