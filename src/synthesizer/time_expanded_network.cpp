/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.

Copyright (c) 2022-2025 Intel Corporation
Copyright (c) 2022-2025 Georgia Institute of Technology
*******************************************************************************/

#include <cassert>
#include <memory>
#include <tacos/synthesizer/time_expanded_network.h>

using namespace tacos;

TimeExpandedNetwork::TimeExpandedNetwork(const Topology& topology,
                                         const ChunkSize chunkSize) noexcept
    : topology_(topology) {
    assert(chunkSize > 0);
    npusCount_ = topology_.npusCount();

    // initialize TEN lists
    linkBusyUntil_ = decltype(linkBusyUntil_)(npusCount_, std::vector<Time>(npusCount_, -1));
    chunk_ = decltype(chunk_)(npusCount_, std::vector<ChunkID>(npusCount_, -1));
    available_ = decltype(available_)(npusCount_, std::vector<bool>(npusCount_, false));
    linkTransferTimes_ =
        decltype(linkTransferTimes_)(npusCount_, std::vector<Time>(npusCount_, -1));

    // calculate link transfer times
    computeLinkTimes_(chunkSize);
}

bool TimeExpandedNetwork::available(const NpuID src, const NpuID dest) const noexcept {
    assert(0 <= src && src < npusCount_);
    assert(0 <= dest && dest < npusCount_);

    // return true if the link is available at the current timestep
    return available_[src][dest];
}

std::unordered_set<TimeExpandedNetwork::NpuID> TimeExpandedNetwork::backtrack(
    const NpuID dest) noexcept {
    assert(0 <= dest && dest < npusCount_);

    // list of available source NPUs
    auto sources = std::unordered_set<NpuID>();

    // filter the available sources from the topology backtracking
    for (const auto src : topology_.backtrack(dest)) {
        if (available_[src][dest]) {
            sources.insert(src);
        }
    }

    return sources;
}

void TimeExpandedNetwork::timestep(const Time time) noexcept {
    assert(time > currentTime_);

    // update the current timestep
    currentTime_ = time;

    // reset the availability of all links
    for (auto src = 0; src < npusCount_; ++src) {
        for (auto dest = 0; dest < npusCount_; ++dest) {
            // if link is still busy, keep it unavailable
            const auto busyUntil = linkBusyUntil_[src][dest];
            if (busyUntil > currentTime_) {
                available_[src][dest] = false;
                continue;
            }

            // otherwise, reset the link availability
            available_[src][dest] = topology_.connected(src, dest);
        }
    }
}

TimeExpandedNetwork::ChunkID TimeExpandedNetwork::chunk(const NpuID src,
                                                        const NpuID dest) const noexcept {
    assert(0 <= src && src < npusCount_);
    assert(0 <= dest && dest < npusCount_);

    // return the chunk ID being transferred over the link
    return chunk_[src][dest];
}

void TimeExpandedNetwork::transferChunk(const NpuID src,
                                        const NpuID dest,
                                        const ChunkID chunk,
                                        const Time time) noexcept {
    assert(0 <= src && src < npusCount_);
    assert(0 <= dest && dest < npusCount_);
    assert(chunk >= 0);
    assert(time >= currentTime_);

    // assert link is currently available and free
    assert(available_[src][dest]);
    assert(linkBusyUntil_[src][dest] < 0);

    // mark the chunk information and set link as busy until the specified time
    available_[src][dest] = false;
    chunk_[src][dest] = chunk;
    linkBusyUntil_[src][dest] = time;
}

void TimeExpandedNetwork::transferFinished(const NpuID src, const NpuID dest) noexcept {
    assert(0 <= src && src < npusCount_);
    assert(0 <= dest && dest < npusCount_);

    // reset the link busy time and chunk
    available_[src][dest] = true;
    linkBusyUntil_[src][dest] = -1;
    chunk_[src][dest] = -1;
}

TimeExpandedNetwork::Time TimeExpandedNetwork::linkTransferTime(const NpuID src,
                                                                const NpuID dest) const noexcept {
    assert(0 <= src && src < npusCount_);
    assert(0 <= dest && dest < npusCount_);
    assert(topology_.connected(src, dest));

    const auto linkTime = linkTransferTimes_[src][dest];
    assert(linkTime >= 0);

    return linkTime;
}

void TimeExpandedNetwork::computeLinkTimes_(const ChunkSize chunkSize) noexcept {
    assert(chunkSize > 0);

    // for all links
    for (auto src = 0; src < npusCount_; ++src) {
        for (auto dest = 0; dest < npusCount_; ++dest) {
            if (!topology_.connected(src, dest)) {
                continue;
            };

            // use alpha-beta model to calculate link transfer time
            const auto bandwidth = topology_.bandwidth(src, dest);
            const auto latency = topology_.latency(src, dest);
            const auto linkTime = alphaBetaModel_(bandwidth, latency, chunkSize);
            linkTransferTimes_[src][dest] = linkTime;
        }
    }
}

TimeExpandedNetwork::Time TimeExpandedNetwork::alphaBetaModel_(const Bandwidth bandwidth,
                                                               const Latency latency,
                                                               const ChunkSize chunkSize) noexcept {
    assert(bandwidth > 0);
    assert(latency >= 0);
    assert(chunkSize > 0);

    // convert bandwidth from GiB/sec to bytes/microseconds
    const auto bandwidthConverted = bandwidth * (1 << 30) / 1e6;  // bytes/microseconds

    // run alpha-beta model
    const auto alpha = latency;
    const auto beta = chunkSize / bandwidthConverted;

    return alpha + beta;
}
