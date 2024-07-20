/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include <cassert>
#include <tacos/topology/topology.h>

using namespace tacos;

Topology::Topology() noexcept = default;

Beta Topology::bandwidth_to_beta(const Bandwidth bandwidth) noexcept {
    assert(bandwidth > 0);

    return 1'000'000 / (bandwidth * 1024);  // GB/s to us/MB
}

void Topology::set_npus_count(const int npus_count) noexcept {
    assert(_npus_count > 0);

    _npus_count = npus_count;

    // initialize data vectors
    _connected = decltype(_connected)(npus_count, std::vector<bool>(npus_count, false));
    _latencies = decltype(_latencies)(npus_count, std::vector<Latency>(npus_count, -1.0));
    _bandwidths = decltype(_bandwidths)(npus_count, std::vector<Bandwidth>(npus_count, -1.0));
    _betas = decltype(_betas)(npus_count, std::vector<Beta>(npus_count, -1.0));
}

int Topology::npus_count() const noexcept {
    assert(_npus_count > 0);

    return _npus_count;
}

void Topology::connect(const NpuId src,
                       const NpuId dest,
                       const Bandwidth bandwidth,
                       const Latency latency,
                       const bool bidirectional) noexcept {
    assert(_npus_count > 0);
    assert(0 <= src && src < _npus_count);
    assert(0 <= dest && dest < _npus_count);
    assert(bandwidth > 0);
    assert(latency >= 0);
    assert(!_connected[src][dest]);  // link already established, potential topology construction error

    // calculate beta
    const auto beta = bandwidth_to_beta(bandwidth);

    // connect src -> dest
    _connected[src][dest] = true;
    _bandwidths[src][dest] = bandwidth;
    _latencies[src][dest] = latency;
    _betas[src][dest] = beta;
    _links_count++;

    if (bidirectional) {
        // connect dest -> src
        _connected[dest][src] = true;
        _latencies[dest][src] = latency;
        _bandwidths[dest][src] = bandwidth;
        _betas[dest][src] = beta;
        _links_count++;
    }
}

Time Topology::transmission_time(const NpuId src, const NpuId dest, const ChunkSize chunk_size) const noexcept {
    assert(_npus_count > 0);
    assert(0 <= src && src < _npus_count);
    assert(0 <= dest && dest < _npus_count);
    assert(chunk_size > 0);
    assert(_connected[src][dest]);

    return _latencies[src][dest] + (_betas[src][dest] * chunk_size);
}

bool Topology::connected(const NpuId src, const NpuId dest) const noexcept {
    assert(_npus_count > 0);
    assert(0 <= src && src < _npus_count);
    assert(0 <= dest && dest < _npus_count);

    return _connected[src][dest];
}

double Topology::link_bandwidth(const NpuId src, const NpuId dest) const noexcept {
    assert(_npus_count > 0);
    assert(0 <= src && src < _npus_count);
    assert(0 <= dest && dest < _npus_count);
    assert(_connected[src][dest]);

    return _bandwidths[src][dest];
}

int Topology::links_count() const noexcept {
    assert(_npus_count > 0);
    assert(_links_count > 0);

    return _links_count;
}

std::vector<NpuId> Topology::backtrack_source_npus(const NpuId dest) const noexcept {
    assert(_npus_count > 0);
    assert(0 <= dest && dest < _npus_count);

    // list of potential source NPUs
    auto source_npus = std::vector<NpuId>();

    // iterate over all NPUs and append if it can be potential source
    for (auto src = 0; src < _npus_count; src++) {
        if (src == dest) {
            // if src == dest, skip this
            continue;
        }

        if (_connected[src][dest]) {
            // potential source NPU
            source_npus.push_back(src);
        }
    }

    return source_npus;
}
