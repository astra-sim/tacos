/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#pragma once

#include <memory>
#include <random>
#include <tacos/collective/collective.h>
#include <tacos/event-queue/event_queue.h>
#include <tacos/synthesizer/time_expanded_network.h>
#include <tacos/topology/topology.h>
#include <tacos/writer/synthesis_result.h>

namespace tacos {

class Synthesizer {
  public:
    using Time = EventQueue::Time;
    using NpuID = Topology::NpuID;
    using ChunkID = Collective::ChunkID;
    using ChunkSize = Topology::ChunkSize;
    using CollectiveCondition = Collective::CollectiveCondition;

    Synthesizer(std::shared_ptr<Topology> topology,
                std::shared_ptr<Collective> collective,
                bool verbose = false) noexcept;

    [[nodiscard]] SynthesisResult synthesize() noexcept;

  private:
    EventQueue eventQueue = {};
    Time currentTime;

    std::shared_ptr<Topology> topology;
    std::shared_ptr<Collective> collective;

    TimeExpandedNetwork ten;

    int npusCount;
    int chunksCount;

    bool verbose;

    // synthesis result
    SynthesisResult synthesisResult;

    CollectiveCondition precondition = {};
    CollectiveCondition postcondition = {};

    // topology link delays
    std::set<Time> distinctLinkDelays = {};

    // random devices
    std::random_device randomDevice = {};
    std::mt19937 randomEngine = decltype(randomEngine)(randomDevice());

    void scheduleNextEvents() noexcept;

    void linkChunkMatching() noexcept;

    [[nodiscard]] std::pair<NpuID, ChunkID> selectPostcondition(
        CollectiveCondition* const currentPostcondition) noexcept;

    [[nodiscard]] std::set<NpuID> checkCandidateSourceNpus(
        ChunkID chunk,
        const CollectiveCondition& currentPrecondition,
        const std::set<NpuID>& sourceNpus) noexcept;

    [[nodiscard]] NpuID selectSourceNpu(
        const std::set<NpuID>& candidateSourceNpus) noexcept;

    void markLinkChunkMatch(NpuID src, NpuID dest, ChunkID chunk) noexcept;

    [[nodiscard]] bool synthesisCompleted() const noexcept;
};

}  // namespace tacos
