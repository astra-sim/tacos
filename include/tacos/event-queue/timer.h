/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.

Copyright (c) 2022 Intel Corporation
Copyright (c) 2022 Georgia Institute of Technology
*******************************************************************************/

#pragma once

#include <chrono>

namespace tacos {

class Timer {
  public:
    explicit Timer() noexcept;

    void start() noexcept;

    void stop() noexcept;

    void reset() noexcept;

    [[nodiscard]] double elapsedTime() const noexcept;

  private:
    std::chrono::high_resolution_clock::time_point startTime = {};
    std::chrono::high_resolution_clock::time_point stopTime = {};

    bool timerStarted = false;
    bool timerStopped = false;
};

}  // namespace tacos
