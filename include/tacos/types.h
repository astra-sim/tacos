/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#pragma once

#include <utility>

namespace tacos {

// Topology
using LinkId = std::pair<int, int>;
using LinkWeight = double;

// Collective
using ChunkId = int;
using ChunkSize = double;
}  // namespace tacos
