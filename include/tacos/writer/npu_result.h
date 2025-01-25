/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.

Copyright (c) 2022 Intel Corporation
Copyright (c) 2022 Georgia Institute of Technology
*******************************************************************************/

#pragma once

#include <map>
#include <tacos/collective/collective.h>
#include <tacos/topology/topology.h>
#include <tacos/writer/comm_op.h>
#include <tacos/writer/link_result.h>

namespace tacos {

class NpuResult {
  public:
    using NpuID = Topology::NpuID;
    using ChunkID = Collective::ChunkID;
    using LinkType = LinkResult::LinkType;
    using LinkID = LinkResult::LinkID;
    using OpID = LinkResult::OpID;

    NpuResult(NpuID id, std::shared_ptr<Topology> topology) noexcept;

    [[nodiscard]] LinkResult& linkFrom(NpuID id) noexcept;
    [[nodiscard]] LinkResult& linkTo(NpuID id) noexcept;

    void registerRecvDep(ChunkID chunk, CommOp* depOp) noexcept;

    CommOp* const getDep(ChunkID chunk) noexcept;

  private:
    int npusCount_;

    NpuID id_;

    std::map<ChunkID, CommOp*> depRecvOp_;

    std::map<NpuID, LinkResult> ingressLinks_;
    std::map<NpuID, LinkResult> egressLinks_;
};

}  // namespace tacos
