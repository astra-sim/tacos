/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.

Copyright (c) 2022-2025 Intel Corporation
Copyright (c) 2022-2025 Georgia Institute of Technology
*******************************************************************************/

#pragma once

#include <functional>
#include <queue>
#include <unordered_set>
#include <vector>

namespace tacos {

/// @brief Manages discrete event times during TACOS execution.
class EventQueue {
  public:
    /// @brief Unit in TACOS: microseconds (us)
    using Time = double;

    /// @brief Construct an empty event queue.
    EventQueue() noexcept;

    /// @brief Schedule a new event time.
    /// @param time time to schedule a new event.
    void schedule(Time time) noexcept;

    /// @brief Pop the next event time from the queue.
    /// @return next event time
    Time pop() noexcept;

    /// @brief Check the event queue is empty.
    /// @return true if no events are scheduled, false otherwise.
    [[nodiscard]] bool empty() noexcept;

    /// @brief Reset the event queue to an empty state.
    void reset() noexcept;

  private:
    /// @brief Current event queue time.
    Time currentTime_ = -1;

    /// @brief Set of unique event times.
    std::unordered_set<Time> events_ = {};

    /// @brief Priority queue to manage event times in ascending order.
    std::priority_queue<Time, std::vector<Time>, std::greater<>> event_queue_ = {};
};
}  // namespace tacos
