/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.

Copyright (c) 2022 Intel Corporation
Copyright (c) 2022 Georgia Institute of Technology
*******************************************************************************/

#include <cassert>
#include <tacos/writer/link_result.h>
#include <tacos/writer/npu_result.h>

using namespace tacos;

LinkResult::LinkID LinkResult::nextID_ = 0;

LinkResult::LinkID LinkResult::nextID() noexcept {
    const auto next_id = nextID_;
    nextID_++;

    return next_id;
}

LinkResult::LinkResult(const LinkType type, NpuResult* const npu) noexcept
    : id_(nextID()),
      type_(type),
      npu_(*npu) {}

LinkResult::LinkID LinkResult::id() const noexcept {
    return id_;
}

void LinkResult::send(const ChunkID chunk) noexcept {
    assert(type_ == LinkType::Egress);

    // create op
    const auto opID = currentOpID();
    ops_.emplace(opID, CommOp(chunk, id(), opID));
    auto& op = ops_.at(opID);

    // check and set dependent op
    auto* const depOp = npu_.getDep(chunk);
    if (depOp == nullptr) {
        // initial chunk
        return;
    }
    op.setDepOp(depOp);
}

void LinkResult::recv(const ChunkID chunk) noexcept {
    assert(type_ == LinkType::Ingress);

    // schedule this chunk
    const auto opID = currentOpID();
    ops_.emplace(opID, CommOp(chunk, id(), opID));

    // register this to the NPU
    auto* const depOp = &ops_.at(opID);
    npu_.registerRecvDep(chunk, depOp);
}

LinkResult::OpID LinkResult::currentOpID() noexcept {
    const auto currentID = nextOpID_;
    nextOpID_++;

    return currentID;
}
