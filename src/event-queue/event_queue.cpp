/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include <cassert>
#include <tacos/event-queue/event_queue.h>

using namespace tacos;

EventQueue::EventQueue() noexcept = default;

void EventQueue::schedule(const Time newEventTime) noexcept {
    assert(newEventTime > currentTime);

    // if newEventTime already scheduled, skip
    if (eventTimes.find(newEventTime) != eventTimes.end()) {
        return;
    }

    // schedule the event
    eventTimes.insert(newEventTime);
    eventQueue.push(newEventTime);
}

EventQueue::Time EventQueue::pop() noexcept {
    // there must be at least one event
    assert(!empty());

    // pop the event from the queue
    const auto time = eventQueue.top();
    eventQueue.pop();
    eventTimes.erase(time);

    // update the currentTime and return
    currentTime = time;
    return time;
}

bool EventQueue::empty() const noexcept {
    return eventTimes.empty();
}
