/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.

Copyright (c) 2022-2025 Intel Corporation
Copyright (c) 2022-2025 Georgia Institute of Technology
*******************************************************************************/

#include <cassert>
#include <tacos/topology/hypercube_3d.h>

using namespace tacos;

Hypercube3D::Hypercube3D(
    int size_x, int size_y, int size_z, const Bandwidth bandwidth, const Latency latency) noexcept
    : Topology() {
    assert(size_x > 0);
    assert(size_y > 0);
    assert(size_z > 0);
    assert(bandwidth > 0);
    assert(latency >= 0);

    // compute NPUs count
    setNpusCount_(size_x * size_y * size_z);

    // connect x_wise
    for (auto z = 0; z < size_z; ++z) {
        for (auto y = 0; y < size_y; ++y) {
            for (auto x = 0; x < (size_x - 1); ++x) {
                const auto dest_x = x + 1;
                const auto src = (z * size_x * size_y) + (y * size_x) + x;
                const auto dest = (z * size_x * size_y) + (y * size_x) + dest_x;

                connect_(src, dest, bandwidth, latency, true);
            }
        }
    }

    // connect y_wise
    for (auto z = 0; z < size_z; ++z) {
        for (auto y = 0; y < (size_y - 1); ++y) {
            for (auto x = 0; x < size_x; ++x) {
                const auto dest_y = y + 1;
                const auto src = (z * size_x * size_y) + (y * size_x) + x;
                const auto dest = (z * size_x * size_y) + (dest_y * size_x) + x;

                connect_(src, dest, bandwidth, latency, true);
            }
        }
    }

    // connect z_wise
    for (auto z = 0; z < (size_z - 1); ++z) {
        for (auto y = 0; y < size_y; ++y) {
            for (auto x = 0; x < size_x; ++x) {
                const auto dest_z = z + 1;
                const auto src = (z * size_x * size_y) + (y * size_x) + x;
                const auto dest = (dest_z * size_x * size_y) + (y * size_x) + x;

                connect_(src, dest, bandwidth, latency, true);
            }
        }
    }
}
