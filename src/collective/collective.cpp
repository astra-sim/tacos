/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include <cassert>
#include <tacos/collective/collective.h>

using namespace tacos;

Collective::Collective(std::shared_ptr<const Topology> topology, const ChunkSize chunk_size) noexcept
    : _topology(std::move(topology)),
      _chunk_size(chunk_size) {
    assert(chunk_size > 0);
    assert(_topology != nullptr);
}

void Collective::add_chunk(const ChunkId chunk_id, const NpuId src, const NpuId dest) noexcept {
    assert(chunk_id >= 0);
    assert(0 <= src && src < _topology->npus_count());
    assert(0 <= dest && dest < _topology->npus_count());

    // insert pre-/post-condition
    _pre_conditions.emplace(chunk_id, src);
    _post_conditions.emplace(chunk_id, dest);

    // increase the number of chunk by 1, if this chunk is newly added
    if (_chunks.find(chunk_id) == _chunks.end()) {
        _chunks_count++;
        _chunks.emplace(chunk_id);
    }
}

ChunkSize Collective::chunk_size() const noexcept {
    return _chunk_size;
}

int Collective::chunks_count() const noexcept {
    return _chunks_count;
}

const std::set<CollectiveCondition>& Collective::pre_conditions() const noexcept {
    return _pre_conditions;
}

const std::set<CollectiveCondition>& Collective::post_conditions() const noexcept {
    return _post_conditions;
}
