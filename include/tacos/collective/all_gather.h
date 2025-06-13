/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.

Copyright (c) 2022-2025 Intel Corporation
Copyright (c) 2022-2025 Georgia Institute of Technology
*******************************************************************************/

#pragma once

#include <memory>
#include <tacos/collective/collective.h>
#include <tacos/topology/topology.h>

namespace tacos {

/// @brief All-Gather collective communication pattern
class AllGather final : public Collective {
  public:
    /// @brief Constructor for AllGather collective
    /// @param npusCount number of NPUs in the topology
    /// @param collectivesCount number of initial chunks per each NPU
    explicit AllGather(int npusCount, int collectivesCount = 1) noexcept;
};
}  // namespace tacos
