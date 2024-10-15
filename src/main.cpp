/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include <iostream>
#include <tacos/collective/all_gather.h>
#include <tacos/event-queue/timer.h>
#include <tacos/synthesizer/TacosGreedy.h>
#include <tacos/topology/mesh_2d.h>

using namespace tacos;

int main() {
    std::cout << "Hello, world!" << std::endl;
    return 0;

    // // set print precision
    // fixed(std::cout);
    // std::cout.precision(2);

    // // construct a topology
    // const auto x = 5;
    // const auto y = 5;
    // const auto bw = 50;
    // const auto bw_beta = 1'000'000 / (bw * 1024.0);
    // const auto linkAlphaBeta = std::make_pair(0.5, bw_beta);

    // const auto topology = std::make_shared<mesh2d>(x, y, linkAlphaBeta);
    // const auto npusCount = topology->getNpusCount();
    // std::cout << "NPUs count: " << npusCount << std::endl;

    // // create collective
    // const auto collectivesCount = 1;
    // const auto chunkSize = 1024.0 / (npusCount * collectivesCount);
    // const auto collective = std::make_shared<AllGather>(npusCount, chunkSize, collectivesCount);
    // const auto chunksCount = collective->getChunksCount();
    // std::cout << "Chunks count: " << chunksCount << std::endl;

    // // create collective algorithm stat monitor
    // auto algorithmStatMonitor = std::make_shared<AlgorithmStatMonitor>(topology);
    // auto linkUsageTracker = std::make_shared<LinkUsageTracker>();

    // // create timer
    // auto solverTimer = Timer();

    // // create solver and solve
    // solverTimer.start();
    // auto solver = TacosGreedy(topology, collective, algorithmStatMonitor, linkUsageTracker);
    // auto collectiveTime = solver.solve();
    // solverTimer.stop();

    // // print result
    // auto time = solverTimer.elapsedTime();
    // std::cout << std::endl;
    // std::cout << "Time to solve: " << time << " us" << std::endl;
    // std::cout << "All-Gather Time: " << collectiveTime << std::endl;
    // std::cout << "All-Reduce Time: " << collectiveTime * 2 << std::endl;

    // terminate
    return 0;
}
