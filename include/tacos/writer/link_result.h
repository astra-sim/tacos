/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.

Copyright (c) 2022 Intel Corporation
Copyright (c) 2022 Georgia Institute of Technology
*******************************************************************************/

#pragma once

#include <memory>
#include <tacos/collective/collective.h>
#include <tacos/writer/comm_op.h>
#include <vector>

namespace tacos {

class NpuResult;

class LinkResult {
  public:
    enum class LinkType { Ingress, Egress };

    using LinkID = CommOp::LinkID;
    using OpID = CommOp::OpID;
    using ChunkID = Collective::ChunkID;

    LinkResult(LinkID linkID, LinkType type, NpuResult* npu) noexcept;

    [[nodiscard]] LinkID id() const noexcept;
    void send(ChunkID chunk) noexcept;
    void recv(ChunkID chunk) noexcept;

  [[nodiscard]] const std::map<OpID, CommOp>& ops() const noexcept;

  private:
    LinkID id_;

    OpID nextOpID_ = 0;
    [[nodiscard]] OpID currentOpID() noexcept;

    NpuResult& npu_;
    LinkType type_;

    std::map<OpID, CommOp> ops_;
};

}  // namespace tacos
