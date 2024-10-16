/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include <cassert>
#include <tacos/writer/npu_result.h>

using namespace tacos;

NpuResult::NpuResult(const int npu,
                     const std::shared_ptr<Topology> topology,
                     const std::shared_ptr<Collective> collective) noexcept
    : npu(npu) {
    assert(topology != nullptr);

    npusCount = topology->getNpusCount();
    chunksCount = collective->getChunksCount();

    for (auto dest = 0; dest < npusCount; dest++) {
        if (npu == dest) {
            continue;
        }

        if (topology->isConnected(npu, dest)) {
            egressLinksInfo[dest] = {};
        }

        if (topology->isConnected(dest, npu)) {
            ingressLinksInfo[dest] = {};
        }
    }
}

void NpuResult::addIngressLinkInfo(ChunkID chunk, NpuID src) noexcept {
    assert(0 <= chunk && chunk < chunksCount);
    assert(0 <= src && src < npusCount);
    assert(ingressLinksInfo.find(src) != ingressLinksInfo.end());

    ingressLinksInfo[src].push_back(chunk);
}

void NpuResult::addEgressLinkInfo(ChunkID chunk, NpuID dest) noexcept {
    assert(0 <= chunk && chunk < chunksCount);
    assert(0 <= dest && dest < npusCount);
    assert(egressLinksInfo.find(dest) != egressLinksInfo.end());

    egressLinksInfo[dest].push_back(chunk);
}
