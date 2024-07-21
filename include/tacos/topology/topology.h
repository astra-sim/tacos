/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#pragma once

#include <tacos/event_queue/event_queue.h>

#include <vector>

namespace tacos {

/// NPU id = 0, 1, 2, ...
using NpuId = int;

/// Topology representation
using Latency = double;    // us
using Bandwidth = double;  // GB/s
using Beta = double;       // us/MB

/// Chunk Size = MB
using ChunkSize = double;

class Topology {
  public:
    /**
     * Create new base topology without any NPU or link connectivity.
     */
    Topology() noexcept;

    /**
     * Get the number of NPUs in the topology.
     *
     * @return number of NPUs
     */
    [[nodiscard]] int npus_count() const noexcept;

    /**
     * For an existing link src -> dest,
     * calculate the time to send a chunk of chunk_size in us
     * = latency + (beta * chunk_size)
     *
     * @param src src NPU id
     * @param dest dest NPU id
     * @param chunk_size
     *
     * @return transmission time of chunk_size from src -> dest (in us)
     */
    [[nodiscard]] Time link_busy_until(NpuId src, NpuId dest, ChunkSize chunk_size) const noexcept;

    /**
     * Check if src -> dest link exists.
     *
     * @param src src NPU id
     * @param dest dest NPU id
     *
     * @return true if link src -> dest exists, false if not.
     */
    [[nodiscard]] bool connected(NpuId src, NpuId dest) const noexcept;

    /**
     * Get bandwidth of an existing link.
     *
     * @param src src NPU id
     * @param dest dest NPU id
     *
     * @return link bandwidth in GB/s
     */
    [[nodiscard]] double link_bandwidth(NpuId src, NpuId dest) const noexcept;

    /**
     * Get number of links in the topology.
     *
     * @return links_count
     */
    [[nodiscard]] int links_count() const noexcept;

    /**
     * Backtrack all NPUs that can send data to dest.
     *
     * @param dest dest NPU id
     *
     * @return list of source NPUs that can send data to dest
     */
    [[nodiscard]] std::vector<NpuId> backtrack_source_npus(NpuId dest) const noexcept;

  protected:
    /// number of NPUs in the topology
    int _npus_count = -1;

    /**
     * Set npus_count to a specific value,
     * and initialize topology, latency, and bandwidth vectors
     *
     * This should be invoked before connect() or other methods or called.
     *
     * @param npus_count new npus_count of the topology
     */
    void set_npus_count(int npus_count) noexcept;

    /**
     * Create a link from src -> dest, with bandwidth and latency.
     * If bidirectional = true, also create dest -> src link.
     *
     * @param src src NPU id
     * @param dest dest NPU id
     * @param bandwidth bandwidth in GB/s
     * @param latency latency in us
     * @param bidirectional true to also establish dest -> src link, false otherwise
     */
    void connect(NpuId src, NpuId dest, Bandwidth bandwidth, Latency latency, bool bidirectional = false) noexcept;

  private:
    /**
     * Convert bandwidth (GB/s) into beta (us/MB).
     *
     * @param bandwidth bandwidth in GB/s
     *
     * @return beta in us/MB
     */
    static Beta bandwidth_to_beta(Bandwidth bandwidth) noexcept;

    /// number of (unidirectional) links in the topology
    int _links_count = 0;

    /// whether a link (src -> dest) exists or not
    std::vector<std::vector<bool>> _connected;

    /// link latency (in us)
    std::vector<std::vector<Latency>> _latencies;

    /// link bandwidth (in GB/s)
    std::vector<std::vector<Bandwidth>> _bandwidths;

    /// link beta (reciprocal of bandwidth), i.e., MB/us
    std::vector<std::vector<Beta>> _betas;
};
}  // namespace tacos
