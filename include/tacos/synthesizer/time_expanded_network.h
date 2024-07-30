/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#pragma once

#include <algorithm>
#include <memory>
#include <tacos/collective/collective.h>
#include <tacos/topology/topology.h>
#include <vector>

namespace tacos {
class TimeExpandedNetwork {
  public:
    TimeExpandedNetwork(std::shared_ptr<const Topology> topology,
                        std::shared_ptr<const Collective> collective) noexcept;

    [[nodiscard]] std::set<Time> distinct_link_times() const noexcept;

    [[nodiscard]] bool chunk_arrived(NpuId npu, ChunkId chunk) const noexcept;

    void expand_time(Time next_time) noexcept;

    void reset() noexcept;

    void transmit_chunk(NpuId src, NpuId dest, ChunkId chunk) noexcept;

    void reflect_chunk_arrival_info() noexcept;

    [[nodiscard]] std::vector<NpuId> backtrack_source_npus(NpuId dest) noexcept;

  private:
    struct LinkData {
      public:
        bool exist = false;
        Time transmission_time = -1;

        bool busy = false;
        Time busy_until = 0;
        ChunkId transmitting_chunk = -1;

        void reset_transmission_info() noexcept;
    };

    Time _current_time = 0;  // FIXME: to be used

    const std::shared_ptr<const Topology> _topology;

    const std::shared_ptr<const Collective> _collective;

    std::vector<std::vector<bool>> _arrived;

    std::vector<std::vector<bool>> _arrived_next;

    const int _npus_count;

    const ChunkSize _chunk_size;
    const int _chunks_count;

    std::vector<std::vector<LinkData>> _links;

    void _initialize_link_data() noexcept;

    void _process_link_availability(LinkData* link) const noexcept;

    void _initialize_arrived() noexcept;
};

}  // namespace tacos
