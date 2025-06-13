/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.

Copyright (c) 2022-2025 Intel Corporation
Copyright (c) 2022-2025 Georgia Institute of Technology
*******************************************************************************/

#include <climits>
#include <cstdint>
#include <gtest/gtest.h>
#include <tacos/collective/all_gather.h>
#include <tacos/event_queue/event_queue.h>
#include <tacos/synthesizer/synthesizer.h>
#include <tacos/topology/mesh_2d.h>
#include <test_config.h>

using namespace tacos;

TEST_F(TestConfig, Mesh5x5) {
    const auto width = 5;
    const auto height = 5;
    const auto latency = 0.5;
    const auto bandwidth = 50.0;

    const auto topology = Mesh2D(width, height, bandwidth, latency);
    const auto npusCount = topology.npusCount();
    ASSERT_EQ(npusCount, width * height);

    const auto collectivesCount = 1;
    const auto collective = AllGather(npusCount, collectivesCount);
    const auto chunksCount = collective.chunksCount();
    ASSERT_EQ(chunksCount, npusCount * collectivesCount);

    const auto chunkSize = int64_t(1024) * (1 << 20) / (npusCount * collectivesCount);

    auto synthesizer = Synthesizer();

    auto minCollectiveTime = std::numeric_limits<EventQueue::Time>::max();
    for (int i = 0; i < repeat; ++i) {
        auto collectiveTime = synthesizer.solve(topology, collective, chunkSize);
        minCollectiveTime = std::min(minCollectiveTime, collectiveTime);
    }

    const auto expected = 9606.0;
    const auto margin = expected * tolerance;
    ASSERT_NEAR(minCollectiveTime, expected, margin);
}

TEST_F(TestConfig, Mesh10x10) {
    const auto width = 10;
    const auto height = 10;
    const auto latency = 1;
    const auto bandwidth = 100.0;

    const auto topology = Mesh2D(width, height, bandwidth, latency);
    const auto npusCount = topology.npusCount();
    ASSERT_EQ(npusCount, width * height);

    const auto collectivesCount = 2;
    const auto collective = AllGather(npusCount, collectivesCount);
    const auto chunksCount = collective.chunksCount();
    ASSERT_EQ(chunksCount, npusCount * collectivesCount);

    const auto chunkSize = int64_t(1024) * (1 << 20) / (npusCount * collectivesCount);

    auto synthesizer = Synthesizer();

    auto minCollectiveTime = std::numeric_limits<EventQueue::Time>::max();
    for (int i = 0; i < repeat; ++i) {
        auto collectiveTime = synthesizer.solve(topology, collective, chunkSize);
        minCollectiveTime = std::min(minCollectiveTime, collectiveTime);
    }

    const auto expected = 5049.0;
    const auto margin = expected * tolerance;
    ASSERT_NEAR(minCollectiveTime, expected, margin);
}

TEST_F(TestConfig, Mesh7x12) {
    const auto width = 7;
    const auto height = 12;
    const auto latency = 1;
    const auto bandwidth = 100.0;

    const auto topology = Mesh2D(width, height, bandwidth, latency);
    const auto npusCount = topology.npusCount();
    ASSERT_EQ(npusCount, width * height);

    const auto collectivesCount = 3;
    const auto collective = AllGather(npusCount, collectivesCount);
    const auto chunksCount = collective.chunksCount();
    ASSERT_EQ(chunksCount, npusCount * collectivesCount);

    const auto chunkSize = int64_t(4096) * (1 << 20) / (npusCount * collectivesCount);

    auto synthesizer = Synthesizer();

    auto minCollectiveTime = std::numeric_limits<EventQueue::Time>::max();
    for (int i = 0; i < repeat; ++i) {
        auto collectiveTime = synthesizer.solve(topology, collective, chunkSize);
        minCollectiveTime = std::min(minCollectiveTime, collectiveTime);
    }

    const auto expected = 19966.27;
    const auto margin = expected * tolerance;
    ASSERT_NEAR(minCollectiveTime, expected, margin);
}
