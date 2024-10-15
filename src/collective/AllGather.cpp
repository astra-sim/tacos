/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include <cassert>
#include <tacos/AllGather.h>

using namespace Tacos;

AllGather::AllGather(const int npusCount, const ChunkSize chunkSize, const int collectivesCount) noexcept
    : Collective(chunkSize) {
    assert(npusCount > 0);
    assert(chunkSize > 0);
    assert(collectivesCount > 0);

    auto chunkId = 0;

    for (int i = 0; i < collectivesCount; i++) {
        for (int src = 0; src < npusCount; src++) {
            for (int dest = 0; dest < npusCount; dest++) {
                // for every src, make one chunk
                // and distribute this chunk to every dests
                add(chunkId, src, dest);
            }

            // chunkId should be updated here (i.e., when src changes)
            chunkId++;
        }
    }

    // reflect chunks count
    updateChunksCount();
}
