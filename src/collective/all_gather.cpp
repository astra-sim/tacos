/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.

Copyright (c) 2022-2025 Intel Corporation
Copyright (c) 2022-2025 Georgia Institute of Technology
*******************************************************************************/

#include <cassert>
#include <tacos/collective/all_gather.h>

using namespace tacos;

AllGather::AllGather(const int npusCount, const int collectivesCount) noexcept : Collective() {
    assert(collectivesCount > 0);

    auto chunkId = 0;

    // destination for All-Gather: all NPUs in the topology
    auto dests = std::unordered_set<NpuID>();
    for (auto dest = 0; dest < npusCount; ++dest) {
        dests.insert(dest);
    }

    // register chunks for all source NPUs
    for (int c = 0; c < collectivesCount; ++c) {
        for (int src = 0; src < npusCount; ++src) {
            chunk_(src, dests);
        }
    }
}
