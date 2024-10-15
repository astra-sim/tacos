/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include <cassert>
#include <tacos/topology/mesh_2d.h>

using namespace tacos;

Mesh2D::Mesh2D(const int width,
               const int height,
               const Latency latency,
               const Bandwidth bandwidth) noexcept
    : width(width),
      height(height) {
    assert(width > 0);
    assert(height > 0);
    assert(latency >= 0);
    assert(bandwidth > 0);

    // compute NPUs count
    setNpusCount(width * height);

    // connect width-wise links
    for (auto h = 0; h < height; h++) {
        for (auto w = 0; w < width - 1; w++) {
            const auto src = (h * width) + w;
            const auto dest = src + 1;
            connect(src, dest, latency, bandwidth, true);
        }
    }

    // connect height-wise links
    for (auto w = 0; w < width; w++) {
        for (auto h = 0; h < height - 1; h++) {
            const auto src = (h * width) + w;
            const auto dest = src + width;
            connect(src, dest, latency, bandwidth, true);
        }
    }
}
