/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#pragma once

#include <algorithm>
#include <memory>
#include <random>
#include <tacos/collective/collective.h>
#include <tacos/topology/topology.h>
#include <vector>

namespace tacos {
class TimeExpandedNetwork {
  public:
    TimeExpandedNetwork(std::shared_ptr<Topology> topology) noexcept;

    void expand_time(Time next_time) noexcept;

    /**
     * Set a TEN link to be busy, i.e., occupied by a chunk.
     *
     * @param src src NPU id
     * @param dest dest NPU id
     */
    void set_link_busy(NpuId src, NpuId dest) noexcept;

    Time link_busy_until(NpuId src, NpuId dest) const noexcept;

    void mark_link_busy_until(NpuId src, NpuId dest, Time time) noexcept;

    /**
     * Backtrack all source NPUs that can send a chunk to the dest NPU.
     * i.e., return all potential src NPUs who has free TEN link to dest at the current time step
     *
     * If shuffle is true, the order of the NPUs will be shuffled.
     *
     * @param dest dest NPU id
     * @param shuffle true if shuffle the resulting NPU vector order, false if not (i.e., ascending order)
     *
     * @return vector of source NPUs who has free TEN link to dest
     */
    [[nodiscard]] std::vector<NpuId> backtrack_source_npus(NpuId dest, bool shuffle = true) noexcept;

    ChunkId transmitting_chunk(NpuId src, NpuId dest) const noexcept;

    void transmit_chunk(NpuId src, NpuId dest, ChunkId chunk) noexcept;

    /**
     * Expand the time-expanded network by one time unit.
     * i.e., reset all links to be free.
     */
    void reset() noexcept;

  private:
    struct LinkData {
      public:
        bool exist = false;
        bool busy = false;
        Time busy_until = -1;
        ChunkId transmitting_chunk = -1;
    };

    Time _current_time = 0;  // FIXME: to be used

    std::shared_ptr<Topology> _topology;

    int _npus_count;

    std::vector<std::vector<LinkData>> _links;

    std::random_device _randomDevice = {};
    std::default_random_engine _random_engine = decltype(_random_engine)(_randomDevice());

    void _initialize_link_data() noexcept;

    void _process_ten_link_expansion(LinkData* link) const noexcept;
};

}  // namespace tacos
