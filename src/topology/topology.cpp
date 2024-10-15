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
    assert(newNpusCount > 0);

    npusCount = newNpusCount;

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

std::vector<NpuID> Topology::incomingNpus(const NpuID dest) const noexcept {
    assert(0 <= dest && dest < npusCount);

    auto incomingNpus = std::vector<NpuID>();

    // iterate over src, get NPUs connected to dest
    for (auto src = 0; src < npusCount; src++) {
        if (topology[src][dest]) {
            incomingNpus.push_back(src);
        }
    }

    return incomingNpus;
}

bool Topology::isConnected(const NpuID src, const NpuID dest) const noexcept {
    assert(0 <= src && src < npusCount);
    assert(0 <= dest && dest < npusCount);

    return topology[src][dest];
}

int Topology::getNpusCount() const noexcept {
    return npusCount;
}

Time Topology::linkTime(const LinkId link, const ChunkSize chunkSize) const noexcept {
    const auto [src, dest] = link;

    assert(0 <= src && src < npusCount);
    assert(0 <= dest && dest < npusCount);
    assert(chunkSize > 0);

    return alpha[src][dest] + (beta[src][dest] * chunkSize);
}

bool Topology::getTopologyValue(const LinkId link) const noexcept {
    const auto [src, dest] = link;

    assert(0 <= src && src < npusCount);
    assert(0 <= dest && dest < npusCount);

    return topology[src][dest];
}

double Topology::getBW(NpuID src, NpuID dest) {
    const auto b = beta[src][dest];
    const auto bb = 1 / b;                  // MB/us
    const auto bbb = bb * 1e6 / (1 << 10);  // GB/s
    return bbb;
}

int Topology::getLinksCount() const noexcept {
    return linksCount;
}
