/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include <cassert>
#include <fstream>
#include <iostream>
#include <tacos/LinkUsageTracker.h>

using namespace tacos;

LinkUsageTracker::LinkUsageTracker() noexcept {
    linkUsage = {};
    linkUnused = {};
}

void LinkUsageTracker::incrementLinkUsage(const double time) noexcept {
    auto entry = linkUsage.find(time);

    if (entry != linkUsage.end()) {
        entry->second++;
        return;
    }

    linkUsage.emplace(time, 1);
}

void LinkUsageTracker::incrementLinkUnused(const double time) noexcept {
    auto entry = linkUnused.find(time);

    if (entry != linkUnused.end()) {
        entry->second++;
        return;
    }

    linkUnused.emplace(time, 1);
}

void LinkUsageTracker::saveLinkUsage(const std::string& filename,
                                     const int linksCount,
                                     const double finishTime) const noexcept {
    // open new csv file
    auto csv_file = std::ofstream(filename);
    csv_file << "Time,FinishTime,Used,Unused,Total" << std::endl;

    auto reverseDict = std::map<double, int>();

    for (auto it = linkUsage.begin(); it != linkUsage.end(); it++) {
        // retrieve usage
        const auto time = it->first;
        const auto usage = it->second;

        reverseDict.emplace(finishTime - time, usage);
    }

    for (auto it = reverseDict.begin(); it != reverseDict.end(); it++) {
        // retrieve usage
        const auto time = it->first;
        const auto usage = it->second;
        const auto unused_it = reverseDict.find(time);
        assert(unused_it != linkUnused.end());
        const auto unused = unused_it->second;

        // check validity
        const auto total = usage + unused;
        assert(total == linksCount);

        // save
        csv_file << time << "," << finishTime << "," << usage << "," << linksCount << std::endl;
    }

    for (auto it = std::next(linkUsage.begin()); it != linkUsage.end(); it++) {
        // retrieve usage
        const auto time = it->first;
        const auto usage = it->second;
        const auto unused_it = linkUnused.find(time);
        assert(unused_it != linkUnused.end());
        const auto unused = unused_it->second;

        // check validity
        const auto total = usage + unused;
        assert(total == linksCount);

        // save
        csv_file << time + finishTime << "," << finishTime << "," << usage << "," << linksCount << std::endl;
    }

    csv_file.close();
}
