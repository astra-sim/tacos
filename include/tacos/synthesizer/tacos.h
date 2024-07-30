/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#pragma once

#include <array>
#include <memory>
#include <random>
#include <tacos/collective/collective.h>
#include <tacos/event_queue/event_queue.h>
#include <tacos/synthesizer/time_expanded_network.h>
#include <tacos/topology/topology.h>

namespace tacos {
class Tacos {
  public:
    Tacos(std::shared_ptr<const Topology> topology, std::shared_ptr<const Collective> collective) noexcept;

    [[nodiscard]] Time solve() noexcept;

    void reset() noexcept;

  private:
    const std::shared_ptr<const Topology> _topology;
    const std::shared_ptr<const Collective> _collective;

    TimeExpandedNetwork _ten;

    const int _npus_count;
    const int _chunks_count;
    const ChunkSize _chunk_size;

    Time _current_time = 0;

    EventQueue _event_queue = {};

    std::set<Time> _distinct_link_times = {};

    std::random_device _random_device = {};
    std::mt19937 _random_engine = decltype(_random_engine)(_random_device());

    void _schedule_next_events() noexcept;

    [[nodiscard]] std::vector<CollectiveCondition> _compute_unsatisfied_post_conditions(bool shuffle = true) noexcept;

    void _utilization_maximization_matching(
        const std::vector<CollectiveCondition>& unsatisfied_post_conditions) noexcept;

    [[nodiscard]] std::vector<NpuId> _filter_candidate_npus(const std::vector<NpuId>& source_npus,
                                                            ChunkId chunk) const noexcept;

    [[nodiscard]] NpuId _select_npu(const std::vector<NpuId>& candidate_npus) noexcept;
};

}  // namespace tacos
