/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include <cassert>
#include <limits>
#include <tacos/topology/topology.h>

using namespace tacos;

Topology::Topology() noexcept = default;

void Topology::setNpusCount(const int newNpusCount) noexcept {
    assert(!npusCountSet);
    assert(newNpusCount > 0);

    npusCount = newNpusCount;
    npusCountSet = true;

    // allocate memory
    connected.resize(npusCount, std::vector(npusCount, false));
    latencies.resize(npusCount, std::vector<Latency>(npusCount, -1));
    bandwidths.resize(npusCount, std::vector<Bandwidth>(npusCount, -1));
    linkDelays.resize(npusCount,
                      std::vector<Time>(npusCount, std::numeric_limits<uint64_t>::max()));
}

void Topology::connect(const NpuID src,
                       const NpuID dest,
                       const Latency latency,
                       const Bandwidth bandwidth,
                       const bool bidirectional) noexcept {
    assert(0 <= src && src < npusCount);
    assert(0 <= dest && dest < npusCount);
    assert(src != dest);
    assert(latency >= 0);
    assert(bandwidth > 0);
    assert(!connected[src][dest]);

    // connect src -> dest
    connected[src][dest] = true;
    latencies[src][dest] = latency;
    bandwidths[src][dest] = bandwidth;

    // if bidirectional, connect dest -> src
    if (bidirectional) {
        connect(dest, src, latency, bandwidth, false);
    }
}

bool Topology::isConnected(const NpuID src, const NpuID dest) const noexcept {
    assert(npusCountSet);
    assert(0 <= src && src < npusCount);
    assert(0 <= dest && dest < npusCount);

    return connected[src][dest];
}

void Topology::setChunkSize(const ChunkSize newChunkSize) noexcept {
    assert(!chunkSizeSet);
    assert(newChunkSize > 0);

    // set chunk size
    chunkSize = newChunkSize;
    chunkSizeSet = true;

    // calculate link delays
    for (auto src = 0; src < npusCount; src++) {
        for (auto dest = 0; dest < npusCount; dest++) {
            if (!connected[src][dest]) {
                continue;
            }

            const auto linkDelay = computeLinkDelay(src, dest);
            linkDelays[src][dest] = linkDelay;
            distinctLinkDelays.insert(linkDelay);
        }
    }
}

const std::set<Topology::Time>& Topology::getDistinctLinkDelays() const noexcept {
    assert(chunkSizeSet);

    return distinctLinkDelays;
}

Topology::Time Topology::computeLinkDelay(const NpuID src, const NpuID dest) const noexcept {
    assert(npusCountSet);
    assert(chunkSizeSet);

    assert(0 <= src && src < npusCount);
    assert(0 <= dest && dest < npusCount);

    // calculate beta (ns/B)
    const auto bandwidthBytesPerNS = bandwidths[src][dest] * (1 << 30) / 1e9;
    const auto beta = 1 / bandwidthBytesPerNS;

    // calculate link delay using alpha-beta model
    const auto linkDelayNS = latencies[src][dest] + (beta * chunkSize);

    // convert linkDelay to ps
    const auto linkDelayPS = linkDelayNS * 1e3;

    // return linkDelayPS (in Time format)
    return static_cast<Time>(linkDelayPS);
}

int Topology::getNpusCount() const noexcept {
    assert(npusCountSet);

    return npusCount;
}

std::set<Topology::NpuID> Topology::backtrackSourceNpus(const NpuID dest,
                                                        const Time currentTime) const noexcept {
    assert(npusCountSet);
    assert(chunkSizeSet);
    assert(0 <= dest && dest < npusCount);

    auto sourceNpus = std::set<NpuID>();

    // iterate over src, get NPUs connected to dest
    for (auto src = 0; src < npusCount; src++) {
        if (!connected[src][dest]) {
            continue;
        }

        // check if the backtracked delay is valid, i.e., >= 0
        const auto linkDelay = linkDelays[src][dest];
        const auto backtrackedDelay = currentTime - linkDelay;
        if (backtrackedDelay < 0) {
            continue;
        }

        // valid src, add to sourceNpus
        sourceNpus.insert(src);
    }

    return sourceNpus;
}
