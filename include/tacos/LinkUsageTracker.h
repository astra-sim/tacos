/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#pragma once

#include <map>
#include <string>
#include <vector>

namespace tacos {
class LinkUsageTracker {
  public:
    LinkUsageTracker() noexcept;

    void incrementLinkUsage(double time) noexcept;

    void incrementLinkUnused(double time) noexcept;

    void saveLinkUsage(const std::string& filename,
                       int linksCount,
                       double finishTime) const noexcept;

  private:
    std::map<double, int> linkUsage;
    std::map<double, int> linkUnused;
};
}  // namespace tacos
