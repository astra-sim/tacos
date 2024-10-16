/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#pragma once

#include <chakra/schema/protobuf/et_def.pb.h>
#include <memory>
#include <string>
#include <tacos/collective/collective.h>
#include <tacos/topology/topology.h>
#include <tacos/writer/synthesis_result.h>
#include <tacos/writer/writer.h>

namespace tacos {

class ChakraWriter final : public Writer {
  public:
    ChakraWriter(std::shared_ptr<Topology> topology,
                 std::shared_ptr<Collective> collective,
                 SynthesisResult synthesisResult) noexcept;

    void write(const std::string& filename) const noexcept override;

  private:
    using ChakraNodeID = int;
    using ChakraNode = ChakraProtoMsg::Node;
    using ChakraNodeType = ChakraProtoMsg::NodeType;

    enum class OpType {
        Send,
        Recv,
    };

    ChakraNodeID newChakraNodeID = 0;

    std::string getNodeName(ChakraNodeID chakraNodeID,
                            ChunkID chunk,
                            NpuID src,
                            NpuID dest,
                            OpType opType) const noexcept;

    std::unique_ptr<ChakraNode> generateChakraNode(ChunkID chunk,
                                                   NpuID src,
                                                   NpuID dest,
                                                   OpType opType) noexcept;
};

}  // namespace tacos
