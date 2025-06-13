/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.

Copyright (c) 2022-2025 Intel Corporation
Copyright (c) 2022-2025 Georgia Institute of Technology
*******************************************************************************/

#pragma once

#include <cstdint>
#include <tacos/event_queue/event_queue.h>
#include <unordered_map>
#include <vector>

namespace tacos {

class Topology {
  public:
    // data types
    using Time = EventQueue::Time;

    /// @brief NPU ID
    using NpuID = int;

    /// @brief link bandwidth: GiB/sec
    using Bandwidth = double;

    /// @brief link latency: microseconds (us)
    using Latency = double;

    /// @brief Default constructor
    Topology() noexcept;

    /// @brief Get the bandwidth of a link
    /// @param src source NPU ID
    /// @param dest destination NPU ID
    /// @return bandwidth of the link
    [[nodiscard]] Bandwidth bandwidth(NpuID src, NpuID dest) const noexcept;

    /// @brief Get the latency of a link
    /// @param src source NPU ID
    /// @param dest destination NPU ID
    /// @return latency of the link
    [[nodiscard]] Latency latency(NpuID src, NpuID dest) const noexcept;

    /// @brief Backtrack a destination NPU to find all NPUs that can send a chunk to it
    /// @param dest destination NPU ID
    /// @return set of source NPU IDs that can send a chunk to the destination NPU
    [[nodiscard]] std::vector<NpuID> backtrack(NpuID dest) const noexcept;

    /// @brief Get the number of NPUs in the topology
    /// @return number of NPUs
    [[nodiscard]] int npusCount() const noexcept;

    /// @brief Check if a link exists between two NPUs
    /// @param src src NPU ID
    /// @param dest dest NPU ID
    /// @return true if a link exists, false otherwise
    [[nodiscard]] bool connected(NpuID src, NpuID dest) const noexcept;

  protected:
    /// @brief number of NPUs in the topology
    int npusCount_ = -1;

    /// @brief set the number of NPUs in the topology
    /// @param npusCount number of NPUs
    void setNpusCount_(int npusCount) noexcept;

    /// @brief Establish a connection (i.e., add a link) between two NPUs
    /// @param src src NPU
    /// @param dest dest NPU
    /// @param bandwidth bandwidth of the link (in GiB/sec)
    /// @param latency latency of the link (in microseconds)
    /// @param bidirectional if true, create dest -> src link as well
    void connect_(NpuID src,
                  NpuID dest,
                  Bandwidth bandwidth,
                  Latency latency,
                  bool bidirectional = false) noexcept;

  private:
    /// @brief true if a link exists between src and dest
    std::vector<std::vector<bool>> connected_ = {};

    /// @brief link bandwidth of src -> dest (in GiB/sec)
    std::vector<std::vector<Bandwidth>> bandwidths_ = {};

    /// @brief link latency of src -> dest (in microseconds)
    std::vector<std::vector<Latency>> latencies_ = {};

    /// @brief set of NPUs that can send a chunk to a given NPU
    std::unordered_map<NpuID, std::vector<NpuID>> backtrackMap_ = {};
};
}  // namespace tacos
