/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.

Copyright (c) 2022 Intel Corporation
Copyright (c) 2022 Georgia Institute of Technology
*******************************************************************************/

#pragma once

#include <map>
#include <set>
#include <tacos/topology/topology.h>

namespace tacos {
class Collective {
  public:
    using ChunkID = int;
    using ChunkSize = Topology::ChunkSize;
    using NpuID = Topology::NpuID;

    using CollectiveCondition = std::map<NpuID, std::set<ChunkID>>;

    Collective(int npusCount, ChunkSize chunkSize) noexcept;

    [[nodiscard]] ChunkSize getChunkSize() const noexcept;

    [[nodiscard]] int getChunksCount() const noexcept;

    [[nodiscard]] CollectiveCondition getPrecondition() const noexcept;

    [[nodiscard]] CollectiveCondition getPostcondition() const noexcept;

    [[nodiscard]] bool synthesisCompleted() const noexcept;

    [[nodiscard]] int chunksPerNpu() const noexcept;

  protected:
    int npusCount;
    int chunksCount = 0;
    int chunksPerNpu_ = 0;

    void add(ChunkID chunkID, NpuID src, NpuID dest) noexcept;

    void updateChunksCount() noexcept;

  private:
    ChunkSize chunkSize;

    std::set<ChunkID> chunks = {};
    CollectiveCondition precondition = {};
    CollectiveCondition postcondition = {};
};

}  // namespace tacos
