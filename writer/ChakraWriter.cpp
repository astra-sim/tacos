/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include "ChakraWriter.h"
#include <iostream>

using namespace Tacos;

ChakraWriter::ChakraWriter(const int npusCount, const std::shared_ptr<Collective> collective) noexcept
    : npusCount(npusCount),
      chunkSize(collective->getChunkSize()),
      chunksCount(collective->getChunksCount()) {
    npus.reserve(npusCount);
    for (int id = 0; id < npusCount; id++) {
        npus.emplace_back(id, chunksCount, chunkSize);
    }

    // mark precondition
    for (const auto& [chunkId, npuId] : collective->getPrecondition()) {
        npus[npuId].markPrecond(chunkId);
    }
}

void ChakraWriter::addLink(const int src, const int dest) noexcept {
    npus[src].addOutlink(dest);
    npus[dest].addInlink(src);
}

void ChakraWriter::addCommunication(const ChunkId chunkId, const NpuId src, const NpuId dest) noexcept {
    npus[src].addSendOp(chunkId, dest);
    npus[dest].addRecvOp(chunkId, src);
}

void ChakraWriter::print() const noexcept {
    for (auto npu = 0; npu < npusCount; npu++) {
        npus[npu].print();
    }
}

void ChakraWriter::writeToChakra(const std::string& filename) noexcept {
    for (auto& npu : npus) {
        npu.writeToChakra(filename);
    }
}
