/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include <cassert>
#include <iostream>
#include <tacos/synthesizer/tacos.h>

using namespace tacos;

Tacos::Tacos(std::shared_ptr<const Topology> topology, std::shared_ptr<const Collective> collective) noexcept
    : _topology(topology),
      _collective(collective),
      _ten(std::move(topology), std::move(collective)),
      _npus_count(_topology->npus_count()),
      _chunks_count(_collective->chunks_count()),
      _chunk_size(_collective->chunk_size()) {
    assert(_topology != nullptr);
    assert(_collective != nullptr);
    assert(_npus_count > 0);
    assert(_chunks_count > 0);
    assert(_chunk_size > 0);

    // get distinct link times in the topology
    _distinct_link_times = _ten.distinct_link_times();

    // initialize the synthesizer
    reset();
}

Time Tacos::solve() noexcept {
    while (!_event_queue.empty()) {
        // get unsatisfied post-conditions
        auto unsatisfied_post_conditions = _compute_unsatisfied_post_conditions();

        // if there's no unsatisfied post-conditions, the collective synthesis is done
        if (unsatisfied_post_conditions.empty()) {
            break;
        }

        // new matching should be made, update current time
        _current_time = _event_queue.pop();

        // there are remaining unsatisfied post-conditions
        // expand the TEN time till current_time
        _ten.expand_time(_current_time);

        // run the utilization maximization matching algorithm
        _utilization_maximization_matching(unsatisfied_post_conditions);

        // schedule subsequent events for next time
        _schedule_next_events();
    }

    // return the collective time
    return _current_time;
}

void Tacos::reset() noexcept {
    // reset time information
    _current_time = 0;
    _event_queue.reset();

    // reset TEN
    _ten.reset();

    // schedule initial events
    _schedule_next_events();
}

void Tacos::_schedule_next_events() noexcept {
    // schedule next events based on distinct link times
    for (const auto& link_time : _distinct_link_times) {
        const auto next_event_time = _current_time + link_time;
        _event_queue.schedule(next_event_time);
    }
}

std::vector<CollectiveCondition> Tacos::_compute_unsatisfied_post_conditions(const bool shuffle) noexcept {
    auto unsatisfied_post_conditions = std::vector<CollectiveCondition>();

    // iterate over all post-conditions
    for (const auto& [chunk, dest] : _collective->post_conditions()) {
        if (!_ten.chunk_arrived(dest, chunk)) {
            // post-condition is not satisfied
            unsatisfied_post_conditions.emplace_back(chunk, dest);
        }
    }

    // if shuffle is enabled, shuffle the unsatisfied post-conditions
    if (shuffle) {
        // shuffle source_npus if shuffling is enabled
        std::shuffle(unsatisfied_post_conditions.begin(), unsatisfied_post_conditions.end(), _random_engine);
    }

    return unsatisfied_post_conditions;
}

void Tacos::_utilization_maximization_matching(
    const std::vector<CollectiveCondition>& unsatisfied_post_conditions) noexcept {
    // iterate over all unsatisfied post-conditions
    for (const auto& [chunk, dest] : unsatisfied_post_conditions) {
        // get all source NPUs who can send a chunk to the dest NPU
        const auto source_npus = _ten.backtrack_source_npus(dest);

        // among the potential sources, filter out only candidate NPUs
        // i.e., who currently has the chunk
        const auto candidate_npus = _filter_candidate_npus(source_npus, chunk);

        // if there's no candidate, continue to the next unsatisfied post-condition
        if (candidate_npus.empty()) {
            continue;
        }

        // select one candidate source randomly from the candidates
        const auto src = _select_npu(candidate_npus);

        // transmit the chunk from src to dest
        _ten.transmit_chunk(src, dest, chunk);
    }

    // matching for this timestep done, update the chunk arrival info
    std::cout << std::endl;
    _ten.reflect_chunk_arrival_info();
}

std::vector<NpuId> Tacos::_filter_candidate_npus(const std::vector<NpuId>& source_npus,
                                                 const ChunkId chunk) const noexcept {
    assert(0 <= chunk && chunk < _chunks_count);

    auto candidate_npus = std::vector<NpuId>();

    // check whether each src NPU has the chunk
    for (const auto src : source_npus) {
        if (_ten.chunk_arrived(src, chunk)) {
            candidate_npus.push_back(src);
        }
    }

    return candidate_npus;
}

NpuId Tacos::_select_npu(const std::vector<NpuId>& candidate_npus) noexcept {
    assert(!candidate_npus.empty());

    // if there's only one candidate NPU, return it
    if (candidate_npus.size() == 1) {
        return candidate_npus[0];
    }

    // if there are multiple candidate NPUs, select one candidate source NPU randomly
    auto distribution = std::uniform_int_distribution<size_t>(0, candidate_npus.size() - 1);
    const auto random_idx = distribution(_random_engine);

    return candidate_npus[random_idx];
}
