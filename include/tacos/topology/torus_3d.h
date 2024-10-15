/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#pragma once

#include <tacos/topology/topology.h>

namespace tacos {

class Torus3D final : public Topology {
  public:
    Torus3D(int size_x, int size_y, int size_z, Latency latency, Bandwidth bandwidth) noexcept;

  private:
    int lenX;
    int lenY;
    int lenZ;
};

}  // namespace tacos
