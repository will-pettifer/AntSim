//
// Created by billy on 28/07/2025.
//

#ifndef TIMER_H
#define TIMER_H
#include <chrono>


class Timer {
public:
    Timer();
    ~Timer();
private:
    std::chrono::time_point<std::chrono::high_resolution_clock> startPoint;
};



#endif //TIMER_H
