/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.

Copyright (c) 2022 Intel Corporation
Copyright (c) 2022 Georgia Institute of Technology
*******************************************************************************/

#include <cassert>
#include <tacos/synthesizer/time_expanded_network.h>

using namespace tacos;

TimeExpandedNetwork::TimeExpandedNetwork(
    const std::shared_ptr<Topology> topology) noexcept
    : topology(topology) {
    assert(topology != nullptr);

    npusCount = topology->getNpusCount();

    linkBusyUntil.resize(npusCount, std::vector<Time>(npusCount, 0));
    linkAvailable.resize(npusCount, std::vector<bool>(npusCount, false));
}

std::set<TimeExpandedNetwork::NpuID> TimeExpandedNetwork::backtrackTEN(
    const NpuID dest) const noexcept {
    assert(0 <= dest && dest < npusCount);

    // check available source links
    auto sourceNpus = std::set<NpuID>();
    for (auto src = 0; src < npusCount; src++) {
        if (linkAvailable[src][dest]) {
            sourceNpus.insert(src);
        }
    }

    return sourceNpus;
}

void TimeExpandedNetwork::updateCurrentTime(
    const Time newCurrentTime) noexcept {
    assert(newCurrentTime > currentTime);
    currentTime = newCurrentTime;

    // update link availability
    updateLinkAvailability();
}

void TimeExpandedNetwork::markLinkOccupied(const NpuID src,
                                           const NpuID dest) noexcept {
    assert(0 <= src && src < npusCount);
    assert(0 <= dest && dest < npusCount);
    assert(linkAvailable[src][dest]);
    assert(src != dest);

    // mark the link as occupied
    const auto linkDelay = topology->getLinkDelay(src, dest);
    linkBusyUntil[src][dest] = currentTime;
    linkAvailable[src][dest] = false;
}

void TimeExpandedNetwork::updateLinkAvailability() noexcept {
    for (auto src = 0; src < npusCount; src++) {
        for (auto dest = 0; dest < npusCount; dest++) {
            if (src == dest) {
                linkAvailable[src][dest] = false;
                continue;
            }

            if (!topology->isConnected(src, dest)) {
                linkAvailable[src][dest] = false;
                continue;
            }

            linkAvailable[src][dest] = checkLinkAvailability(src, dest);
        }
    }
}

bool TimeExpandedNetwork::checkLinkAvailability(
    const NpuID src, const NpuID dest) const noexcept {
    assert(0 <= src && src < npusCount);
    assert(0 <= dest && dest < npusCount);
    assert(src != dest);

    // the link is available only if:
    // the start time of the current transmission does not overlap
    // with the end time of the previous transmission
    const auto linkDelay = topology->getLinkDelay(src, dest);
    const auto transmissionStartTime = currentTime - linkDelay;

    // transmission start time must be valid
    if (transmissionStartTime < 0) {
        return false;
    }

    return transmissionStartTime >= linkBusyUntil[src][dest];
}
