/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#pragma once

#include <memory>
#include <tacos/collective/collective.h>
#include <tacos/topology/topology.h>

namespace tacos {

class AllGather final : public Collective {
  public:
    /**
     * All-Gather collective communication.
     *
     * @param topology target topology to run All-Gather
     * @param chunk_size each chunk size in MB
     * @param chunks_per_collective how many initial chunks (per NPU) to generate.
     */
    explicit AllGather(std::shared_ptr<Topology> topology,
                       ChunkSize chunk_size = 1,
                       int chunks_per_collective = 1) noexcept;

  private:
    /// number of NPUs in the target topology
    int npus_count;

    /**
     * Generate chunks for a single All-Gather,
     * start from start_chunk_id,
     * then return the next chunk id to be used.
     *
     * @param start_chunk_id starting chunk id
     *
     * @return next chunk id to be used
     */
    [[nodiscard]] int generate_all_gather(int start_chunk_id) noexcept;
};

}  // namespace tacos
