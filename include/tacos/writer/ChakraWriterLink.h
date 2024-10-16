/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#pragma once

#include "ChakraNode.h"
#include "Typing.h"
#include <tuple>
#include <vector>
#include "et_def.pb.h"

namespace Tacos {
class ChakraWriterLink {
  public:
    ChakraWriterLink(NpuId src, NpuId dest) noexcept;

    ChakraWriterLink() noexcept;

    [[nodiscard]] std::pair<int, ChakraProtoMsg::Node*> addOp(NpuId id, OpType opType, int depNodeId, ChunkSize chunkSize) noexcept;

    void print() const noexcept;

  private:
    NpuId src;
    NpuId dest;

    std::vector<ChakraNode> synthesisResult;
};
}  // namespace Tacos
