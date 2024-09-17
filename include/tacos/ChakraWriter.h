/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#pragma once

#include "ChakraWriterNpu.h"
#include "Collective.h"
#include <memory>
#include <vector>

namespace Tacos {
class ChakraWriter {
  public:
    enum class OpType { Send, Recv };

    ChakraWriter(int npusCount, std::shared_ptr<Collective> collective) noexcept;

    void addLink(NpuId src, NpuId dest) noexcept;

    void addCommunication(ChunkId id, NpuId src, NpuId dest) noexcept;

    void print() const noexcept;

    void writeToChakra(const std::string& filename) noexcept;

  private:
    int npusCount;
    int chunksCount;
    ChunkSize chunkSize;

    std::vector<ChakraWriterNpu> npus;
};
}  // namespace Tacos
