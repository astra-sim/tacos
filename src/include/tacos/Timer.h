/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#pragma once

#include <chrono>
#include <string>

namespace Tacos {
class Timer {
  public:
    explicit Timer(std::string name) noexcept;

    void start() noexcept;

    void stop() noexcept;

    void reset() noexcept;

    [[nodiscard]] double getTime(std::string unit = "s") const noexcept;

    void print(std::string unit = "s") const noexcept;

  private:
    std::string name;
    std::chrono::high_resolution_clock::time_point startTime;
    std::chrono::high_resolution_clock::time_point stopTime;
};
}  // namespace Tacos
