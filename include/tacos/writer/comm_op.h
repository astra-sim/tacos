/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.

Copyright (c) 2022 Intel Corporation
Copyright (c) 2022 Georgia Institute of Technology
*******************************************************************************/

#pragma once

#include <tacos/collective/collective.h>

namespace tacos {

class CommOp {
  public:
    using ChunkID = Collective::ChunkID;
    using LinkID = int;
    using OpID = int;

    CommOp(ChunkID chunkID, LinkID linkID, OpID opID) noexcept;

    void setDepOp(CommOp* depOp);

  private:
    ChunkID chunkID_;
    LinkID linkID_;
    OpID opID_;

    CommOp* depOp_;
};

}  // namespace tacos
