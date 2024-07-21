/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include <cassert>
#include <memory>
#include <tacos/synthesizer/time_expanded_network.h>

using namespace tacos;

TimeExpandedNetwork::TimeExpandedNetwork(std::shared_ptr<Topology> topology) noexcept : _topology(std::move(topology)) {
    assert(_topology != nullptr);

    _npus_count = _topology->npus_count();
    assert(_npus_count > 0);

    // allocate LinkData memory
    _links = decltype(_links)(_npus_count, std::vector<LinkData>(_npus_count, LinkData()));

    // initialize LinkData
    _initialize_link_data();

    // initialize 2D backtracking_topology
    reset();
}

void TimeExpandedNetwork::reset() noexcept {
    // reset all links to be free
    // i.e., recover the original topology connectivity
    for (auto src = 0; src < _npus_count; src++) {
        for (auto dest = 0; dest < _npus_count; dest++) {

            auto& link = _links[src][dest];
            link.busy = false;
        }
    }
}

void TimeExpandedNetwork::set_link_busy(const NpuId src, const NpuId dest) noexcept {
    assert(0 <= src && src < _npus_count);
    assert(0 <= dest && dest < _npus_count);

    auto& link = _links[src][dest];
    assert(link.exist);
    assert(!link.busy);

    link.busy = true;
}

std::vector<NpuId> TimeExpandedNetwork::backtrack_source_npus(const NpuId dest, const bool shuffle) noexcept {
    // FIXME: this should check the current_time, if backtracked, < 0
    // FIXME: if so, remove from the potential source.

    assert(0 <= dest < _npus_count);

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

    if (shuffle) {
        // shuffle source_npus if shuffling is enabled
        std::shuffle(source_npus.begin(), source_npus.end(), _random_engine);
    }

    return source_npus;
}

Time TimeExpandedNetwork::link_busy_until(const NpuId src, const NpuId dest) const noexcept {
    assert(0 <= src && src < _npus_count);
    assert(0 <= dest && dest < _npus_count);

    const auto& link = _links[src][dest];
    assert(link.exist);

    return link.busy_until;
}

void TimeExpandedNetwork::mark_link_busy_until(const NpuId src, const NpuId dest, const Time time) noexcept {
    assert(0 <= src && src < _npus_count);
    assert(0 <= dest && dest < _npus_count);

    auto& link = _links[src][dest];
    assert(link.exist);

    link.busy_until = time;
}

ChunkId TimeExpandedNetwork::transmitting_chunk(const NpuId src, const NpuId dest) const noexcept {
    assert(0 <= src && src < _npus_count);
    assert(0 <= dest && dest < _npus_count);

    const auto& link = _links[src][dest];
    assert(link.exist);

    return link.transmitting_chunk;
}

void TimeExpandedNetwork::transmit_chunk(const NpuId src, const NpuId dest, const ChunkId chunk) noexcept {
    assert(0 <= src && src < _npus_count);
    assert(0 <= dest && dest < _npus_count);

    auto& link = _links[src][dest];
    assert(link.exist);

    link.transmitting_chunk = chunk;
}

void TimeExpandedNetwork::_initialize_link_data() noexcept {
    // iterate over all links
    for (auto src = 0; src < _npus_count; src++) {
        for (auto dest = 0; dest < _npus_count; dest++) {
            auto& link = _links[src][dest];

            link.exist = _topology->connected(src, dest);
            link.busy = false;
            link.busy_until = -1;
            link.transmitting_chunk = -1;
        }
    }
}

void TimeExpandedNetwork::expand_time(Time next_time) noexcept {
    assert(next_time > _current_time);

    // update current time
    _current_time = next_time;

    // iterate and expand all TEN links
    for (auto src = 0; src < _npus_count; src++) {
        for (auto dest = 0; dest < _npus_count; dest++) {
            auto* link = &_links[src][dest];
            _process_ten_link_expansion(link);
        }
    }
}

void TimeExpandedNetwork::_process_ten_link_expansion(LinkData* link) const noexcept {
    // Case 1: Link is free, do nothing
    if (!link->busy) {
        return;
    }

    // Case 2: Link is busy, still transmitting chunk, do nothing
    if (_current_time < link->busy_until) {
        return;
    }

    // Case 3: Link is busy, but the transmission finished. For this case:
    //   1. Print the transmission has finished
    //   2. Reset the TEN link
    // TODO: print the transmission has ended

    // reset the TEN link
    link->busy = false;
    link->busy_until = -1;
    link->transmitting_chunk = -1;
}
