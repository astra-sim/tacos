/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#pragma once

#include <boost/filesystem.hpp>
#include <memory>
#include <string>
#include <tacos/collective/collective.h>
#include <tacos/topology/topology.h>
#include <tacos/writer/synthesis_result.h>

namespace tacos {

class Writer {
  public:
    Writer(std::shared_ptr<Topology> topology,
           std::shared_ptr<Collective> collective,
           SynthesisResult synthesisResult) noexcept;

    virtual void write(const std::string& filename) const noexcept = 0;

  protected:
    using NpuID = Topology::NpuID;
    using ChunkID = Collective::ChunkID;
    using ChunkSize = Collective::ChunkSize;

    int npusCount;
    int chunksCount;
    ChunkSize chunkSize;

    std::shared_ptr<Topology> topology;
    std::shared_ptr<Collective> collective;
    SynthesisResult synthesisResult;

    [[nodiscard]] boost::filesystem::path prepareFile(const std::string& filename) const noexcept;
};

}  // namespace tacos
