/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.

Copyright (c) 2022-2025 Intel Corporation
Copyright (c) 2022-2025 Georgia Institute of Technology
*******************************************************************************/

#include <cassert>
#include <tacos/topology/topology.h>

using namespace tacos;

Topology::Topology() noexcept = default;

void Topology::setNpusCount_(const int npusCount) noexcept {
    assert(npusCount > 0);

    // set npusCount
    npusCount_ = npusCount;

    // allocate memory
    connected_ = decltype(connected_)(npusCount, std::vector<bool>(npusCount, false));
    latencies_ = decltype(latencies_)(npusCount, std::vector<Latency>(npusCount, 0));
    bandwidths_ = decltype(bandwidths_)(npusCount, std::vector<Bandwidth>(npusCount, 0));

    for (auto dest = 0; dest < npusCount; ++dest) {
        backtrackMap_[dest] = {};
    }
}

Topology::Bandwidth Topology::bandwidth(NpuID src, NpuID dest) const noexcept {
    assert(0 <= src && src < npusCount_);
    assert(0 <= dest && dest < npusCount_);
    assert(connected_[src][dest]);

    return bandwidths_[src][dest];
}

Topology::Latency Topology::latency(NpuID src, NpuID dest) const noexcept {
    assert(0 <= src && src < npusCount_);
    assert(0 <= dest && dest < npusCount_);
    assert(connected_[src][dest]);

    return latencies_[src][dest];
}

std::vector<Topology::NpuID> Topology::backtrack(const NpuID dest) const noexcept {
    assert(0 <= dest && dest < npusCount_);
    assert(backtrackMap_.size() == npusCount_);

    return backtrackMap_.at(dest);
}

void Topology::connect_(const NpuID src,
                        const NpuID dest,
                        Bandwidth bandwidth,
                        Latency latency,
                        const bool bidirectional) noexcept {
    assert(0 <= src && src < npusCount_);
    assert(0 <= dest && dest < npusCount_);
    assert(bandwidth > 0);
    assert(latency >= 0);

    // connect src -> dest
    connected_[src][dest] = true;
    bandwidths_[src][dest] = bandwidth;
    latencies_[src][dest] = latency;
    backtrackMap_[dest].push_back(src);

    if (bidirectional) {
        // connect dest -> src (if bi-directional)
        connect_(dest, src, bandwidth, latency, false);
    }
}

bool Topology::connected(const NpuID src, const NpuID dest) const noexcept {
    assert(0 <= src && src < npusCount_);
    assert(0 <= dest && dest < npusCount_);

    return connected_[src][dest];
}

int Topology::npusCount() const noexcept {
    assert(npusCount_ > 0);

    return npusCount_;
}
