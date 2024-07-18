/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include "TacosNetwork.h"
#include <cassert>
#include <memory>

using namespace Tacos;

TacosNetwork::TacosNetwork(const std::shared_ptr<Topology> topology, const ChunkSize chunkSize) noexcept
    : topology(topology),
      chunksSize(chunkSize) {
    assert(chunkSize > 0);

    npusCount = topology->getNpusCount();

    // initialize 2D vectors
    linkTimes = decltype(linkTimes)(npusCount, std::vector<Time>(npusCount, -1));
    processingChunks = decltype(processingChunks)(npusCount, std::vector<ChunkId>(npusCount, -1));
    backtrackingTopology = decltype(backtrackingTopology)(npusCount, std::vector<bool>(npusCount, false));

    // initialize 2D backtracking topology
    reset();
}

std::vector<NpuId> TacosNetwork::incomingNpus(const NpuId dest, const bool shuffle) noexcept {
    assert(0 <= dest < npusCount);

    auto incomingNpus = std::vector<NpuId>();

    // check all incoming NPUs
    for (auto src = 0; src < npusCount; src++) {
        if (backtrackingTopology[src][dest]) {
            incomingNpus.push_back(src);
        }
    }

    if (shuffle) {
        std::shuffle(incomingNpus.begin(), incomingNpus.end(), randomEngine);
    }

    return incomingNpus;
}

std::vector<NpuId> TacosNetwork::outgoingNpus(const NpuId src, const bool shuffle) noexcept {
    assert(0 <= src < npusCount);

    auto outgoingNpus = std::vector<NpuId>();

    // check all outgoing NPUs
    for (auto dest = 0; dest < npusCount; dest++) {
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
    assert(0 <= src && src < npusCount);
    assert(0 <= dest && dest < npusCount);

    backtrackingTopology[src][dest] = false;
}

void TacosNetwork::reset() noexcept {
    for (auto src = 0; src < npusCount; src++) {
        for (auto dest = 0; dest < npusCount; dest++) {
            const auto link = LinkId(src, dest);
            backtrackingTopology[src][dest] = topology->getTopologyValue(link);
        }
    }
}

Time TacosNetwork::linkTime(const LinkId link) const noexcept {
    auto [src, dest] = link;

    assert(0 <= src && src < npusCount);
    assert(0 <= dest && dest < npusCount);

    return linkTimes[src][dest];
}

void TacosNetwork::setLinkTime(const LinkId link, const Time time) noexcept {
    const auto [src, dest] = link;

    assert(0 <= src && src < npusCount);
    assert(0 <= dest && dest < npusCount);

    linkTimes[src][dest] = time;
}

ChunkId TacosNetwork::processingChunk(const LinkId link) const noexcept {
    const auto [src, dest] = link;

    assert(0 <= src && src < npusCount);
    assert(0 <= dest && dest < npusCount);

    return processingChunks[src][dest];
}

void TacosNetwork::setProcessingChunk(const LinkId link, const ChunkId chunk) noexcept {
    const auto [src, dest] = link;

    assert(0 <= src && src < npusCount);
    assert(0 <= dest && dest < npusCount);

    processingChunks[src][dest] = chunk;
}
