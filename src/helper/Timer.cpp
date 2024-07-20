/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include "Timer.h"
#include <iostream>

using namespace Tacos;

Timer::Timer(std::string name) noexcept : name(std::move(name)) {}

void Timer::start() noexcept {
    startTime = std::chrono::high_resolution_clock::now();
}

void Timer::stop() noexcept {
    stopTime = std::chrono::high_resolution_clock::now();
}

void Timer::reset() noexcept {
    startTime = {};
    stopTime = {};
}

double Timer::getTime(std::string unit) const noexcept {
    if (unit == "ms") {
        std::chrono::duration<double, std::milli> elapsedTime = stopTime - startTime;
        return elapsedTime.count();
    }

    if (unit == "us") {
        std::chrono::duration<double, std::micro> elapsedTime = stopTime - startTime;
        return elapsedTime.count();
    }

    // default: second
    std::chrono::duration<double> elapsedTime = stopTime - startTime;

    return elapsedTime.count();
}

void Timer::print(std::string unit) const noexcept {
    auto elapsedTime = getTime(unit);
    std::cout << "[" << name << "] Timer: " << elapsedTime << " " << unit << std::endl;
}
