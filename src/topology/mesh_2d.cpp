/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.

Copyright (c) 2022-2025 Intel Corporation
Copyright (c) 2022-2025 Georgia Institute of Technology
*******************************************************************************/

#include <cassert>
#include <tacos/topology/mesh_2d.h>

using namespace tacos;

Mesh2D::Mesh2D(const int width,
               const int height,
               const Bandwidth bandwidth,
               const Latency latency) noexcept
    : Topology() {
    assert(width > 0);
    assert(height > 0);
    assert(bandwidth > 0);
    assert(latency >= 0);

    // compute NPUs count
    setNpusCount_(width * height);

    // connect x-axis wise
    for (auto row = 0; row < height; ++row) {
        for (auto col = 0; col < (width - 1); ++col) {
            const auto src = (row * width) + col;
            const auto dest = src + 1;
            connect_(src, dest, bandwidth, latency, true);
        }
    }

    // connect y-axis wise
    for (auto row = 0; row < (height - 1); ++row) {
        for (auto col = 0; col < width; ++col) {
            const auto src = (row * width) + col;
            const auto dest = src + width;
            connect_(src, dest, bandwidth, latency, true);
        }
    }
}
