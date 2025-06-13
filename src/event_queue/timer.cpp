/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.

Copyright (c) 2022-2025 Intel Corporation
Copyright (c) 2022-2025 Georgia Institute of Technology
*******************************************************************************/

#include <tacos/event_queue/timer.h>

using namespace tacos;

Timer::Timer() noexcept = default;

void Timer::start() noexcept {
    startTime = std::chrono::high_resolution_clock::now();
}

void Timer::stop() noexcept {
    stopTime = std::chrono::high_resolution_clock::now();
}

double Timer::time() const noexcept {
    std::chrono::duration<double, std::micro> elapsedTime = stopTime - startTime;
    return elapsedTime.count();
}
