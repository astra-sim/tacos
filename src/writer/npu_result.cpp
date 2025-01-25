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

    // construct ingress links
    for (auto npu = 0; npu < npusCount_; npu++) {
        if (npu == id) {
            continue;
        }

        if (topology->isConnected(npu, id)) {
            ingressLinks_.emplace(npu, LinkResult(nextLinkID_, LinkType::Ingress, this));
            nextLinkID_++;
        }
    }

    // construct egress links
    for (auto npu = 0; npu < npusCount_; npu++) {
        if (npu == id) {
            continue;
        }

        if (topology->isConnected(id, npu)) {
            egressLinks_.emplace(npu, LinkResult(nextLinkID_, LinkType::Egress, this));
            nextLinkID_++;
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

const std::map<NpuResult::NpuID, LinkResult>& NpuResult::ingressLinks() const noexcept {
    return ingressLinks_;
}

const std::map<NpuResult::NpuID, LinkResult>& NpuResult::egressLinks() const noexcept {
    return egressLinks_;
}
