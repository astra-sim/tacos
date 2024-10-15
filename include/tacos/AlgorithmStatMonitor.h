/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#pragma once

#include <memory>
#include <string>
#include <tacos/Typing.h>
#include <tacos/topology/topology.h>
#include <vector>

namespace tacos {

class AlgorithmStatMonitor {
  public:
    explicit AlgorithmStatMonitor(std::shared_ptr<Topology> topology) noexcept;

    void incrementProcessedChunkSize(LinkId link, ChunkSize chunkSize) noexcept;

    void saveProcessedChunkSize(const std::string& filename,
                                double artime,
                                std::shared_ptr<Topology> topology) const noexcept;

  private:
    int npusCount;

    std::vector<std::vector<ChunkSize>> chunkSizeProcessedPerLink;

    std::shared_ptr<Topology> topology;
};

}  // namespace tacos
