/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#pragma once

#include <map>
#include <set>
#include <tacos/topology/topology.h>

namespace tacos {
class Collective {
  public:
    using ChunkID = int;
    using ChunkSize = double;  // MB
    using NpuID = Topology::NpuID;

    using CollectiveCondition = std::map<NpuID, std::set<ChunkID>>;

    Collective(int npusCount, ChunkSize chunkSize) noexcept;

    [[nodiscard]] ChunkSize getChunkSize() const noexcept;

    [[nodiscard]] int getChunksCount() const noexcept;

  protected:
    int npusCount;
    int chunksCount = 0;

    void add(ChunkID chunkID, NpuID src, NpuID dest) noexcept;

    void updateChunksCount() noexcept;

  private:
    ChunkSize chunkSize;

    std::set<ChunkID> chunks = {};
    CollectiveCondition precondition = {};
    CollectiveCondition postcondition = {};
};

}  // namespace tacos
