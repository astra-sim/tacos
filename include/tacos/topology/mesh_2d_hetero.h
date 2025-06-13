/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.

Copyright (c) 2022-2025 Intel Corporation
Copyright (c) 2022-2025 Georgia Institute of Technology
*******************************************************************************/

#pragma once

#include <tacos/topology/topology.h>

namespace tacos {

/// @brief 2D mesh topology with heterogeneous links
/// i.e., links in the x-axis and y-axis can have different latencies and bandwidths
class Mesh2D_Hetero final : public Topology {
  public:
    /// @brief Constructor for heterogeneous 2D mesh topology
    /// @param width width of the mesh (number of NPUs in the x-axis)
    /// @param height size of the mesh (number of NPUs in the y-axis)
    /// @param bandwidthWidth bandwidth of the links in the x-axis (in GiB/sec)
    /// @param latencyWidth latency of the links in the x-axis (in microseconds)
    /// @param bandwidthHeight bandwidth of the links in the y-axis (in GiB/sec)
    /// @param latencyHeight latency of the links in the y-axis (in microseconds)
    Mesh2D_Hetero(int width,
                  int height,
                  Bandwidth bandwidthWidth,
                  Latency latencyWidth,
                  Bandwidth bandwidthHeight,
                  Latency latencyHeight) noexcept;
};
}  // namespace tacos
