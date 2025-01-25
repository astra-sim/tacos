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
    hasDep_ = true;
    depOp_ = depOp;
}

CommOp::ChunkID CommOp::chunkID() const noexcept {
    return chunkID_;
}

bool CommOp::hasDep() const noexcept {
    return hasDep_;
}

const CommOp* CommOp::depOp() const noexcept {
    return depOp_;
}

CommOp::LinkID CommOp::linkID() const noexcept {
    return linkID_;
}

CommOp::OpID CommOp::opID() const noexcept {
    return opID_;
}
