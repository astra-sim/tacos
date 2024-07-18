/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include "Mesh2D.h"
#include <cassert>

using namespace Tacos;

Mesh2D::Mesh2D(const int width, const int height, const LinkAlphaBeta linkAlphaBeta) noexcept : Topology() {
    assert(width > 0);
    assert(height > 0);
    assert(linkAlphaBeta.first >= 0);
    assert(linkAlphaBeta.second >= 0);

    // compute NPUs count
    setNpusCount(width * height);

    // connect x-axis wise
    for (auto row = 0; row < height; row++) {
        for (auto col = 0; col < (width - 1); col++) {
            const auto src = (row * width) + col;
            const auto dest = src + 1;
            connect(src, dest, linkAlphaBeta, true);
        }
    }

    // connect y-axis wise
    for (auto row = 0; row < (height - 1); row++) {
        for (auto col = 0; col < width; col++) {
            const auto src = (row * width) + col;
            const auto dest = src + width;
            connect(src, dest, linkAlphaBeta, true);
        }
    }
}
