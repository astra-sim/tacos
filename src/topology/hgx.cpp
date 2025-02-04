/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.

Copyright (c) 2022 Intel Corporation
Copyright (c) 2022 Georgia Institute of Technology
*******************************************************************************/

#include <cassert>
#include <tacos/topology/hgx.h>

using namespace tacos;

HGX::HGX(const int width,
               const int height,
               const Latency latency_0,
               const Bandwidth bandwidth_0,
               const Latency latency_1,
               const Bandwidth bandwidth_1) noexcept
    : width(width),
      height(height) {
    assert(width > 0);
    assert(height > 0);
    assert(latency_0 >= 0);
    assert(bandwidth_0 > 0);
    assert(latency_1 >= 0);
    assert(bandwidth_1 > 0);

    // compute GPUs count
    setNpusCount(width * height);

    // connect width-wise links
    for (auto h = 0; h < height; h++) {
        for (auto w1 = 0; w1 < width - 1; w1++) {
            for (auto w2 = w1 + 1; w2 < width; w2++) {
            //auto w2 = w1 + 1;
                const auto src = (h * width) + w1;
                const auto dest = (h * width) + w2;
                connect(src, dest, latency_0, bandwidth_0, true);
            }
        }
    }

    // connect height-wise links
    for (auto h1 = 0; h1 < height - 1; h1++) {
        for (auto w1 = 0; w1 < width; w1++) {
            const auto src = (h1 * width) + w1;
            for (auto h2 = h1 + 1; h2 < height; h2++) {
                //for (auto w2 = 0; w2 < width; w2++) {
                auto w2 = w1;
                    const auto dest = (h2 * width) + w2;
                    connect(src, dest, latency_1, bandwidth_1, true);
                //}
            }
        }
    }
}
