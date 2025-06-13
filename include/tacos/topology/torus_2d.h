/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.

Copyright (c) 2022-2025 Intel Corporation
Copyright (c) 2022-2025 Georgia Institute of Technology
*******************************************************************************/

#pragma once

#include <tacos/topology/topology.h>

namespace tacos {

/// @brief 2D torus topology
class Torus2D final : public Topology {
  public:
    /// @brief Constructor for 2D torus topology
    /// @param width width of the torus (number of NPUs in the x-axis)
    /// @param height height of the torus (number of NPUs in the y-axis)
    /// @param bandwidth bandwidth of the links (in GiB/sec)
    /// @param latency latency of the links (in microseconds)
    Torus2D(int width, int height, Bandwidth bandwidth, Latency latency) noexcept;
};
}  // namespace tacos
