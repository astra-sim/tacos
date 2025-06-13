/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.

Copyright (c) 2022-2025 Intel Corporation
Copyright (c) 2022-2025 Georgia Institute of Technology
*******************************************************************************/

#include <iostream>
#include <tacos/collective/all_gather.h>
#include <tacos/event_queue/timer.h>
#include <tacos/synthesizer/synthesizer.h>
#include <tacos/topology/hypercube_3d.h>
#include <tacos/topology/mesh_2d.h>
#include <tacos/topology/torus_2d.h>
#include <tacos/topology/torus_3d.h>

using namespace tacos;

int main() {
    // set print precision
    fixed(std::cout);
    std::cout.precision(2);

    // construct a topology
    const auto x = 5;
    const auto y = x;
    const auto z = y;

    const auto latency = 0.5;   // microseconds (us)
    const auto bandwidth = 50;  // GiB/sec

    const auto topology = Hypercube3D(x, y, z, bandwidth, latency);
    const auto npusCount = topology.npusCount();
    std::cout << "NPUs count: " << npusCount << std::endl;

    // create collective
    const auto collectivesCount = 1;
    const auto collective = AllGather(npusCount, collectivesCount);
    const auto chunksCount = collective.chunksCount();
    std::cout << "Chunks count: " << chunksCount << std::endl;

    // set chunk size
    const auto chunkSize = 1024 * (1 << 20) / (npusCount * collectivesCount);

    // create timer
    auto solverTimer = Timer();

    // create solver and solve
    solverTimer.start();
    auto solver = Synthesizer();
    auto collectiveTime = solver.solve(topology, collective, chunkSize);
    solverTimer.stop();

    // print result
    auto time = solverTimer.time();
    std::cout << std::endl;
    std::cout << "Time to solve: " << time / 1000 << " ms" << std::endl;
    std::cout << "All-Gather Time: " << collectiveTime << std::endl;
    std::cout << "All-Reduce Time: " << collectiveTime * 2 << std::endl;

    // terminate
    return 0;
}
