/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include "ChakraWriterNpu.h"

#include "et_def.pb.h"
#include <iostream>

using namespace Tacos;

ChakraWriterNpu::ChunkDepInfo::ChunkDepInfo() noexcept : isPrecond(false), depNodeId(-1) {}

ChakraWriterNpu::ChakraWriterNpu(const int id, const int chunksCount, const ChunkSize chunkSize) noexcept : id(id), chunkSize(chunkSize) {
    for (auto c = 0; c < chunksCount; c++) {
        dependencyTable[c] = ChunkDepInfo();
    }
}

void ChakraWriterNpu::markPrecond(const ChunkId chunkId) noexcept {
    dependencyTable[chunkId].isPrecond = true;
}

void ChakraWriterNpu::addInlink(const int src) noexcept {
    inlinks[src] = ChakraWriterLink(src, id);
}

void ChakraWriterNpu::addOutlink(const int dest) noexcept {
    outlinks[dest] = ChakraWriterLink(id, dest);
}

void ChakraWriterNpu::addSendOp(const ChunkId chunkId, const NpuId dest) noexcept {
    // add dependenct recv op
    auto depNodeId = -1;
    const auto& depEntry = dependencyTable[chunkId];
    if (!depEntry.isPrecond) {
        depNodeId = depEntry.depNodeId;
        if (depNodeId == -1) {
            std::cout << "dependency not received!" << std::endl;
            assert(false);
            exit(-1);
        }
    }

    const auto [_, chakraNode] = outlinks[dest].addOp(chunkId, OpType::Send, depNodeId, chunkSize);
    et_nodes.push_back(chakraNode);
}

void ChakraWriterNpu::addRecvOp(const ChunkId chunkId, const NpuId src) noexcept {
    const auto [nodeId, chakraNode] = inlinks[src].addOp(chunkId, OpType::Recv, -1, chunkSize);
    et_nodes.push_back(chakraNode);

    // mark dependent op
    if (dependencyTable[chunkId].isPrecond) {
        std::cout << "precond received!" << std::endl;
        assert(false);
        exit(-1);
    }

    if (dependencyTable[chunkId].depNodeId != -1) {
        std::cout << "already received!" << std::endl;
        assert(false);
        exit(-1);
    }

    dependencyTable[chunkId].depNodeId = nodeId;
}

void ChakraWriterNpu::print() const noexcept {
    std::cout << "[NPU " << id << "]" << std::endl;
    for (const auto& [src, link] : outlinks) {
        std::cout << "(Send)" << std::endl;
        link.print();
    }
    for (const auto& [src, link] : inlinks) {
        std::cout << "(Recv)" << std::endl;
        link.print();
    }
}

void ChakraWriterNpu::writeToChakra(const std::string& filename) noexcept {
    const std::string etFilename = filename + "." + std::to_string(id) + ".et";
    auto* graph_stream = new ProtoOutputStream(etFilename);

    // assign global metadata
    ChakraProtoMsg::GlobalMetadata globalMetadata;

    auto* attribute = globalMetadata.add_attr();
    attribute->set_name("schema");
    attribute->set_string_val("1.0.2-chakra.0.0.4");
    graph_stream->write(globalMetadata);

    auto nodesCount = et_nodes.size();
    auto itr = et_nodes.begin();
    auto start = et_nodes.begin();

    while (nodesCount > 0) {
        Node* node = *itr;
        graph_stream->write(*node);
        delete node;

        itr++;
        nodesCount--;
    }

    et_nodes.erase(start, itr);
    delete graph_stream;
}
