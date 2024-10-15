/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include <cassert>
#include <fstream>
#include <tacos/AlgorithmStatMonitor.h>
#include <yaml-cpp/yaml.h>

using namespace Tacos;

AlgorithmStatMonitor::AlgorithmStatMonitor(const std::shared_ptr<Topology> topology) noexcept : topology(topology) {
    npusCount = topology->getNpusCount();

    // reset 2D vector
    chunkSizeProcessedPerLink = decltype(chunkSizeProcessedPerLink)(npusCount, std::vector<ChunkSize>(npusCount, -1));

    // for existing links, set the value to 0
    for (auto src = 0; src < npusCount; src++) {
        for (auto dest = 0; dest < npusCount; dest++) {
            if (topology->connected(src, dest)) {
                chunkSizeProcessedPerLink[src][dest] = 0;
            }
        }
    }
}

void AlgorithmStatMonitor::incrementProcessedChunkSize(const LinkId link, const ChunkSize chunkSize) noexcept {
    const auto [src, dest] = link;

    // assert validity
    assert(0 <= src && src < npusCount);
    assert(0 <= dest && dest < npusCount);

    // increment processed chunk size
    chunkSizeProcessedPerLink[src][dest] += chunkSize;
}

void AlgorithmStatMonitor::saveProcessedChunkSize(const std::string& filename,
                                                  double artime,
                                                  std::shared_ptr<Topology> topology) const noexcept {
    // YAML node to save link loads
    YAML::Node link_loads;

    // FIXME: assuming devicesCount = npus_count
    const auto devicesCount = npusCount;

    link_loads["runtime"] = artime;
    link_loads["devices-count"] = devicesCount;

    // iterate over all links
    for (auto src = 0; src < devicesCount; src++) {
        for (auto dest = 0; dest < devicesCount; dest++) {
            // skip if src == dest
            if (src == dest) {
                continue;
            }

            // skip if (src, dest) is not connected
            if (!topology->connected(src, dest)) {
                continue;
            }

            // get the link load
            const auto chunkSizeProcessed = chunkSizeProcessedPerLink[src][dest];
            const auto chunkSizeMB = (double)chunkSizeProcessed * 1024;  // FIXME: in KB

            // if there's load, create a new yaml node
            YAML::Node link_load;
            link_load["link"]["src"] = src;
            link_load["link"]["dest"] = dest;
            link_load["link"]["bw"] = topology->getBW(src, dest);
            link_load["load"] = -1;
            link_load["chunk_size"] = chunkSizeMB * 2;  // FIXME: assuming All-Reduce (synthesized from All-Gather)

            // push back to the link_loads
            link_loads["loads"].push_back(link_load);
        }
    }

    // save the link loads
    auto yaml_file = std::ofstream(filename);
    yaml_file << link_loads;
    yaml_file.close();
}
