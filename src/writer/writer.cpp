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
