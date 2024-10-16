/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#pragma once

#include <memory>
#include <string>
#include <tacos/collective/collective.h>
#include <tacos/topology/topology.h>
#include <tacos/writer/synthesis_result.h>

namespace tacos {

class CsvWriter {
  public:
    CsvWriter(std::shared_ptr<Topology> topology,
              std::shared_ptr<Collective> collective,
              SynthesisResult synthesisResult) noexcept;

    void write(const std::string& filename) const noexcept;

  private:
    using NpuID = Topology::NpuID;

    int npusCount;
    std::shared_ptr<Topology> topology;
    std::shared_ptr<Collective> collective;
    SynthesisResult synthesisResult;

    void writeMetadata(std::ofstream& csvFile) const noexcept;
    void writeHeader(std::ofstream& csvFile) const noexcept;
    void writeSynthesisResult(std::ofstream& csvFile) const noexcept;
    void writeLinkInfo(NpuID src, NpuID dest, std::ofstream& csvFile) const noexcept;
};

}  // namespace tacos
