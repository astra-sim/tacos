/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include <iostream>
#include <tacos/collective/all_gather.h>
#include <tacos/event-queue/timer.h>
#include <tacos/synthesizer/synthesizer.h>
#include <tacos/topology/mesh_2d.h>
#include <tacos/writer/synthesis_result.h>

using namespace tacos;

int main() {
    // set print precision
    fixed(std::cout);
    std::cout.precision(2);

    // print header
    std::cout << "[TACOS]" << std::endl;
    std::cout << std::endl;

    // construct a topology
    const auto width = 3;
    const auto height = 3;
    const auto bandwidth = 50.0;  // GB/s
    const auto latency = 500;     // ns

    const auto topology = std::make_shared<Mesh2D>(width, height, latency, bandwidth);
    const auto npusCount = topology->getNpusCount();

    std::cout << "[Topology Information]" << std::endl;
    std::cout << "\t- NPUs Count: " << npusCount << std::endl;
    std::cout << std::endl;

    // target collective
    const auto chunkSize = 1'048'576;  // B
    const auto initChunksPerNpu = 1;

    const auto collective = std::make_shared<AllGather>(npusCount, chunkSize, initChunksPerNpu);
    const auto chunksCount = collective->getChunksCount();

    std::cout << "[Collective Information]" << std::endl;
    const auto chunkSizeMB = chunkSize / (1 << 20);
    std::cout << "\t- Chunks Count: " << chunksCount << std::endl;
    std::cout << "\t- Chunk Size: " << chunkSize << " B";
    std::cout << " (" << chunkSizeMB << " MB)" << std::endl;
    std::cout << std::endl;

    // instantiate synthesizer
    auto synthesizer = Synthesizer(topology, collective);

    // create timer
    auto timer = Timer();

    // synthesize collective algorithm
    std::cout << "[Synthesis Process]" << std::endl;

    timer.start();
    const auto synthesisResult = synthesizer.synthesize();
    timer.stop();

    std::cout << std::endl;

    // print result
    std::cout << "[Synthesis Result]" << std::endl;

    const auto elapsedTimeUSec = timer.elapsedTime();
    const auto elapsedTimeSec = elapsedTimeUSec / 1e6;
    std::cout << "\t- Time to solve: " << elapsedTimeUSec << " us";
    std::cout << " (" << elapsedTimeSec << " s)" << std::endl;

    const auto collectiveTimePS = synthesisResult.getCollectiveTime();
    const auto collectiveTimeUSec = collectiveTimePS / 1.0e6;
    std::cout << "\t- Synthesized Collective Time: " << collectiveTimePS << " ps";
    std::cout << " (" << collectiveTimeUSec << " us)" << std::endl;

    // terminate
    return 0;
}
