/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include <cassert>
#include <iostream>
#include <tacos/synthesizer/time_expanded_network.h>

using namespace tacos;

void TimeExpandedNetwork::LinkData::reset_transmission_info() noexcept {
    busy = false;
    busy_until = 0;
    transmitting_chunk = -1;
}

TimeExpandedNetwork::TimeExpandedNetwork(std::shared_ptr<const Topology> topology,
                                         std::shared_ptr<const Collective> collective) noexcept
    : _topology(std::move(topology)),
      _collective(std::move(collective)),
      _npus_count(_topology->npus_count()),
      _chunks_count(_collective->chunks_count()),
      _chunk_size(_collective->chunk_size()) {
    assert(_topology != nullptr);
    assert(_collective != nullptr);
    assert(_npus_count > 0);
    assert(_chunk_size > 0);
    assert(_chunks_count > 0);

    // allocate and initialize link metadata
    _links = decltype(_links)(_npus_count, std::vector<LinkData>(_npus_count, LinkData()));
    _initialize_link_data();

    // initialize the TEN
    reset();
}

std::set<Time> TimeExpandedNetwork::distinct_link_times() const noexcept {
    auto link_times = std::set<Time>();

    // iterate over all links
    for (auto src = 0; src < _npus_count; src++) {
        for (auto dest = 0; dest < _npus_count; dest++) {
            const auto& link = _links[src][dest];

            if (link.exist) {
                // insert link_times
                link_times.insert(link.transmission_time);
            }
        }
    }

    return link_times;
}

bool TimeExpandedNetwork::chunk_arrived(const NpuId npu, const ChunkId chunk) const noexcept {
    assert(0 <= npu && npu < _npus_count);
    assert(0 <= chunk && chunk < _chunks_count);

    // return whether the chunk has arrived the NPU
    return _arrived[chunk][npu];
}

void TimeExpandedNetwork::expand_time(const Time next_time) noexcept {
    assert(next_time > _current_time);

    // update current time
    _current_time = next_time;

    // iterate and expand all TEN links based on their current availability
    for (auto src = 0; src < _npus_count; src++) {
        for (auto dest = 0; dest < _npus_count; dest++) {
            auto* link = &_links[src][dest];
            _process_link_availability(link);
        }
    }
}

std::vector<NpuId> TimeExpandedNetwork::backtrack_source_npus(const NpuId dest) noexcept {
    assert(0 <= dest && dest < _npus_count);

    auto source_npus = std::vector<NpuId>();

    // check all source NPUs that can send a chunk to the dest NPU
    // via a free TEN link
    for (auto src = 0; src < _npus_count; src++) {
        if (src == dest) {
            continue;
        }

        const auto& link = _links[src][dest];
        if (link.exist && !link.busy) {
            source_npus.push_back(src);
        }
    }

    return source_npus;
}

void TimeExpandedNetwork::transmit_chunk(const NpuId src, const NpuId dest, const ChunkId chunk) noexcept {
    assert(0 <= src && src < _npus_count);
    assert(0 <= dest && dest < _npus_count);
    assert(0 <= chunk && chunk < _chunks_count);

    // get the link
    auto& link = _links[src][dest];
    assert(link.exist);
    assert(!link.busy);

    // mark the link as busy transmitting chunk
    link.busy = true;
    link.transmitting_chunk = chunk;

    // mark this link busy until current_time
    link.busy_until = _current_time;

    // mark the chunk as arrived
    _arrived_next[chunk][dest] = true;

    // print out link-chunk match info
    // FIXME: this should be saved as a separate data structure
    const auto transmission_start_time = _current_time - link.transmission_time;
    const auto transmission_end_time = _current_time;
    std::cout << "[Chunk " << chunk << "] " << src << " -> " << dest << " (" << transmission_start_time << " -- "
              << transmission_end_time << ")" << std::endl;
}

void TimeExpandedNetwork::reset() noexcept {
    // set information to be initial state
    _current_time = 0;

    // initialize the arrived chunks
    _initialize_arrived();

    // initialize link information
    _initialize_link_data();
}

void TimeExpandedNetwork::reflect_chunk_arrival_info() noexcept {
    // copy the _arrived_next to _arrived
    _arrived = _arrived_next;
}

void TimeExpandedNetwork::_initialize_link_data() noexcept {
    // iterate over all links
    for (auto src = 0; src < _npus_count; src++) {
        for (auto dest = 0; dest < _npus_count; dest++) {
            auto& link = _links[src][dest];

            // fetch link information
            link.exist = _topology->connected(src, dest);

            // if the link exist,
            // assign this link's chunk transmission time
            if (link.exist) {
                link.transmission_time = _topology->transmission_time(src, dest, _chunk_size);
            } else {
                link.transmission_time = -1;
            }

            // reset the transmission info
            link.reset_transmission_info();

            // initially, all link is busy until t=0
            link.busy = true;
            link.busy_until = 0;
        }
    }
}

void TimeExpandedNetwork::_process_link_availability(LinkData* const link) const noexcept {
    // Check whether a TEN link is available to be used to send a chunk arriving at current_time
    // i.e., backtrack the TEN link's availability

    // Skip if the link doesn't exist
    if (!link->exist) {
        return;
    }

    // Skip the link is already free
    if (!link->busy) {
        return;
    }

    // Case 1: link was busy, and the link is still not available
    // i.e., if you backtrack the transmission start time, it's overlapping with the link's busy_until time
    const auto transmission_start_time = _current_time - link->transmission_time;
    if (transmission_start_time < link->busy_until) {
        return;
    }

    // Case 2: the TEN link was busy till busy_until time, but it's not overlapping and become available again
    // reset the transmission information to make the link available
    link->reset_transmission_info();
}

void TimeExpandedNetwork::_initialize_arrived() noexcept {
    // allocate memory for _arrived, marking everything as false
    _arrived = decltype(_arrived)(_chunks_count, std::vector<bool>(_npus_count, false));
    _arrived_next = decltype(_arrived_next)(_chunks_count, std::vector<bool>(_npus_count, false));

    // mark all pre-conditions as already arrived
    for (const auto& [chunk, src] : _collective->pre_conditions()) {
        _arrived[chunk][src] = true;
        _arrived_next[chunk][src] = true;
    }
}
