/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#pragma once

#include <tacos/topology/topology.h>

namespace tacos {
class Mesh2D final : public Topology {
  public:
    Mesh2D(int width, int height, LinkAlphaBeta linkAlphaBeta) noexcept;
};
}  // namespace tacos
