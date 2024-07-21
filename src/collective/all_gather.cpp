/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include <cassert>
#include <tacos/collective/all_gather.h>

using namespace tacos;

AllGather::AllGather(std::shared_ptr<Topology> topology,
                     const ChunkSize chunk_size,
                     const int chunks_per_collective) noexcept
    : Collective(std::move(topology), chunk_size) {
    assert(chunk_size > 0);
    assert(chunks_per_collective > 0);

    // set npus_count
    npus_count = _topology->npus_count();
    assert(npus_count > 0);

    // repeat generating all-gather
    auto current_chunk_id = 0;
    for (auto c = 0; c < chunks_per_collective; c++) {
        // generate one all-gather
        const auto next_chunk_id = generate_all_gather(current_chunk_id);

        // repeat the process
        current_chunk_id = next_chunk_id;
    }
}

int AllGather::generate_all_gather(const int start_chunk_id) noexcept {
    auto chunk_id = start_chunk_id;

    // for every src, make one chunk
    // distribute this chunk to all other NPUs
    for (auto src = 0; src < npus_count; src++) {
        for (auto dest = 0; dest < npus_count; dest++) {
            if (src == dest) {
                continue;
            }

            add_chunk(chunk_id, src, dest);
        }

        // src changes: chunk_id should be updated
        chunk_id++;
    }

    // chunk_id should be the next chunk id to be used
    return chunk_id;
}
