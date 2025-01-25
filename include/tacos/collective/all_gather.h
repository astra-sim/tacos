/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.

Copyright (c) 2022 Intel Corporation
Copyright (c) 2022 Georgia Institute of Technology
*******************************************************************************/

#pragma once

#include <tacos/collective/collective.h>

namespace tacos {

class AllGather final : public Collective {
  public:
    explicit AllGather(int npusCount,
                       ChunkSize chunkSize = 1,
                       int initChunksPerNpu = 1) noexcept;
};

}  // namespace tacos
