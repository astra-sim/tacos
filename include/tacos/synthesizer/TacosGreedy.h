/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#pragma once

#include "TacosNetwork.h"
#include <tacos/collective/collective.h>
#include <tacos/event-queue/event_queue.h>
#include <tacos/topology/topology.h>

namespace tacos {
class TacosGreedy {
  public:
    using ChunkSize = Topology::ChunkSize;
    using Time = EventQueue::Time;
    using NpuID = Topology::NpuID;
    using ChunkID = Collective::ChunkID;
    using LinkId = Topology::LinkId;

    using RequestSet = std::vector<std::pair<ChunkID, NpuId>>;
    using CandidateLinkSet = std::set<std::pair<LinkId, Time>>;
    using Contains = std::vector<std::vector<bool>>;

    TacosGreedy(std::shared_ptr<Topology> topology,
                std::shared_ptr<Collective> collective) noexcept;

    [[nodiscard]] Time solve() noexcept;

  private:
    using RequestSet = std::vector<std::pair<ChunkID, NpuID>>;
    using CandidateLinkSet = std::set<std::pair<LinkId, Time>>;
    using Contains = std::vector<std::vector<bool>>;

    std::unique_ptr<TacosNetwork> network;
    EventQueue eventQueue;

    std::shared_ptr<Topology> topology;
    std::shared_ptr<Collective> collective;

    int npusCount;
    int chunksCount;
    ChunkSize chunkSize;

    std::random_device randomDevice;
    std::default_random_engine randomEngine{randomDevice()};

    std::shared_ptr<TacosGreedy::RequestSet> initializeRequests(
        std::shared_ptr<Contains> contains) noexcept;

    bool prepareBacktracking(std::shared_ptr<RequestSet> requests,
                             Time currentTime,
                             std::shared_ptr<Contains> contains) noexcept;

    static std::pair<LinkId, Time> selectBestLink(const CandidateLinkSet& candidateLinks) noexcept;
};
}  // namespace tacos
