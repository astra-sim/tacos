/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.

Copyright (c) 2022 Intel Corporation
Copyright (c) 2022 Georgia Institute of Technology
*******************************************************************************/

#include <cassert>
#include <tacos/writer/writer.h>

using namespace tacos;

Writer::Writer(const std::shared_ptr<Topology> topology,
               const std::shared_ptr<Collective> collective,
               const SynthesisResult synthesisResult) noexcept
    : topology(topology),
      collective(collective),
      synthesisResult(synthesisResult) {
    assert(topology != nullptr);
    assert(collective != nullptr);

    npusCount = topology->getNpusCount();
    chunksCount = collective->getChunksCount();
    chunkSize = collective->getChunkSize();
}

boost::filesystem::path Writer::prepareFile(
    const std::string& filename) const noexcept {
    assert(!filename.empty());

    // get the current process path
    auto currentPath = boost::filesystem::current_path();
    auto directoryPath = currentPath.append("results");

    // create directory, if not already exist
    if (!boost::filesystem::exists(directoryPath)) {
        boost::filesystem::create_directory(directoryPath);
    }

    // get filepath
    const auto filePath = directoryPath.append(filename);
    assert(filePath.has_extension());

    // if the file already exists, remove it
    if (boost::filesystem::exists(filePath)) {
        boost::filesystem::remove(filePath);
    }

    return filePath;
}
