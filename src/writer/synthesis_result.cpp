/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.

Copyright (c) 2022 Intel Corporation
Copyright (c) 2022 Georgia Institute of Technology
*******************************************************************************/

#include <cassert>
#include <tacos/writer/synthesis_result.h>

using namespace tacos;

SynthesisResult::SynthesisResult(
    const std::shared_ptr<Topology> topology,
    const std::shared_ptr<Collective> collective) noexcept
    : npusCount_(topology->getNpusCount()) {
    // set the number of NPUs
    npus_.reserve(npusCount_);

    // initialize NpuResult for each NPU
    for (auto npu = 0; npu < npusCount_; npu++) {
        npus_.emplace_back(npu, topology);
    }

    // mark initial chunks
    const auto precondition = collective->getPrecondition();
    for (const auto& [src, chunks] : precondition) {
        for (const auto chunk : chunks) {
            npus_[src].registerRecvDep(chunk, nullptr);
        }
    }
}

NpuResult& SynthesisResult::npu(const NpuID id) noexcept {
    assert(0 <= id && id < npusCount_);
    return npus_[id];
}

void SynthesisResult::collectiveTime(Time time) noexcept {
    assert(time > 0);
    collectiveTime_ = time;
}

SynthesisResult::Time SynthesisResult::collectiveTime() const noexcept {
    assert(collectiveTime_ > 0);
    return collectiveTime_;
}
