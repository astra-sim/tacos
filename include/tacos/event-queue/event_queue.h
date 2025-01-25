/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.

Copyright (c) 2022 Intel Corporation
Copyright (c) 2022 Georgia Institute of Technology
*******************************************************************************/

#pragma once

#include <cstdint>
#include <functional>
#include <queue>
#include <set>
#include <vector>

namespace tacos {

class EventQueue {
  public:
    using Time = uint64_t;  // ps

    EventQueue() noexcept;

    void schedule(Time newEventTime) noexcept;

    [[nodiscard]] Time getCurrentTime() const noexcept;

    [[nodiscard]] Time pop() noexcept;

    [[nodiscard]] bool empty() const noexcept;

  private:
    Time currentTime = 0;
    std::set<Time> eventTimes = {};
    std::priority_queue<Time, std::vector<Time>, std::greater<>> eventQueue =
        {};
};

}  // namespace tacos
