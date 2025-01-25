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

    [[nodiscard]] ChunkID chunkID() const noexcept;

    [[nodiscard]] bool hasDep() const noexcept;

    [[nodiscard]] LinkID linkID() const noexcept;

    [[nodiscard]] OpID opID() const noexcept;

    [[nodiscard]] const CommOp* depOp() const noexcept;

  private:
    ChunkID chunkID_;
    LinkID linkID_;
    OpID opID_;

    bool hasDep_ = false;
    CommOp* depOp_;
};

}  // namespace tacos
