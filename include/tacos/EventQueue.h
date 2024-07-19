/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#pragma once

#include "Typing.h"
#include <functional>
#include <queue>
#include <set>
#include <vector>

namespace Tacos {
class EventQueue {
  public:
    EventQueue() noexcept;

    void schedule(Time nextTime) noexcept;

    Time pop() noexcept;

    [[nodiscard]] bool empty() noexcept;

  private:
    Time currentTime;
    std::set<Time> events;
    std::priority_queue<Time, std::vector<Time>, std::greater<>> event_queue;
};
}  // namespace Tacos
