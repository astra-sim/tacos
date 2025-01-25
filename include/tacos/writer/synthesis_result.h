/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.

Copyright (c) 2022 Intel Corporation
Copyright (c) 2022 Georgia Institute of Technology
*******************************************************************************/

#pragma once

#include <memory>
#include <tacos/collective/collective.h>
#include <tacos/event-queue/event_queue.h>
#include <tacos/topology/topology.h>
#include <tacos/writer/npu_result.h>
#include <vector>

namespace tacos {

class SynthesisResult {
  public:
    using Time = EventQueue::Time;
    using NpuID = Topology::NpuID;

    SynthesisResult(std::shared_ptr<Topology> topology,
                    std::shared_ptr<Collective> collective) noexcept;

    [[nodiscard]] NpuResult& npu(NpuID id) noexcept;

    void collectiveTime(Time time) noexcept;
    [[nodiscard]] Time collectiveTime() const noexcept;

  private:
    int npusCount;
    std::vector<NpuResult> npus_;

    Time collectiveTime_;
};

}  // namespace tacos
