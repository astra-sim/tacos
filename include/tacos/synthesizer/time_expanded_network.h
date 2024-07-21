/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#pragma once

#include <algorithm>
#include <memory>
#include <random>
#include <tacos/topology/topology.h>

#include <tacos/collective/collective.h>
#include <vector>

namespace tacos {
class TacosNetwork {
  public:
    using NpuId = NpuId;

    TacosNetwork(std::shared_ptr<Topology> topology, ChunkSize chunkSize) noexcept;

    std::vector<NpuId> backtrack_source_npus(NpuId dest, bool shuffle = true) noexcept;

    std::vector<NpuId> outgoingNpus(NpuId src, bool shuffle = true) noexcept;

    void removeLink(NpuId src, NpuId dest) noexcept;

    void reset() noexcept;

    Time transmission_time(NpuId src, NpuId dest) const noexcept;

    void setLinkTime(NpuId src, NpuId dest, Time time) noexcept;

    ChunkId processingChunk(NpuId src, NpuId dest) const noexcept;

    void setProcessingChunk(NpuId src, NpuId dest, ChunkId chunk) noexcept;

  private:
    std::shared_ptr<Topology> topology;

    std::vector<std::vector<Time>> linkTimes;
    std::vector<std::vector<ChunkId>> processingChunks;
    std::vector<std::vector<bool>> backtrackingTopology;
    size_t topologyBytesCount;

    ChunkSize chunksSize;

    int npus_count;

    std::random_device randomDevice;
    std::default_random_engine randomEngine{randomDevice()};
};
}  // namespace tacos
