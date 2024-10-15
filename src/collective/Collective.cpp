/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include <cassert>
#include <tacos/Collective.h>

using namespace tacos;

Collective::Collective(const ChunkSize chunkSize) noexcept : chunkSize(chunkSize) {
    assert(chunkSize > 0);

    chunks = {};
    precondition = {};
    postcondition = {};
}

void Collective::add(const ChunkId chunkId, const NpuId src, const NpuId dest) noexcept {
    assert(chunkId >= 0);
    assert(src >= 0);
    assert(dest >= 0);

    chunks.insert(chunkId);
    precondition.emplace(chunkId, src);
    postcondition.emplace(chunkId, dest);
}

const Collective::CollectiveSet& Collective::getPrecondition() const noexcept {
    return precondition;
}

const Collective::CollectiveSet& Collective::getPostcondition() const noexcept {
    return postcondition;
}

ChunkSize Collective::getChunkSize() const noexcept {
    return chunkSize;
}

int Collective::getChunksCount() const noexcept {
    return chunksCount;
}

void Collective::updateChunksCount() noexcept {
    chunksCount = static_cast<int>(chunks.size());
}
