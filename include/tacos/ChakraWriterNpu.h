/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#pragma once

#include "ChakraWriterLink.h"
#include "Typing.h"
#include "et_def.pb.h"
#include "protoio.hh"
#include <map>
#include <string>

namespace Tacos {
class ChakraWriterNpu {
  public:
    ChakraWriterNpu(NpuId id, int chunksCount) noexcept;

    void markPrecond(ChunkId chunkId) noexcept;

    void addInlink(NpuId src) noexcept;

    void addOutlink(NpuId dest) noexcept;

    void addSendOp(ChunkId chunkId, NpuId dest) noexcept;

    void addRecvOp(ChunkId chunkId, NpuId src) noexcept;

    void print() const noexcept;

    void writeToChakra(const std::string& filename) noexcept;

    [[nodiscard]] int getDependentRecvNodeId(ChunkId chunkId) const noexcept;

  private:
    using Node = ChakraProtoMsg::Node;
    struct ChunkDepInfo {
      ChunkDepInfo() noexcept;

      bool isPrecond;
      int depNodeId;
    };

    NpuId id;
    std::map<int, ChakraWriterLink> inlinks;
    std::map<int, ChakraWriterLink> outlinks;

    std::map<ChunkId, ChunkDepInfo> dependencyTable;

    // ET Storage
    std::vector<Node*> et_nodes;
    // ProtoOutputStream* graph_stream;
    // bool first_win_;
    // uint32_t dep_window_size_;
    // uint64_t curr_id_;
};
}  // namespace Tacos
