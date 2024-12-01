/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#pragma once

#include <tacos/collective/collective.h>
#include <tacos/event-queue/event_queue.h>
#include <tacos/topology/topology.h>
#include <tacos/writer/npu_result.h>

namespace tacos {

class SynthesisResult {
  public:
    using NpuID = Topology::NpuID;
    using ChunkID = Collective::ChunkID;
    using ChunkSize = Collective::ChunkSize;
    using Time = EventQueue::Time;

    SynthesisResult(std::shared_ptr<Topology> topology,
                    std::shared_ptr<Collective> collective) noexcept;

    void markLinkChunkMatch(ChunkID chunk, NpuID src, NpuID dest) noexcept;

    void setCollectiveTime(Time newCollectiveTime) noexcept;

    [[nodiscard]] Time getCollectiveTime() const noexcept;

    [[nodiscard]] std::vector<ChunkID> getEgressLinkInfo(
        NpuID src, NpuID dest) const noexcept;

    [[nodiscard]] std::vector<ChunkID> getIngressLinkInfo(
        NpuID src, NpuID dest) const noexcept;

  private:
    int npusCount;
    int chunksCount;

    Time collectiveTime = 0;

    std::vector<NpuResult> npuResults;
};

}  // namespace tacos
