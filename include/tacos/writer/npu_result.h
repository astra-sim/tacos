/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#pragma once

#include <map>
#include <memory>
#include <tacos/collective/collective.h>
#include <tacos/topology/topology.h>
#include <vector>

namespace tacos {

class NpuResult {
  public:
    using NpuID = Topology::NpuID;
    using ChunkID = Collective::ChunkID;

    NpuResult(int npu,
              std::shared_ptr<Topology> topology,
              std::shared_ptr<Collective> collective) noexcept;

    void addIngressLinkInfo(ChunkID chunk, NpuID src) noexcept;

    void addEgressLinkInfo(ChunkID chunk, NpuID dest) noexcept;

    std::vector<ChunkID> getIngressLinkInfo(NpuID src) const noexcept;

    std::vector<ChunkID> getEgressLinkInfo(NpuID dest) const noexcept;

  private:
    int npu;
    int npusCount;
    int chunksCount;
    std::map<NpuID, std::vector<ChunkID>> ingressLinksInfo;
    std::map<NpuID, std::vector<ChunkID>> egressLinksInfo;
};

}  // namespace tacos
