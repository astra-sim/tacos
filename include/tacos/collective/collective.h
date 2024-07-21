/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#pragma once

#include <memory>
#include <set>
#include <tacos/topology/topology.h>

namespace tacos {

/// Chunk Id = 0, 1, 2, ...
using ChunkId = int;

class Collective {
  public:
    /// precondition = (chunk_id, src), postcondition = (chunk_id, dest)
    using CollectiveCondition = std::pair<ChunkId, NpuId>;

    /***
     * Construct an empty collective with chunk_size.
     *
     * @param topology pointer to the underlying topology
     * @param chunk_size each chunk's size (in MB)
     */
    Collective(std::shared_ptr<Topology> topology, ChunkSize chunk_size) noexcept;

    /**
     * Get chunk size (in MB)
     * @return chunk size (in MB)
     */
    [[nodiscard]] ChunkSize chunk_size() const noexcept;

    /**
     * Get the number of unique chunks in the collective
     *
     * @return the number of chunks
     */
    [[nodiscard]] int chunks_count() const noexcept;

    /**
     * Get the pre-conditions of the collective
     *
     * @return pre-conditions
     */
    [[nodiscard]] const std::set<CollectiveCondition>& pre_conditions() const noexcept;

    /**
     * Get the post-conditions of the collective
     *
     * @return post-conditions
     */
    [[nodiscard]] const std::set<CollectiveCondition>& post_conditions() const noexcept;

  protected:
    /// pointer to the underlying topology
    std::shared_ptr<Topology> _topology;

    /**
     * Add (chunk_id, src) to pre-condition and (chunk_id, dest) to post-condition
     * and increase chunks_count if required.
     *
     * @param chunk_id id of the chunk
     * @param src src NPU of the chunk
     * @param dest dest NPU of the chunk
     */
    void add_chunk(ChunkId chunk_id, NpuId src, NpuId dest) noexcept;

  private:
    /// total number of chunks of the collective
    int _chunks_count = 0;

    /// size of each chunk (in MB)
    ChunkSize _chunk_size;

    /// set of all chunk IDs
    std::set<ChunkId> _chunks = {};

    /// pre-conditions: set of (ChunkId, src NPU)
    std::set<CollectiveCondition> _pre_conditions = {};

    /// post-conditions: set of (ChunkId, dest NPU)
    std::set<CollectiveCondition> _post_conditions = {};
};

}  // namespace tacos
