/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include <cassert>
#include <tacos/collective/all_gather.h>

using namespace tacos;

AllGather::AllGather(const int npusCount,
                     const ChunkSize chunkSize,
                     const int initChunksPerNpu) noexcept
    : Collective(npusCount, chunkSize) {
    assert(npusCount > 0);
    assert(chunkSize > 0);
    assert(initChunksPerNpu > 0);

    auto chunkID = 0;

    for (int i = 0; i < initChunksPerNpu; i++) {
        for (int src = 0; src < npusCount; src++) {
            for (int dest = 0; dest < npusCount; dest++) {
                // for every src, make one chunk
                // and distribute this chunk to every dests
                add(chunkID, src, dest);
            }

            // chunkID should be updated here (i.e., when src changes)
            chunkID++;
        }
    }

    // reflect chunks count
    updateChunksCount();
}
