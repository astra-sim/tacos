/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include <cassert>
#include <iostream>
#include <tacos/event_queue/event_queue.h>

using namespace tacos;

EventQueue::EventQueue() noexcept {
    // initialize event_queue with one event at t = 0.
    _event_times.insert(0);
    _event_times_sorted.push(0);
}

void EventQueue::schedule(const Time event_time) noexcept {
    assert(event_time > _current_time);

    if (_event_times.find(event_time) != _event_times.end()) {
        // event_time already scheduled
        return;
    }

    // insert event_time to the EventQueue
    _event_times.insert(event_time);
    _event_times_sorted.push(event_time);
}

Time EventQueue::pop() noexcept {
    assert(!_event_times.empty());

    // get next event time
    const auto next_event_time = _event_times_sorted.top();

    // remove next_event_time from the EventQueue
    _event_times.erase(next_event_time);
    _event_times_sorted.pop();

    // progress the current time and return
    _current_time = next_event_time;
    return next_event_time;
}

bool EventQueue::empty() noexcept {
    return _event_times.empty();
}
