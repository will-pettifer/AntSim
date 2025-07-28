//
// Created by billy on 28/07/2025.
//

#include "Timer.h"

#include <iostream>
#include <ostream>

Timer::Timer() {
    startPoint = std::chrono::high_resolution_clock::now();
}

Timer::~Timer() {
    const auto endPoint = std::chrono::high_resolution_clock::now();

    const auto start = std::chrono::time_point_cast<std::chrono::microseconds>(startPoint).time_since_epoch().count();
    const auto end = std::chrono::time_point_cast<std::chrono::microseconds>(endPoint).time_since_epoch().count();
    const auto duration = end - start;

    std::cout << duration << ", " << std::endl;
}
