/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#pragma once

#include "Topology.h"

namespace Tacos {
    class Hypercube3D final : public Topology {
    public:
        Hypercube3D(int size_x, int size_y, int size_z, LinkAlphaBeta linkAlphaBeta) noexcept;
    };
}  // namespace Tacos
