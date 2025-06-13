/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.

Copyright (c) 2022-2025 Intel Corporation
Copyright (c) 2022-2025 Georgia Institute of Technology
*******************************************************************************/

#pragma once

#include <chrono>

namespace tacos {

/// @brief Class to measure elapsed time in microseconds.
class Timer {
  public:
    /// @brief Construct a Timer instance.
    Timer() noexcept;

    /// @brief Start the timer.
    void start() noexcept;

    /// @brief Stop the timer.
    void stop() noexcept;

    /// @brief Get the elapsed time in microseconds.
    /// @return elapsed time in microseconds.
    [[nodiscard]] double time() const noexcept;

  private:
    /// @brief Start time point.
    std::chrono::high_resolution_clock::time_point startTime;

    /// @brief Stop time point.
    std::chrono::high_resolution_clock::time_point stopTime;
};
}  // namespace tacos
