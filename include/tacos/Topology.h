/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#pragma once

#include "Typing.h"
#include <vector>

namespace Tacos {
    class Topology {
    public:
        Topology() noexcept;

        [[nodiscard]] std::vector<NpuId> incomingNpus(NpuId dest) const noexcept;

        [[nodiscard]] int getNpusCount() const noexcept;

        [[nodiscard]] bool connected(NpuId src, NpuId dest) const noexcept;

        [[nodiscard]] Time linkTime(LinkId link, ChunkSize chunkSize) const noexcept;

        [[nodiscard]] bool getTopologyValue(LinkId link) const noexcept;

        [[nodiscard]] double getBW(NpuId src, NpuId dest);

        [[nodiscard]] int getLinksCount() const noexcept;

    protected:
        int npusCount;

        void setNpusCount(int newNpusCount) noexcept;

        void connect(NpuId src, NpuId dest, LinkAlphaBeta linkAlphaBeta, bool bidirectional = false) noexcept;

    private:
        std::vector<std::vector<bool>> topology;
        std::vector<std::vector<LinkWeight>> alpha;
        std::vector<std::vector<LinkWeight>> beta;

        int linksCount;
    };
}  // namespace Tacos
