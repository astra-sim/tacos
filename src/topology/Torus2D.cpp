/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include <tacos/Torus2D.h>
#include <cassert>

using namespace Tacos;

Torus2D::Torus2D(const int width, const int height, const LinkAlphaBeta linkAlphaBeta) noexcept : Topology() {
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
        const auto src = (row * width) + (width - 1);
        const auto dest = (row * width);
        connect(src, dest, linkAlphaBeta, true);
    }

    // connect y-axis wise
    for (auto col = 0; col < width; col++) {
        for (auto row = 0; row < (height - 1); row++) {
            const auto src = (row * width) + col;
            const auto dest = src + width;
            connect(src, dest, linkAlphaBeta, true);
        }
        const auto src = ((height - 1) * width) + col;
        const auto dest = col;
        connect(src, dest, linkAlphaBeta, true);
    }
}
