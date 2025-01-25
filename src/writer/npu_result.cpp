/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.

Copyright (c) 2022 Intel Corporation
Copyright (c) 2022 Georgia Institute of Technology
*******************************************************************************/

#include <cassert>
#include <tacos/writer/npu_result.h>

using namespace tacos;

NpuResult::NpuResult(const NpuID id,
                     const std::shared_ptr<Topology> topology) noexcept
    : npusCount_(topology->getNpusCount()),
      id_(id) {

    // check any ingress/egress links
    for (auto npu = 0; npu < npusCount_; npu++) {
        if (npu == id) {
            continue;
        }

        // ingress links
        if (topology->isConnected(npu, id)) {
            ingressLinks_.emplace(npu, LinkResult(LinkType::Ingress, this));
        }

        // egress links
        if (topology->isConnected(id, npu)) {
            egressLinks_.emplace(npu, LinkResult(LinkType::Egress, this));
        }
    }
}

LinkResult& NpuResult::linkFrom(const NpuID id) noexcept {
    assert(0 <= id && id < npusCount_);
    assert(ingressLinks_.find(id) != ingressLinks_.end());

    return ingressLinks_.at(id);
}

LinkResult& NpuResult::linkTo(const NpuID id) noexcept {
    assert(0 <= id && id < npusCount_);
    assert(egressLinks_.find(id) != egressLinks_.end());

    return egressLinks_.at(id);
}

void NpuResult::registerRecvDep(const ChunkID chunk,
                                CommOp* const depOp) noexcept {
    assert(depRecvOp_.find(chunk) == depRecvOp_.end());
    depRecvOp_.emplace(chunk, depOp);
}

CommOp* const NpuResult::getDep(const ChunkID chunk) noexcept {
    assert(depRecvOp_.find(chunk) != depRecvOp_.end());
    return depRecvOp_.at(chunk);
}
