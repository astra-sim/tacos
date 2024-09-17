/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include "AllGather.h"
#include "ChakraWriter.h"
#include "Hypercube3D.h"
#include "Mesh2D.h"
#include "TacosGreedy.h"
#include "Timer.h"
#include "Torus2D.h"
#include "Torus3D.h"
#include <iostream>

using namespace Tacos;

int main() {
    // set print precision
    fixed(std::cout);
    std::cout.precision(2);

    // construct a topology
    const auto x = 3;
    const auto y = 3;

    const auto bw = 50;  // GB/s
    const auto bw_beta = 1'000'000 / (bw * 1024.0);
    const auto linkAlphaBeta = std::make_pair(0.5, bw_beta);

    const auto topology = std::make_shared<Mesh2D>(x, y, linkAlphaBeta);
    const auto npusCount = topology->getNpusCount();
    std::cout << "NPUs count: " << npusCount << std::endl;

    // create collective
    const auto collectivesCount = 1;
    const auto collectiveSize = 1024.0;  // MB
    const auto chunkSize = collectiveSize / (npusCount * collectivesCount);
    const auto collective = std::make_shared<AllGather>(npusCount, chunkSize, collectivesCount);
    const auto chunksCount = collective->getChunksCount();
    std::cout << "Chunks size: " << chunkSize << " MB" << std::endl;
    std::cout << "Chunks count: " << chunksCount << std::endl;

    // create Chakra Writer
    const auto chakraWriter = std::make_shared<ChakraWriter>(npusCount, collective);
    for (auto src = 0; src < npusCount; src++) {
        for (auto dest = 0; dest < npusCount; dest++) {
            if (src == dest) {
                continue;
            }

            if (topology->connected(src, dest)) {
                chakraWriter->addLink(src, dest);
            }
        }
    }

    // create timer
    auto solverTimer = Timer("PathSolver");

    // create solver and solve
    solverTimer.start();
    auto solver = TacosGreedy(topology, collective, chakraWriter);
    auto collectiveTime = solver.solve();
    solverTimer.stop();

    // print result
    auto time = solverTimer.getTime("ms");
    std::cout << std::endl;
    std::cout << "Time to solve: " << time << " ms" << std::endl;
    std::cout << "All-Gather Time: " << collectiveTime << std::endl;
    std::cout << "All-Reduce Time: " << collectiveTime * 2 << std::endl;

    // print Chakra Writer
    // chakraWriter->print();
    chakraWriter->writeToChakra("tacos");

    // terminate
    return 0;
}
