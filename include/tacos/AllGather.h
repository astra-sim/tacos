/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#pragma once

#include "Collective.h"

namespace Tacos {
class AllGather final : public Collective {
  public:
    explicit AllGather(int npusCount, ChunkSize chunkSize = 1, int collectivesCount = 1) noexcept;
};
}  // namespace Tacos
