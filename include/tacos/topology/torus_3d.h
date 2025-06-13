/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.

Copyright (c) 2022-2025 Intel Corporation
Copyright (c) 2022-2025 Georgia Institute of Technology
*******************************************************************************/

#pragma once

#include <tacos/topology/topology.h>

namespace tacos {

/// @brief 3D torus topology
class Torus3D final : public Topology {
  public:
    /// @brief Constructor for a 3D torus topology
    /// @param size_x size of the x-axis (number of NPUs in the x-axis)
    /// @param size_y size of the y-axis (number of NPUs in the y-axis)
    /// @param size_z size of the z-axis (number of NPUs in the z-axis)
    /// @param bandwidth bandwidth of the links (in GiB/sec)
    /// @param latency latency of the links (in microseconds)
    Torus3D(int size_x, int size_y, int size_z, Bandwidth bandwidth, Latency latency) noexcept;
};
}  // namespace tacos
