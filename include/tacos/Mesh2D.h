/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#pragma once

#include "Topology.h"

namespace Tacos {
class Mesh2D final : public Topology {
  public:
    Mesh2D(int width, int height, LinkAlphaBeta linkAlphaBeta) noexcept;
};
}  // namespace Tacos
