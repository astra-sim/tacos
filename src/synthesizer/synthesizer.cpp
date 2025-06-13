/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.

Copyright (c) 2022-2025 Intel Corporation
Copyright (c) 2022-2025 Georgia Institute of Technology
*******************************************************************************/

#include <cassert>
#include <limits>
#include <tacos/synthesizer/synthesizer.h>

using namespace tacos;

Synthesizer::Synthesizer() noexcept = default;

Synthesizer::Time Synthesizer::solve(const Topology& topology,
                                     const Collective& collective,
                                     ChunkSize chunkSize) noexcept {
    assert(chunkSize > 0);

    // initialize the synthesizer
    initialize_(topology, collective, chunkSize);

    // mark trivial initial case
    // that is, chunks in preconditions are already at their sources
    markPrecondition_();

    // then, repeat the link-chunk matching process
    while (!eventQueue_.empty()) {
        // get current event time
        currentTime_ = eventQueue_.pop();

        // first, filter out unsatisfied postconditions
        // this is required when choosing the chunk replacement candidates
        // during the expansion of the TEN
        auto postconditionMap = filterPostcondition_();

        // then, expand the TEN
        // this method will also process and update the arrival of chunks
        // at the current timestep, and will change the unsatisfied postconditions
        expandTenTimestep_(&postconditionMap);

        // after the expansion of the TEN, check if there are any unsatisfied postconditions
        auto postcondition = shufflePostcondition_(postconditionMap);

        if (postcondition.empty()) {
            // no unsatisfied postcondition left to map
            // if so, just proceed to the next event
            // until all chunks arrive at their destinations
            continue;
        }

        // for all unsatisfied postconditions, run link-chunk matching
        for (const auto [chunk, dest] : postcondition) {
            linkChunkMatching_(chunk, dest);
        }
    }

    // all matching has been finished
    // return measured collective_ time
    assert(collectiveTime_ > 0);
    return collectiveTime_;
}

void Synthesizer::initialize_(const Topology& topology,
                              const Collective& collective,
                              const ChunkSize chunkSize) noexcept {
    // reset the event queue
    eventQueue_.reset();
    currentTime_ = 0;

    // set topology and collective
    topology_ = &topology;
    collective_ = &collective;

    // set variables
    npusCount = topology_->npusCount();
    chunksCount_ = collective_->chunksCount();

    // construct TEN from the topology
    ten_ = std::make_unique<TimeExpandedNetwork>(*topology_, chunkSize);

    // construct chunkMap_
    chunkMap_.assign(chunksCount_, std::vector<bool>(npusCount, false));
}

void Synthesizer::markPrecondition_() noexcept {
    // for every chunk, mark its source NPU as true in the chunkMap_
    for (auto chunk = 0; chunk < chunksCount_; ++chunk) {
        const auto src = collective_->precondition(chunk);
        chunkMap_[chunk][src] = true;
    }
}

Synthesizer::PostconditionMap Synthesizer::filterPostcondition_() const noexcept {
    auto postconditionMap = PostconditionMap();

    // iterate over all chunks
    for (auto chunk = 0; chunk < chunksCount_; ++chunk) {
        // check which destination NPUs have not yet received the chunk
        const auto dests = collective_->postcondition(chunk);
        for (const auto dest : dests) {
            if (!chunkMap_[chunk][dest]) {
                postconditionMap[dest].insert(chunk);
            }
        }
    }

    return postconditionMap;
}

std::vector<Synthesizer::Condition> Synthesizer::shufflePostcondition_(
    const PostconditionMap& postconditionMap) noexcept {
    auto postcondition = std::vector<Condition>();

    // flatten the postcondition map into a vector of conditions
    for (const auto& [dest, chunks] : postconditionMap) {
        for (const auto chunk : chunks) {
            postcondition.emplace_back(chunk, dest);
        }
    }

    // shuffle the postcondition vector
    std::shuffle(postcondition.begin(), postcondition.end(), randomEngine);
    return postcondition;
}

