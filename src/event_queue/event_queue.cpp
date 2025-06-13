/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.

Copyright (c) 2022-2025 Intel Corporation
Copyright (c) 2022-2025 Georgia Institute of Technology
*******************************************************************************/

#include <cassert>
#include <tacos/event_queue/event_queue.h>

using namespace tacos;

EventQueue::EventQueue() noexcept {
    // reset the event queue
    reset();
}

void EventQueue::schedule(const Time time) noexcept {
    assert(time > currentTime_);

    // Check if the event time is already scheduled
    if (events_.find(time) != events_.end()) {
        // Already exists: no need to insert
        return;
    }

    // insert the new event time
    events_.insert(time);
    event_queue_.push(time);
}

EventQueue::Time EventQueue::pop() noexcept {
    assert(!events_.empty());

    // pop the next event time
    const auto time = event_queue_.top();
    event_queue_.pop();
    events_.erase(time);
    assert(time > currentTime_);

    // update the current time
    currentTime_ = time;
    return time;
}

bool EventQueue::empty() noexcept {
    return events_.empty();
}

void EventQueue::reset() noexcept {
    currentTime_ = -1;
    events_.clear();
    event_queue_ = {};

    // schedule the initial event at time 0
    schedule(0);
}
