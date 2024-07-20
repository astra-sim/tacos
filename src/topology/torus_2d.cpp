/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include <cassert>
#include <tacos/topology/torus_2d.h>

using namespace tacos;

Torus2D::Torus2D(const int width, const int height, const Bandwidth bandwidth, const Latency latency) noexcept
    : Topology() {
    assert(width > 0);
    assert(height > 0);
    assert(bandwidth > 0);
    assert(latency > 0);

    // compute NPUs count
    set_npus_count(width * height);

    // connect x-axis wise
    for (auto row = 0; row < height; row++) {
        for (auto col = 0; col < (width - 1); col++) {
            const auto src = (row * width) + col;
            const auto dest = src + 1;
            connect(src, dest, bandwidth, latency, true);
        }
        const auto src = (row * width) + (width - 1);
        const auto dest = (row * width);
        connect(src, dest, bandwidth, latency, true);
    }

    // connect y-axis wise
    for (auto col = 0; col < width; col++) {
        for (auto row = 0; row < (height - 1); row++) {
            const auto src = (row * width) + col;
            const auto dest = src + width;
            connect(src, dest, bandwidth, latency, true);
        }
        const auto src = ((height - 1) * width) + col;
        const auto dest = col;
        connect(src, dest, bandwidth, latency, true);
    }
}
