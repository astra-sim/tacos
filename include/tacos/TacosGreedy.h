/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#pragma once

#include <array>
#include <memory>
#include <tacos/AlgorithmStatMonitor.h>
#include <tacos/LinkUsageTracker.h>
#include <tacos/collective/collective.h>
#include <tacos/event_queue/event_queue.h>
#include <tacos/synthesizer/time_expanded_network.h>
#include <tacos/topology/topology.h>

namespace tacos {
class TacosGreedy {
  public:
    using LinkId = std::pair<NpuId, NpuId>;

    TacosGreedy(std::shared_ptr<Topology> topology,
                std::shared_ptr<Collective> collective,
                std::shared_ptr<AlgorithmStatMonitor> algStatMonitor,
                std::shared_ptr<LinkUsageTracker> linkUsageTracker) noexcept;

    [[nodiscard]] Time solve() noexcept;

  private:
    using RequestSet = std::vector<std::pair<ChunkId, NpuId>>;
    using CandidateLinkSet = std::set<std::pair<LinkId, Time>>;
    using Contains = std::vector<std::vector<bool>>;

    std::unique_ptr<TimeExpandedNetwork> network;
    EventQueue eventQueue;

    std::shared_ptr<Topology> topology;
    std::shared_ptr<Collective> collective;
    std::shared_ptr<AlgorithmStatMonitor> algorithmStatMonitor;
    std::shared_ptr<LinkUsageTracker> linkUsageTracker;

    int npus_count;
    int chunksCount;
    ChunkSize chunkSize;

    std::random_device randomDevice;
    std::default_random_engine randomEngine{randomDevice()};

    std::shared_ptr<TacosGreedy::RequestSet> initializeRequests(std::shared_ptr<Contains> contains) noexcept;

    bool prepareBacktracking(std::shared_ptr<RequestSet> requests,
                             Time currentTime,
                             std::shared_ptr<Contains> contains) noexcept;

    static std::pair<LinkId, Time> selectBestLink(const CandidateLinkSet& candidateLinks) noexcept;
};
}  // namespace tacos
