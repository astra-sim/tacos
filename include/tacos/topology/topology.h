/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.

Copyright (c) 2022 Intel Corporation
Copyright (c) 2022 Georgia Institute of Technology
*******************************************************************************/

#pragma once

#include <cstdint>
#include <set>
#include <tacos/event-queue/event_queue.h>
#include <tuple>
#include <vector>

namespace tacos {

class Topology {

  public:
    using Time = EventQueue::Time;

    using NpuID = int;
    using Latency = double;      // ns
    using Bandwidth = double;    // GB/s ~ B/ns
    using ChunkSize = uint64_t;  // B

    Topology() noexcept;

    [[nodiscard]] bool isConnected(NpuID src, NpuID dest) const noexcept;

    void setChunkSize(ChunkSize newChunkSize) noexcept;

    [[nodiscard]] Time getLinkDelay(NpuID src, NpuID dest) const noexcept;

    [[nodiscard]] int getNpusCount() const noexcept;

    [[nodiscard]] std::set<Time> getDistinctLinkDelays() const noexcept;

    [[nodiscard]] int getLinksCount() const noexcept;

    [[nodiscard]] Latency getLatency(NpuID src, NpuID dest) const noexcept;

    [[nodiscard]] Bandwidth getBandwidth(NpuID src, NpuID dest) const noexcept;

  protected:
    void setNpusCount(int newNpusCount) noexcept;

    void connect(NpuID src,
                 NpuID dest,
                 Latency latency,
                 Bandwidth bandwidth,
                 bool bidirectional = false) noexcept;

  private:
    int npusCount = -1;
    int linksCount = 0;
    bool npusCountSet = false;

    std::set<Time> distinctLinkDelays = {};

    std::vector<std::vector<bool>> connected = {};
    std::vector<std::vector<Latency>> latencies = {};
    std::vector<std::vector<Bandwidth>> bandwidths = {};
    std::vector<std::vector<Time>> linkDelays = {};

    Time computeLinkDelay(NpuID src, NpuID dest) const noexcept;

    ChunkSize chunkSize = -1;
    bool chunkSizeSet = false;
};

}  // namespace tacos
