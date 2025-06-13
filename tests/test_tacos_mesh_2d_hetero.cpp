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
#include <tacos/topology/mesh_2d_hetero.h>
#include <test_config.h>

using namespace tacos;

TEST_F(TestConfig, Mesh2DHetero5x5) {
    const auto width = 5;
    const auto height = 5;
    const auto latency = 0.5;
    const auto bandwidth1 = 50.0;
    const auto bandwidth2 = 75.0;

    const auto topology = Mesh2D_Hetero(width, height, bandwidth1, latency, bandwidth2, latency);
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

    const auto expected = 8005.0;
    const auto margin = expected * tolerance;
    ASSERT_NEAR(minCollectiveTime, expected, margin);
}

TEST_F(TestConfig, Mesh2DHetero8x5) {
    const auto width = 8;
    const auto height = 5;
    const auto latency = 1;
    const auto bandwidth1 = 80.0;
    const auto bandwidth2 = 35.0;

    const auto topology = Mesh2D_Hetero(width, height, bandwidth1, latency, bandwidth2, latency);
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

    const auto expected = 8595.43;
    const auto margin = expected * tolerance;
    ASSERT_NEAR(minCollectiveTime, expected, margin);
}

TEST_F(TestConfig, Mesh2DHetero7x10) {
    const auto width = 7;
    const auto height = 10;
    const auto latency = 1.5;
    const auto bandwidth1 = 16.0;
    const auto bandwidth2 = 73.0;

    const auto topology = Mesh2D_Hetero(width, height, bandwidth1, latency, bandwidth2, latency);
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

    const auto expected = 44612.47;
    const auto margin = expected * tolerance;
    ASSERT_NEAR(minCollectiveTime, expected, margin);
}
