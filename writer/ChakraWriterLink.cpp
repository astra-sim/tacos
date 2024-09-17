/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include "ChakraWriterLink.h"
#include <iostream>

using namespace Tacos;

ChakraWriterLink::ChakraWriterLink(const NpuId src, const NpuId dest) noexcept : src(src), dest(dest) {}

ChakraWriterLink::ChakraWriterLink() noexcept : ChakraWriterLink(-1, -1) {}

std::pair<int, ChakraProtoMsg::Node*> ChakraWriterLink::addOp(const ChunkId chunkId, const OpType opType, const int depNodeId) noexcept {
    auto& newNode = synthesisResult.emplace_back(chunkId, src, dest, opType);

    // add trivial dependency: prev node
    if (synthesisResult.size() >= 2) {
        const auto depId = synthesisResult[synthesisResult.size() - 2].getNodeId();
        newNode.addDependency(depId);
    }

    // if another dependency, assign it
    if (depNodeId != -1) {
            newNode.addDependency(depNodeId);
    }

    auto* chakraNode = newNode.generateChakraNode();
    auto nodeId = newNode.getNodeId();
    return std::make_pair(nodeId, chakraNode);
}

void ChakraWriterLink::print() const noexcept {
    std::cout << "\t[" << src << "->" << dest << "]: ";
    for (auto chakraNode : synthesisResult) {
        chakraNode.print();
        std::cout << " ";
    }
    std::cout << std::endl;
}
