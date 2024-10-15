/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#pragma once

#include <utility>

namespace tacos {
// Time
using Time = double;

// Topology
using NpuId = int;
using LinkId = std::pair<NpuId, NpuId>;
using LinkWeight = Time;
using LinkAlphaBeta = std::pair<LinkWeight, LinkWeight>;

// Collective
using ChunkId = int;
using ChunkSize = double;
}  // namespace tacos
