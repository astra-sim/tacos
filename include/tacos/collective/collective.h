/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.

Copyright (c) 2022-2025 Intel Corporation
Copyright (c) 2022-2025 Georgia Institute of Technology
*******************************************************************************/

#pragma once

#include <tacos/topology/topology.h>
#include <unordered_map>
#include <unordered_set>

namespace tacos {

/// @brief Abstract base class for collective communication patterns
class Collective {
  public:
    // data types
    /// @brief Chunk ID
    using ChunkID = int;

    /// @brief chunk size: in bytes
    using ChunkSize = int64_t;

    using NpuID = Topology::NpuID;

    /// @brief Base class constructor for collective pattern
    Collective() noexcept;

    /// @brief Return the source NPU for a given chunk
    /// @param chunk chunk ID
    /// @return source NPU of the chunk
    [[nodiscard]] NpuID precondition(ChunkID chunk) const noexcept;

    /// @brief Return the destination NPUs for a given chunk
    /// @param chunk chunk ID
    /// @return destination NPUs of the chunk
    [[nodiscard]] const std::unordered_set<NpuID>& postcondition(ChunkID chunk) const noexcept;

    /// @brief Get the number of chunks in this collective
    /// @return number of chunks in the collective pattern
    [[nodiscard]] int chunksCount() const noexcept;

  protected:
    /// @brief Number of chunks in the collective
    int chunksCount_ = 0;

    /// @brief Insert new precondition and postcondition for a chunk
    void chunk_(NpuID src, std::unordered_set<NpuID> dests) noexcept;

  private:
    /// @brief New Chunk ID to be used
    int newChunkID_ = 0;

    /// @brief Precondition: maps each chunk to the (single) source NPU
    std::unordered_map<ChunkID, NpuID> precondition_ = {};

    /// @brief Postcondition: maps each chunk to (multiple) destination NPUs
    std::unordered_map<ChunkID, std::unordered_set<NpuID>> postcondition_ = {};
};
}  // namespace tacos
