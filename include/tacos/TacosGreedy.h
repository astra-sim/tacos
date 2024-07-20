/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#pragma once

#include <tacos/AlgorithmStatMonitor.h>
#include <tacos/Collective.h>
#include <tacos/EventQueue.h>
#include <tacos/LinkUsageTracker.h>
#include <tacos/TacosNetwork.h>
#include <tacos/topology/topology.h>
#include <array>
#include <memory>

namespace Tacos {
class TacosGreedy {
  public:
    TacosGreedy(std::shared_ptr<Topology> topology,
                std::shared_ptr<Collective> collective,
                std::shared_ptr<AlgorithmStatMonitor> algStatMonitor,
                std::shared_ptr<LinkUsageTracker> linkUsageTracker) noexcept;

    [[nodiscard]] Time solve() noexcept;

  private:
    using RequestSet = std::vector<std::pair<ChunkId, NpuId>>;
    using CandidateLinkSet = std::set<std::pair<LinkId, Time>>;
    using Contains = std::vector<std::vector<bool>>;

    std::unique_ptr<TacosNetwork> network;
    EventQueue eventQueue;

    std::shared_ptr<Topology> topology;
    std::shared_ptr<Collective> collective;
    std::shared_ptr<AlgorithmStatMonitor> algorithmStatMonitor;
    std::shared_ptr<LinkUsageTracker> linkUsageTracker;

    int npusCount;
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
}  // namespace Tacos
