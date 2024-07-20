/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#pragma once

#include <chrono>
#include <string>

namespace tacos {

class Timer {
    /***
     * Timer class to measure time elapsed between start and stop.
     */
  public:
    /**
     * Construct a timer.
     */
    Timer() noexcept;

    /**
     * Start the timer.
     */
    void start() noexcept;

    /**
     * Stop the timer.
     */
    void stop() noexcept;

    /**
     * Reset the timer.
     */
    void reset() noexcept;

    /**
     * Get the elapsed time between start and stop
     * in ms unit.
     *
     * @return elapsed time in ms
     */
    [[nodiscard]] double elapsed_time() const noexcept;

  private:
    /// timer started time
    std::chrono::high_resolution_clock::time_point _start_time = {};

    /// timer stopped time
    std::chrono::high_resolution_clock::time_point _stop_time = {};

    /// check whether the timer has started
    bool _timer_started = false;

    /// check whether the timer has stopped
    bool _timer_stopped = false;
};

}  // namespace tacos
