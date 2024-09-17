/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#pragma once

#include "et_def.pb.h"
#include <Typing.h>
#include <set>

namespace Tacos {

class ChakraNode {
  public:
    ChakraNode(ChunkId chunkId, NpuId src, NpuId dest, OpType opType) noexcept;

    [[nodiscard]] int getNodeId() const noexcept;

    void print() const noexcept;

    void addDependency(int depNodeId) noexcept;

    ChakraProtoMsg::Node* generateChakraNode() const noexcept;

    [[nodiscard]] std::string getNodeName() const noexcept;

  private:
    using Node = ChakraProtoMsg::Node;
    using NodeType = ChakraProtoMsg::NodeType;

    static int nextNodeId;
    int nodeId;
    NpuId src;
    NpuId dest;
    NpuId chunkId;
    OpType opType;
    std::set<int> dependencies;
};

}  // namespace Tacos
