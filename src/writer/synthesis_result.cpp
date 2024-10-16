/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include <cassert>
#include <tacos/writer/synthesis_result.h>

using namespace tacos;

SynthesisResult::SynthesisResult(const std::shared_ptr<Topology> topology,
                                 const std::shared_ptr<Collective> collective) noexcept {
    assert(topology != nullptr);
    assert(collective != nullptr);

    npusCount = topology->getNpusCount();
    chunksCount = collective->getChunksCount();

    // initialize each npu information
    for (auto npu = 0; npu < npusCount; npu++) {
        npuResults.emplace_back(npu, topology, collective);
    }
}

void SynthesisResult::markLinkChunkMatch(const ChunkID chunk,
                                         const NpuID src,
                                         const NpuID dest) noexcept {
    assert(0 <= chunk && chunk < chunksCount);
    assert(0 <= src && src < npusCount);
    assert(0 <= dest && dest < npusCount);

    // egress: src -> dest
    npuResults[src].addEgressLinkInfo(chunk, dest);

    // ingress: dest <- src
    npuResults[dest].addIngressLinkInfo(chunk, src);
}

void SynthesisResult::setCollectiveTime(Time newCollectiveTime) noexcept {
    collectiveTime = newCollectiveTime;
}

SynthesisResult::Time SynthesisResult::getCollectiveTime() const noexcept {
    return collectiveTime;
}

std::vector<SynthesisResult::ChunkID> SynthesisResult::getEgressLinkInfo(
    const NpuID src, const NpuID dest) const noexcept {
    assert(0 <= src && src < npusCount);
    assert(0 <= dest && dest < npusCount);

    return npuResults[src].getEgressLinkInfo(dest);
}
