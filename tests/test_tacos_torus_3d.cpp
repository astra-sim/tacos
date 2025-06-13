/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.

Copyright (c) 2022-2025 Intel Corporation
Copyright (c) 2022-2025 Georgia Institute of Technology
*******************************************************************************/

#include <cstdint>
#include <gtest/gtest.h>
#include <tacos/collective/all_gather.h>
#include <tacos/event_queue/event_queue.h>
#include <tacos/synthesizer/synthesizer.h>
#include <tacos/topology/torus_3d.h>
#include <test_config.h>

using namespace tacos;

TEST_F(TestConfig, Torus3x3x3) {
    const auto x = 3;
    const auto y = 3;
    const auto z = 3;
    const auto latency = 0.5;
    const auto bandwidth = 50.0;

    const auto topology = Torus3D(x, y, z, bandwidth, latency);
    const auto npusCount = topology.npusCount();
    ASSERT_EQ(npusCount, x * y * z);

    const auto collectivesCount = 1;
    const auto collective = AllGather(npusCount, collectivesCount);
    const auto chunksCount = collective.chunksCount();
    ASSERT_EQ(chunksCount, npusCount * collectivesCount);

    const auto chunkSize = int64_t(1024) * (1 << 20) / (npusCount * collectivesCount);

    auto solver = Synthesizer();

    auto minCollectiveTime = std::numeric_limits<EventQueue::Time>::max();
    for (int i = 0; i < repeat; ++i) {
        auto collectiveTime = solver.solve(topology, collective, chunkSize);
        minCollectiveTime = std::min(minCollectiveTime, collectiveTime);
    }

    const auto expected = 3706.20;
    const auto margin = expected * tolerance;
    ASSERT_NEAR(minCollectiveTime, expected, margin);
}

TEST_F(TestConfig, Torus3x4x5) {
    const auto x = 3;
    const auto y = 4;
    const auto z = 5;
    const auto latency = 0.5;
    const auto bandwidth = 50.0;

    const auto topology = Torus3D(x, y, z, bandwidth, latency);
    const auto npusCount = topology.npusCount();
    ASSERT_EQ(npusCount, x * y * z);

    const auto collectivesCount = 2;
    const auto collective = AllGather(npusCount, collectivesCount);
    const auto chunksCount = collective.chunksCount();
    ASSERT_EQ(chunksCount, npusCount * collectivesCount);

    const auto chunkSize = int64_t(2048) * (1 << 20) / (npusCount * collectivesCount);

    auto solver = Synthesizer();

    auto minCollectiveTime = std::numeric_limits<EventQueue::Time>::max();
    for (int i = 0; i < repeat; ++i) {
        auto collectiveTime = solver.solve(topology, collective, chunkSize);
        minCollectiveTime = std::min(minCollectiveTime, collectiveTime);
    }

    const auto expected = 6676.67;
    const auto margin = expected * tolerance;
    ASSERT_NEAR(minCollectiveTime, expected, margin);
}

TEST_F(TestConfig, Torus4x2x3) {
    const auto x = 4;
    const auto y = 2;
    const auto z = 3;
    const auto latency = 0.5;
    const auto bandwidth = 50.0;

    const auto topology = Torus3D(x, y, z, bandwidth, latency);
    const auto npusCount = topology.npusCount();
    ASSERT_EQ(npusCount, x * y * z);

    const auto collectivesCount = 3;
    const auto collective = AllGather(npusCount, collectivesCount);
    const auto chunksCount = collective.chunksCount();
    ASSERT_EQ(chunksCount, npusCount * collectivesCount);

    const auto chunkSize = int64_t(3072) * (1 << 20) / (npusCount * collectivesCount);

    auto solver = Synthesizer();

    auto minCollectiveTime = std::numeric_limits<EventQueue::Time>::max();
    for (int i = 0; i < repeat; ++i) {
        auto collectiveTime = solver.solve(topology, collective, chunkSize);
        minCollectiveTime = std::min(minCollectiveTime, collectiveTime);
    }

    const auto expected = 11673.67;
    const auto margin = expected * tolerance;
    ASSERT_NEAR(minCollectiveTime, expected, margin);
}