void Synthesizer::expandTenTimestep_(PostconditionMap* const postconditionMap) noexcept {
    // first, expand the TEN structure
    ten_->timestep(currentTime_);

    // this bool value is to track if any meaningful event happened during this timestep
    // e.g., chunk arrival or replacement
    // so that we can update the collective time
    auto eventHappened = false;

    // for every src-dest pairs
    for (auto src = 0; src < npusCount; src++) {
        for (auto dest = 0; dest < npusCount; dest++) {
            // if TEN is not available, skip
            // i.e., link doesn't exist or it is busy transferring a chunk
            if (!ten_->available(src, dest)) {
                continue;
            }

            // if a TEN link is available, there are two cases:
            // 1. the TEN link is indeed free, or
            // 2. it just become free by finishing a transfer
            // for case 2, we should mark this transfer as finished
            // and check for replacement possibilities

            // for case 1 (link is free), we can skip this
            auto chunk = ten_->chunk(src, dest);
            if (chunk < 0) {
                continue;
            }

            // for case 2, check if the chunk has already arrived at dest
            // by following other paths
            // and if so, check if we can replace this path with another chun
            if (chunkMap_[chunk][dest]) {
                // dest has already received this chunk
                // so check the replacement candidates
                const auto replacementChunk = findReplacementChunk_(src, dest, postconditionMap);

                if (!replacementChunk.has_value()) {
                    // no replacement candidate found
                    // just mark this TEN link as available and skip
                    ten_->transferFinished(src, dest);
                    continue;
                }

                // replacement candidate found
                chunk = replacementChunk.value();
            }

            // a meaningful chunk (regardless of replacement) has arrived at dest
            eventHappened = true;

            // mark the chunk arrived at dest, and mark this TEN link as available
            chunkMap_[chunk][dest] = true;
            ten_->transferFinished(src, dest);

            // mark this postcondition as satisfied
            // i.e., remove this chunk from the postcondition map
            auto it = postconditionMap->find(dest);
            if (it != postconditionMap->end()) {
                it->second.erase(chunk);
                if (it->second.empty()) {
                    postconditionMap->erase(it);
                }
            }
        }
    }

    // at the end of the TEN expansion
    // if a meaningful event happened, we should update the collective time
    if (eventHappened) {
        // update collective time to current time
        collectiveTime_ = currentTime_;
    }
}

std::optional<Synthesizer::ChunkID> Synthesizer::findReplacementChunk_(
    const NpuID src, const NpuID dest, const PostconditionMap* const postconditionMap) noexcept {
    // trivial scenario: if dest has all postconditions satisfied,
    // there's no need for replacement
    if (postconditionMap->find(dest) == postconditionMap->end()) {
        return std::nullopt;
    }

    // check if any replacement candidate exists
    auto candidates = std::vector<ChunkID>();

    // iterate over all unsatisfied postcondition of this dest NPU
    for (const auto chunk : postconditionMap->at(dest)) {
        // if this chunk is available at src NPU
        // but has not yet arrived at dest NPU,
        // this chunk can be a replacement candidate
        if (chunkMap_[chunk][src] && !chunkMap_[chunk][dest]) {
            candidates.push_back(chunk);
        }
    }

    // if there's no candidate, return nullopt
    if (candidates.empty()) {
        return std::nullopt;
    }

    // if there's only one candidate, return it
    if (candidates.size() == 1) {
        return candidates[0];
    }

    // if there are multiple candidates, randomly select one and return it
    auto dist = std::uniform_int_distribution<>(0, candidates.size() - 1);
    const auto idx = dist(randomEngine);
    return candidates[idx];
}

void Synthesizer::linkChunkMatching_(const ChunkID chunk, const NpuID dest) noexcept {
    // backtrack source NPUs
    auto sources = ten_->backtrack(dest);

    // filter candidate link-chunk matching
    // that has the earliest estimated chunk arrival time
    auto arrivalTime = std::numeric_limits<Time>::max();
    auto candidates = std::vector<NpuID>();

    // iterate over all source NPUs
    for (const auto src : sources) {
        // if src does not have the chunk, skip
        if (!chunkMap_[chunk][src]) {
            continue;
        }

        // if source has the chunk, check the link transfer time
        const auto linkWeight = ten_->linkTransferTime(src, dest);
        const auto linkTime = currentTime_ + linkWeight;

        // if this link time is equal to the minimum time
        // add to the candidate links
        if (isEqual(linkTime, arrivalTime)) {
            candidates.emplace_back(src);
        }

        // if this link time is less than the minimum time,
        // update the minimum time and reset candidates
        if (linkTime < arrivalTime) {
            arrivalTime = linkTime;
            candidates.clear();
            candidates.emplace_back(src);
        }
    }

    // if candidates are empty, no match can be made
    if (candidates.empty()) {
        return;
    }

    // randomly shuffle and select one source NPU to make link-chunk match
    std::shuffle(candidates.begin(), candidates.end(), randomEngine);
    const auto selectedSrc = candidates.front();

    // mark the TEN as occupied
    ten_->transferChunk(selectedSrc, dest, chunk, arrivalTime);

    // schedule an event when the matched chunk arrives
    eventQueue_.schedule(arrivalTime);
}

bool Synthesizer::isEqual(const Time lhs, const Time rhs) noexcept {
    constexpr Time epsilon = 1e-9;
    return std::abs(lhs - rhs) < epsilon;
}
