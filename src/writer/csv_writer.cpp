/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.

Copyright (c) 2022 Intel Corporation
Copyright (c) 2022 Georgia Institute of Technology
*******************************************************************************/

#include <boost/algorithm/string/join.hpp>
#include <boost/filesystem.hpp>
#include <cassert>
#include <fstream>
#include <iostream>
#include <tacos/writer/csv_writer.h>

using namespace tacos;

CsvWriter::CsvWriter(const std::shared_ptr<Topology> topology,
                     const std::shared_ptr<Collective> collective,
                     const SynthesisResult synthesisResult) noexcept
    : Writer(topology, collective, synthesisResult) {
    assert(topology != nullptr);
    assert(collective != nullptr);

    npusCount = topology->getNpusCount();
}

void CsvWriter::write(const std::string& filename) const noexcept {
    assert(!filename.empty());

    // prepare the file
    const auto filePath = prepareFile(filename);
    assert(filePath.extension() == ".csv");

    // open the file
    auto csvFile = std::ofstream(filePath.string(), std::ios::out);

    // write data
    writeMetadata(csvFile);
    writeHeader(csvFile);
    writeSynthesisResult(csvFile);

    // close the csv file
    csvFile.close();

    // print statement
    std::cout << "\t- CSV: stored at " << filePath.string() << std::endl;
}

void CsvWriter::writeMetadata(std::ofstream& csvFile) const noexcept {
    csvFile << "NPUs Count," << npusCount << std::endl;
    csvFile << "Links Count," << topology->getLinksCount() << std::endl;
    csvFile << "Chunks Count," << collective->getChunksCount() << std::endl;
    csvFile << "Chunk Size," << collective->getChunkSize() << ",B" << std::endl;
}

void CsvWriter::writeHeader(std::ofstream& csvFile) const noexcept {
    csvFile << "SrcID,";
    csvFile << "DestID,";
    csvFile << "Latency (ns),";
    csvFile << "Bandwidth (GB/s),";
    csvFile << "Chunks" << std::endl;
}

void CsvWriter::writeSynthesisResult(std::ofstream& csvFile) const noexcept {
    for (auto src = 0; src < npusCount; src++) {
        for (auto dest = 0; dest < npusCount; dest++) {
            if (src == dest) {
                continue;
            }

            if (!topology->isConnected(src, dest)) {
                continue;
            }

            writeLinkInfo(src, dest, csvFile);
        }
    }
}

void CsvWriter::writeLinkInfo(const NpuID src,
                              const NpuID dest,
                              std::ofstream& csvFile) const noexcept {
    csvFile << src << ",";
    csvFile << dest << ",";
    csvFile << topology->getLatency(src, dest) << ",";
    csvFile << topology->getBandwidth(src, dest) << ",";

    const auto egressLinksInfo = synthesisResult.getEgressLinkInfo(src, dest);
    if (egressLinksInfo.empty()) {
        csvFile << "None" << std::endl;
        return;
    }

    if (egressLinksInfo.size() == 1) {
        csvFile << egressLinksInfo[0] << std::endl;
        return;
    }

    for (auto i = 0; i < egressLinksInfo.size() - 1; i++) {
        csvFile << egressLinksInfo[i] << ",";
    }
    csvFile << egressLinksInfo.back() << std::endl;
}
