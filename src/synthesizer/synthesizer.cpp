/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include <algorithm>
#include <cassert>
#include <iostream>
#include <tacos/synthesizer/synthesizer.h>

using namespace tacos;

Synthesizer::Synthesizer(const std::shared_ptr<Topology> topology,
                         const std::shared_ptr<Collective> collective) noexcept
    : topology(topology),
      collective(collective) {
    assert(topology != nullptr);
    assert(collective != nullptr);

    npusCount = topology->getNpusCount();
    chunksCount = collective->getChunksCount();

    // set topology chunk size
    const auto chunkSize = collective->getChunkSize();
    topology->setChunkSize(chunkSize);
    distinctLinkDelays = topology->getDistinctLinkDelays();

    // setup initial precondition and postcondition
    precondition = collective->getPrecondition();
    postcondition = collective->getPostcondition();

    // setup initial events
    currentTime = eventQueue.getCurrentTime();
    scheduleNextEvents();
}

void Synthesizer::synthesize() noexcept {
    while (!eventQueue.empty()) {
        // update current time
        currentTime = eventQueue.pop();

        // run link-chunk matching
        linkChunkMatching();

        // if synthesis is completed, break
        if (synthesisCompleted()) {
            break;
        }

        // if synthesis is not finished, schedule next events
        scheduleNextEvents();
    }

    assert(synthesisCompleted());
}

void Synthesizer::scheduleNextEvents() noexcept {
    assert(!distinctLinkDelays.empty());

    for (const auto linkDelay : distinctLinkDelays) {
        const auto nextEventTime = currentTime + linkDelay;
        eventQueue.schedule(nextEventTime);
    }
}

void Synthesizer::linkChunkMatching() noexcept {
    // get current precondition and postcondition
    const auto currentPrecondition = precondition;
    auto currentPostcondition = postcondition;

    // iterate over all unsatisfied postconditions
    while (!currentPostcondition.empty()) {
        // randomly select one postcondition
        const auto [dest, chunk] = selectPostcondition(&currentPostcondition);

        // backtrack the topology to find potential source NPUs
        const auto sourceNpus = topology->backtrackSourceNpus(dest, currentTime);

        // among the sourceNpus, find the candidate sources
        const auto candidateSourceNpus =
            checkCandidateSourceNpus(chunk, currentPrecondition, sourceNpus);

        // if there are no candidate source NPUs, skip
        if (candidateSourceNpus.empty()) {
            continue;
        }

        // randomly select one candidate source NPU
        auto src = selectSourceNpu(candidateSourceNpus);

        // link-chunk match made: mark this
        markLinkChunkMatch(src, dest, chunk);
    }
}

std::pair<Synthesizer::NpuID, Synthesizer::ChunkID> Synthesizer::selectPostcondition(
    CollectiveCondition* const currentPostcondition) noexcept {
    assert(currentPostcondition != nullptr);
    assert(!currentPostcondition->empty());

    // randomly pick an entry
    auto postconditionDist = std::uniform_int_distribution<>(0, currentPostcondition->size() - 1);
    int randomNpuIdx = postconditionDist(randomEngine);
    auto randomNpuIt = std::next(currentPostcondition->begin(), randomNpuIdx);
    auto dest = randomNpuIt->first;
    auto& chunks = randomNpuIt->second;

    // randomly pick a chunk
    auto chunkDist = std::uniform_int_distribution<>(0, chunks.size() - 1);
    int randomChunkIdx = chunkDist(randomEngine);
    auto randomChunkIt = std::next(chunks.begin(), randomChunkIdx);
    auto chunk = *randomChunkIt;

    // remove selected chunk from the postcondition
    chunks.erase(randomChunkIt);

    // remove the selected npu, if there's no remaining postcondition
    if (chunks.empty()) {
        currentPostcondition->erase(randomNpuIt);
    }

    // return the selected npu and chunk
    return {dest, chunk};
}

std::set<Synthesizer::NpuID> Synthesizer::checkCandidateSourceNpus(
    const ChunkID chunk,
    const CollectiveCondition& currentPrecondition,
    const std::set<NpuID>& sourceNpus) noexcept {
    assert(0 <= chunk && chunk < chunksCount);
    assert(!currentPrecondition.empty());
    assert(!sourceNpus.empty());

    auto candidateSourceNpus = std::set<NpuID>();

    // check which source NPUs hold the chunk
    for (const auto src : sourceNpus) {
        const auto chunksAtSrc = currentPrecondition.at(src);
        if (chunksAtSrc.find(chunk) != chunksAtSrc.end()) {
            candidateSourceNpus.insert(src);
        }
    }

    return candidateSourceNpus;
}

Synthesizer::NpuID Synthesizer::selectSourceNpu(
    const std::set<NpuID>& candidateSourceNpus) noexcept {
    assert(!candidateSourceNpus.empty());

    // if only one candidate source NPU, return it
    if (candidateSourceNpus.size() == 1) {
        const auto firstCandidate = candidateSourceNpus.begin();
        return *firstCandidate;
    }

    // randomly select one candidate source NPU
    auto candidateSourceNpusDist =
        std::uniform_int_distribution<>(0, candidateSourceNpus.size() - 1);
    int randomSrcIdx = candidateSourceNpusDist(randomEngine);
    auto randomSrcIt = std::next(candidateSourceNpus.begin(), randomSrcIdx);
    return *randomSrcIt;
}

void Synthesizer::markLinkChunkMatch(const NpuID src,
                                     const NpuID dest,
                                     const ChunkID chunk) noexcept {
    // mark the link-chunk match
    std::cout << "[Time " << currentTime << "] ";
    std::cout << src << " -> " << dest << ": " << chunk << std::endl;

    // insert the chunk to the precondition
    precondition[dest].insert(chunk);

    // remove the chunk from the postcondition
    postcondition[dest].erase(chunk);

    // if there's no remaining postcondition of the dest, remove it
    if (postcondition[dest].empty()) {
        postcondition.erase(dest);
    }
}

bool Synthesizer::synthesisCompleted() const noexcept {
    // synthesis is done when there's no remaining postcondition
    return postcondition.empty();
}
