/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.

Copyright (c) 2022 Intel Corporation
Copyright (c) 2022 Georgia Institute of Technology
*******************************************************************************/

#include <cassert>
#include <tacos/writer/comm_op.h>

using namespace tacos;

CommOp::CommOp(const ChunkID chunkID,
               const LinkID linkID,
               const OpID opID) noexcept
    : chunkID_(chunkID),
      linkID_(linkID),
      opID_(opID) {}

void CommOp::setDepOp(CommOp* const depOp) {
    depOp_ = depOp;
}
