/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include <cassert>
#include <tacos/Topology.h>

using namespace Tacos;

Topology::Topology() noexcept : npusCount(0), linksCount(0) {}

void Topology::setNpusCount(const int newNpusCount) noexcept {
    assert(newNpusCount > 0);

    npusCount = newNpusCount;

    // allocate memory
    topology = decltype(topology)(npusCount, std::vector<bool>(npusCount, false));
    alpha = decltype(alpha)(npusCount, std::vector<LinkWeight>(npusCount, -1.0));
    beta = decltype(beta)(npusCount, std::vector<LinkWeight>(npusCount, -1.0));
}

std::vector<NpuId> Topology::incomingNpus(const NpuId dest) const noexcept {
    assert(0 <= dest && dest < npusCount);

    auto incomingNpus = std::vector<NpuId>();

    // iterate over src, get NPUs connected to dest
    for (auto src = 0; src < npusCount; src++) {
        if (topology[src][dest]) {
            incomingNpus.push_back(src);
        }
    }

    return incomingNpus;
}

void Topology::connect(const NpuId src,
                       const NpuId dest,
                       const LinkAlphaBeta linkAlphaBeta,
                       const bool bidirectional) noexcept {
    assert(0 <= src && src < npusCount);
    assert(0 <= dest && dest < npusCount);
    assert(linkAlphaBeta.first >= 0);
    assert(linkAlphaBeta.second >= 0);

    // connect src -> dest
    topology[src][dest] = true;
    alpha[src][dest] = linkAlphaBeta.first;
    beta[src][dest] = linkAlphaBeta.second;
    linksCount++;

    if (bidirectional) {
        // connect dest -> src (if bi-directional)
        topology[dest][src] = true;
        alpha[dest][src] = linkAlphaBeta.first;
        beta[dest][src] = linkAlphaBeta.second;
        linksCount++;
    }
}

bool Topology::connected(const NpuId src, const NpuId dest) const noexcept {
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

double Topology::getBW(NpuId src, NpuId dest) {
    const auto b = beta[src][dest];
    const auto bb = 1 / b;                  // MB/us
    const auto bbb = bb * 1e6 / (1 << 10);  // GB/s
    return bbb;
}

int Topology::getLinksCount() const noexcept {
    return linksCount;
}
