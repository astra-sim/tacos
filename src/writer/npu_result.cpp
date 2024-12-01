/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.

Copyright (c) 2022 Intel Corporation
Copyright (c) 2022 Georgia Institute of Technology
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

    // initialize dependencyInfo
    for (auto chunk = 0; chunk < chunksCount; chunk++) {
        dependencyInfo[chunk] = std::nullopt;
    }
}

void NpuResult::addIngressLinkInfo(ChunkID chunk, NpuID src) noexcept {
    assert(0 <= chunk && chunk < chunksCount);
    assert(0 <= src && src < npusCount);
    assert(ingressLinksInfo.find(src) != ingressLinksInfo.end());

    ingressLinksInfo[src].push_back(chunk);

    // mark dependency info
    const auto opIdx = ingressLinksInfo[src].size() - 1;
    dependencyInfo[chunk] = opIdx;
}

void NpuResult::addEgressLinkInfo(ChunkID chunk, NpuID dest) noexcept {
    assert(0 <= chunk && chunk < chunksCount);
    assert(0 <= dest && dest < npusCount);
    assert(egressLinksInfo.find(dest) != egressLinksInfo.end());

    egressLinksInfo[dest].push_back(chunk);
}

std::vector<NpuResult::ChunkID> NpuResult::getIngressLinkInfo(
    const NpuID src) const noexcept {
    assert(0 <= src && src < npusCount);

    if (ingressLinksInfo.find(src) == ingressLinksInfo.end()) {
        return {};
    }

    return ingressLinksInfo.at(src);
}

std::vector<NpuResult::ChunkID> NpuResult::getEgressLinkInfo(
    const NpuID dest) const noexcept {
    assert(0 <= dest && dest < npusCount);

    if (egressLinksInfo.find(dest) == egressLinksInfo.end()) {
        return {};
    }

    return egressLinksInfo.at(dest);
}
