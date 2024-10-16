/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#pragma once

#include <tacos/topology/topology.h>

namespace tacos {

class Hypercube3D final : public Topology {
  public:
    Hypercube3D(int lenX, int lenY, int lenZ, Latency latency, Bandwidth bandwidth) noexcept;

  private:
    int lenX;
    int lenY;
    int lenZ;
};

}  // namespace tacos
