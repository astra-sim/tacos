/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include <cassert>
#include <tacos/topology/torus_3d.h>

using namespace tacos;

Torus3D::Torus3D(const int lenX,
                 const int lenY,
                 const int lenZ,
                 const Latency latency,
                 const Bandwidth bandwidth) noexcept
    : lenX(lenX),
      lenY(lenY),
      lenZ(lenZ),
      Topology() {

    // compute NPUs count
    setNpusCount(lenX * lenY * lenZ);

    // connect x_wise
    for (auto z = 0; z < lenZ; z++) {
        for (auto y = 0; y < lenY; y++) {
            for (auto x = 0; x < lenX - 1; x++) {
                const auto destX = x + 1;

                const auto src = (z * lenX * lenY) + (y * lenX) + x;
                const auto dest = (z * lenX * lenY) + (y * lenX) + destX;
                connect(src, dest, latency, bandwidth, true);
            }

            // wrap-around
            const auto src = (z * lenX * lenY) + (y * lenX) + (lenX - 1);
            const auto dest = (z * lenX * lenY) + (y * lenX);
            connect(src, dest, latency, bandwidth, true);
        }
    }

    // connect y_wise
    for (auto z = 0; z < lenZ; z++) {
        for (auto x = 0; x < lenX; x++) {
            for (auto y = 0; y < lenY - 1; y++) {
                const auto destY = y + 1;

                const auto src = (z * lenX * lenY) + (y * lenX) + x;
                const auto dest = (z * lenX * lenY) + (destY * lenX) + x;

                connect(src, dest, latency, bandwidth, true);
            }

            // wrap-around
            const auto src = (z * lenX * lenY) + ((lenY - 1) * lenX) + x;
            const auto dest = (z * lenX * lenY) + x;
            connect(src, dest, latency, bandwidth, true);
        }
    }

    // connect z_wise
    for (auto y = 0; y < lenY; y++) {
        for (auto x = 0; x < lenX; x++) {
            for (auto z = 0; z < lenZ - 1; z++) {
                const auto destZ = z + 1;

                const auto src = (z * lenX * lenY) + (y * lenX) + x;
                const auto dest = (destZ * lenX * lenY) + (y * lenX) + x;
                connect(src, dest, latency, bandwidth, true);
            }

            // wrap-around
            const auto src = ((lenZ - 1) * lenX * lenY) + (y * lenX) + x;
            const auto dest = (y * lenX) + x;
            connect(src, dest, latency, bandwidth, true);
        }
    }
}
