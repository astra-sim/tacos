/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#pragma once

#include "Typing.h"
#include <set>

namespace Tacos {
class Collective {
  public:
    using CollectiveSet = std::set<std::pair<ChunkId, NpuId>>;

    explicit Collective(ChunkSize chunkSize) noexcept;

    [[nodiscard]] const CollectiveSet& getPrecondition() const noexcept;

    [[nodiscard]] const CollectiveSet& getPostcondition() const noexcept;

    [[nodiscard]] ChunkSize getChunkSize() const noexcept;

    [[nodiscard]] int getChunksCount() const noexcept;

  protected:
    int chunksCount;

    void add(ChunkId chunkId, NpuId src, NpuId dest) noexcept;
    void updateChunksCount() noexcept;

  private:
    ChunkSize chunkSize;
    std::set<ChunkId> chunks;
    CollectiveSet precondition;
    CollectiveSet postcondition;
};
}  // namespace Tacos
