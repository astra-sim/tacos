/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#pragma once

#include <functional>
#include <queue>
#include <set>
#include <vector>

namespace tacos {

/// Time = us;
using Time = double;

class EventQueue {
    /**
     * EventQueue used in TACOS project manages the time of the events,
     * not the events themselves.
     */
  public:
    /***
     * Create a new event queue
     * with one event scheduled at t = 0.
     */
    EventQueue() noexcept;

    /**
     * Schedule an event to be executed at event_time.
     *
     * @param event_time time of the event
     */
    void schedule(Time event_time) noexcept;

    /**
     * Get the time of the next event,
     * and remove it from the event queue.
     *
     * @return time of the next event
     */
    Time pop() noexcept;

    /**
     * Check if the event queue is empty.
     *
     * @return true if the event queue is empty, false if not.
     */
    [[nodiscard]] bool empty() noexcept;

  private:
    /// current time of the event queue
    Time _current_time = 0;

    /// set of scheduled event times, unsorted
    /// Used for fast lookup of the event times already scheduled
    std::set<Time> _event_times;

    /// scheduled event times, sorted
    /// Used for fast retrieval of the next event time
    std::priority_queue<Time, std::vector<Time>, std::greater<>> _event_times_sorted;
};

}  // namespace tacos
