/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include <cassert>
#include <tacos/event_queue/timer.h>

using namespace tacos;

Timer::Timer() noexcept = default;

void Timer::start() noexcept {
    assert(!_timer_started);
    assert(!_timer_stopped);

    _timer_started = true;
    _start_time = std::chrono::high_resolution_clock::now();
}

void Timer::stop() noexcept {
    assert(_timer_started);
    assert(!_timer_stopped);

    _timer_stopped = true;
    _stop_time = std::chrono::high_resolution_clock::now();
}

void Timer::reset() noexcept {
    _start_time = {};
    _stop_time = {};
    _timer_started = false;
    _timer_stopped = false;
}

double Timer::elapsed_time() const noexcept {
    assert(_timer_started);
    assert(_timer_stopped);

    const auto elapsed_time = std::chrono::duration<double, std::milli>(_stop_time - _start_time);
    return elapsed_time.count();
}
