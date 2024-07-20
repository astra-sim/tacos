/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include "EventQueue.h"
#include <cassert>

using namespace Tacos;

EventQueue::EventQueue() noexcept : currentTime(0) {
    event_queue = {};
    events = {};
}

void EventQueue::schedule(Time nextTime) noexcept {
    assert(nextTime >= 0);

    if (events.find(nextTime) == events.end()) {
        // doesn't exist: should insert
        events.insert(nextTime);
        event_queue.push(nextTime);
    }
}

Time EventQueue::pop() noexcept {
    if (events.empty()) {
        return -1;
    }

    const auto time = event_queue.top();
    event_queue.pop();
    events.erase(time);
    currentTime = time;

    return time;
}

bool EventQueue::empty() noexcept {
    return events.empty();
}
