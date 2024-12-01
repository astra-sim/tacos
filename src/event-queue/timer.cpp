/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.

Copyright (c) 2022 Intel Corporation
Copyright (c) 2022 Georgia Institute of Technology
*******************************************************************************/

#include <cassert>
#include <tacos/event-queue/timer.h>

using namespace tacos;

Timer::Timer() noexcept = default;

void Timer::start() noexcept {
    assert(!timerStarted && !timerStopped);

    timerStarted = true;
    startTime = std::chrono::high_resolution_clock::now();
}

void Timer::stop() noexcept {
    assert(timerStarted && !timerStopped);

    timerStopped = true;
    stopTime = std::chrono::high_resolution_clock::now();
}

void Timer::reset() noexcept {
    startTime = {};
    stopTime = {};
    timerStarted = false;
    timerStopped = false;
}

double Timer::elapsedTime() const noexcept {
    assert(timerStarted && timerStopped);

    std::chrono::duration<double, std::micro> time = stopTime - startTime;
    return time.count();
}
