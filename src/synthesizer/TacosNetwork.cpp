/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include <cassert>
#include <memory>
#include <tacos/TacosNetwork.h>

using namespace tacos;

TacosNetwork::TacosNetwork(const std::shared_ptr<Topology> topology, const ChunkSize chunkSize) noexcept
    : topology(topology),
      chunksSize(chunkSize) {
    assert(chunkSize > 0);

    npus_count = topology->npus_count();

    // initialize 2D vectors
    linkTimes = decltype(linkTimes)(npus_count, std::vector<EventQueue::Time>(npus_count, -1));
    processingChunks = decltype(processingChunks)(npus_count, std::vector<ChunkId>(npus_count, -1));
    backtrackingTopology = decltype(backtrackingTopology)(npus_count, std::vector<bool>(npus_count, false));

    // initialize 2D backtracking topology
    reset();
}

std::vector<Topology::NpuId> TacosNetwork::backtrack_source_npus(const NpuId dest, const bool shuffle) noexcept {
    assert(0 <= dest < npus_count);

    auto incomingNpus = std::vector<NpuId>();

    // check all incoming NPUs
    for (auto src = 0; src < npus_count; src++) {
        if (backtrackingTopology[src][dest]) {
            incomingNpus.push_back(src);
        }
    }

    if (shuffle) {
        std::shuffle(incomingNpus.begin(), incomingNpus.end(), randomEngine);
    }

    return incomingNpus;
}

std::vector<Topology::NpuId> TacosNetwork::outgoingNpus(const NpuId src, const bool shuffle) noexcept {
    assert(0 <= src < npus_count);

    auto outgoingNpus = std::vector<NpuId>();

    // check all outgoing NPUs
    for (auto dest = 0; dest < npus_count; dest++) {
        if (backtrackingTopology[src][dest]) {
            outgoingNpus.push_back(dest);
        }
    }

    if (shuffle) {
        std::shuffle(outgoingNpus.begin(), outgoingNpus.end(), randomEngine);
    }

    return outgoingNpus;
}

void TacosNetwork::removeLink(const LinkId link) noexcept {
    const auto [src, dest] = link;
    assert(0 <= src && src < npus_count);
    assert(0 <= dest && dest < npus_count);

    backtrackingTopology[src][dest] = false;
}

void TacosNetwork::reset() noexcept {
    for (auto src = 0; src < npus_count; src++) {
        for (auto dest = 0; dest < npus_count; dest++) {
            backtrackingTopology[src][dest] = topology->connected(src, dest);
        }
    }
}

EventQueue::Time TacosNetwork::transmission_time(const LinkId link) const noexcept {
    auto [src, dest] = link;

    assert(0 <= src && src < npus_count);
    assert(0 <= dest && dest < npus_count);

    return linkTimes[src][dest];
}

void TacosNetwork::setLinkTime(const LinkId link, const EventQueue::Time time) noexcept {
    const auto [src, dest] = link;

    assert(0 <= src && src < npus_count);
    assert(0 <= dest && dest < npus_count);

    linkTimes[src][dest] = time;
}

ChunkId TacosNetwork::processingChunk(const LinkId link) const noexcept {
    const auto [src, dest] = link;

    assert(0 <= src && src < npus_count);
    assert(0 <= dest && dest < npus_count);

    return processingChunks[src][dest];
}

void TacosNetwork::setProcessingChunk(const LinkId link, const ChunkId chunk) noexcept {
    const auto [src, dest] = link;

    assert(0 <= src && src < npus_count);
    assert(0 <= dest && dest < npus_count);

    processingChunks[src][dest] = chunk;
}
