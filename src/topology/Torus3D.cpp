/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include <cassert>
#include <tacos/Torus3D.h>

using namespace Tacos;

Torus3D::Torus3D(int size_x, int size_y, int size_z, LinkAlphaBeta linkAlphaBeta) noexcept : Topology() {
    assert(size_x > 0);
    assert(size_y > 0);
    assert(size_z > 0);
    assert(linkAlphaBeta.first >= 0);
    assert(linkAlphaBeta.second >= 0);

    // compute NPUs count
    setNpusCount(size_x * size_y * size_z);

    // connect x_wise
    for (auto z = 0; z < size_z; z++) {
        for (auto y = 0; y < size_y; y++) {
            for (auto x = 0; x < size_x - 1; x++) {
                const auto dest_x = x + 1;
                const auto src = (z * size_x * size_y) + (y * size_x) + x;
                const auto dest = (z * size_x * size_y) + (y * size_x) + dest_x;

                connect(src, dest, linkAlphaBeta, true);
            }

            const auto src = (z * size_x * size_y) + (y * size_x) + (size_x - 1);
            const auto dest = (z * size_x * size_y) + (y * size_x);

            connect(src, dest, linkAlphaBeta, true);
        }
    }

    // connect y_wise
    for (auto z = 0; z < size_z; z++) {
        for (auto x = 0; x < size_x; x++) {
            for (auto y = 0; y < size_y - 1; y++) {
                const auto dest_y = y + 1;
                const auto src = (z * size_x * size_y) + (y * size_x) + x;
                const auto dest = (z * size_x * size_y) + (dest_y * size_x) + x;

                connect(src, dest, linkAlphaBeta, true);
            }
            const auto src = (z * size_x * size_y) + ((size_y - 1) * size_x) + x;
            const auto dest = (z * size_x * size_y) + +x;
            connect(src, dest, linkAlphaBeta, true);
        }
    }

    // connect z_wise

    for (auto y = 0; y < size_y; y++) {
        for (auto x = 0; x < size_x; x++) {
            for (auto z = 0; z < size_z - 1; z++) {
                const auto dest_z = z + 1;
                const auto src = (z * size_x * size_y) + (y * size_x) + x;
                const auto dest = (dest_z * size_x * size_y) + (y * size_x) + x;

                connect(src, dest, linkAlphaBeta, true);
            }
            const auto src = ((size_z - 1) * size_x * size_y) + (y * size_x) + x;
            const auto dest = (y * size_x) + x;

            connect(src, dest, linkAlphaBeta, true);
        }
    }
}
