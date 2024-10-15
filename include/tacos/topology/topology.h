/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#pragma once

#include <tacos/event-queue/event_queue.h>
#include <vector>

namespace tacos {

class Topology {

  public:
    using Time = EventQueue::Time;

    using NpuID = int;
    using Latency = double;    // us
    using Bandwidth = double;  // GB/s

    Topology() noexcept;

    [[nodiscard]] std::vector<NpuID> incomingNpus(NpuID dest) const noexcept;

    [[nodiscard]] int getNpusCount() const noexcept;

    [[nodiscard]] bool isConnected(NpuID src, NpuID dest) const noexcept;

    [[nodiscard]] Time linkTime(LinkId link, ChunkSize chunkSize) const noexcept;

    [[nodiscard]] bool getTopologyValue(LinkId link) const noexcept;

    [[nodiscard]] double getBW(NpuID src, NpuID dest);

    [[nodiscard]] int getLinksCount() const noexcept;

  protected:
    int npusCount = -1;

    void setNpusCount(int newNpusCount) noexcept;

    void connect(NpuID src,
                 NpuID dest,
                 Latency latency,
                 Bandwidth bandwidth,
                 bool bidirectional = false) noexcept;

  private:
    std::vector<std::vector<bool>> connected = {};
    std::vector<std::vector<Latency>> latencies = {};
    std::vector<std::vector<Bandwidth>> bandwidths = {};
    std::vector<std::vector<Time>> linkDelays = {};
};

}  // namespace tacos
