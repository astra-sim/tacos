/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.

Copyright (c) 2022-2025 Intel Corporation
Copyright (c) 2022-2025 Georgia Institute of Technology
*******************************************************************************/

#include <cassert>
#include <tacos/collective/collective.h>

using namespace tacos;

Collective::Collective() noexcept = default;

int Collective::chunksCount() const noexcept {
    assert(chunksCount_ > 0);
    return chunksCount_;
}

void Collective::chunk_(const NpuID src, std::unordered_set<NpuID> dests) noexcept {
    assert(src >= 0);
    assert(!dests.empty());

    // assert this chunk is not already registered
    assert(precondition_.find(newChunkID_) == precondition_.end());
    assert(postcondition_.find(newChunkID_) == postcondition_.end());

    // insert to precondition and postcondition
    precondition_[newChunkID_] = src;
    postcondition_[newChunkID_] = std::move(dests);
    chunksCount_++;

    // increment nw chunkID for the next chunk
    newChunkID_++;
}

Collective::NpuID Collective::precondition(const ChunkID chunk) const noexcept {
    assert(0 <= chunk && chunk < chunksCount_);
    assert(precondition_.find(chunk) != precondition_.end());

    return precondition_.at(chunk);
}

const std::unordered_set<Collective::NpuID>& Collective::postcondition(
    ChunkID chunk) const noexcept {
    assert(0 <= chunk && chunk < chunksCount_);
    assert(postcondition_.find(chunk) != postcondition_.end());

    return postcondition_.at(chunk);
}
