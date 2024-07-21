/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include <iostream>
#include <tacos/AlgorithmStatMonitor.h>
#include <tacos/LinkUsageTracker.h>
#include <tacos/TacosGreedy.h>
#include <tacos/collective/all_gather.h>
#include <tacos/event_queue/timer.h>
#include <tacos/topology/mesh_2d.h>

using namespace tacos;

int main() {
    // set print precision
    fixed(std::cout);
    std::cout.precision(2);

    // construct a topology
    const auto width = 5;
    const auto height = 5;
    const auto latency = 0.5;
    const auto bandwidth = 50;

    const auto topology = std::make_shared<Mesh2D>(width, height, bandwidth, latency);
    const auto npus_count = topology->npus_count();
    std::cout << "NPUs count: " << npus_count << std::endl;

    // create collective
    const auto collectivesCount = 1;
    const auto chunkSize = 1024.0 / (npus_count * collectivesCount);
    const auto collective = std::make_shared<AllGather>(topology, chunkSize, collectivesCount);
    const auto chunksCount = collective->chunks_count();
    std::cout << "Chunks count: " << chunksCount << std::endl;

    // create collective algorithm stat monitor
    auto algorithmStatMonitor = std::make_shared<AlgorithmStatMonitor>(topology);
    auto linkUsageTracker = std::make_shared<LinkUsageTracker>();

    // create timer
    auto solverTimer = Timer();

    // create solver and solve
    solverTimer.start();
    auto solver = TacosGreedy(topology, collective, algorithmStatMonitor, linkUsageTracker);
    auto collectiveTime = solver.solve();
    solverTimer.stop();

    // print result
    auto time = solverTimer.elapsed_time();
    std::cout << std::endl;
    std::cout << "Time to solve: " << time << " ms" << std::endl;
    std::cout << "All-Gather Time: " << collectiveTime << std::endl;
    std::cout << "All-Reduce Time: " << collectiveTime * 2 << std::endl;

    const auto links_count = topology->links_count();
    std::cout << "Links: " << links_count << std::endl;

    // terminate
    return 0;
}
