/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/


#include "ChakraNode.h"
// #include "protoio.hh"
// #include "et_def.pb.h"
#include <iostream>
#include <sstream>

using namespace Tacos;

int ChakraNode::nextNodeId = 0;

ChakraNode::ChakraNode(const ChunkId chunkId, const NpuId src, const NpuId dest, const OpType opType) noexcept
    : chunkId(chunkId),
      nodeId(nextNodeId),
      src(src),
      dest(dest),
      opType(opType) {
    ChakraNode::nextNodeId++;
}

int ChakraNode::getNodeId() const noexcept {
    return nodeId;
}

void ChakraNode::print() const noexcept {
    std::cout << "(Chunk: " << chunkId << ", chakra: " << nodeId << ", dep: [";
    for (const auto dep : dependencies) {
        std::cout << dep << ", ";
    }
    std::cout << "])";
}

void ChakraNode::addDependency(int depNodeId) noexcept {
    dependencies.insert(depNodeId);
}

std::string ChakraNode::getNodeName() const noexcept {
    std::ostringstream nodeName;
    nodeName << "Node " << nodeId << ", ";
    nodeName << "Chunk: " << chunkId << ", ";
    switch (opType) {
    case OpType::Send:
        nodeName << "Send: ";
        break;
    case OpType::Recv:
        nodeName << "Recv: ";
        break;
    default:
        assert(false);
        exit(-1);
    }
    nodeName << src << " -> " << dest;

    return nodeName.str();
}

ChakraProtoMsg::Node* ChakraNode::generateChakraNode() const noexcept {
    Node* node = new Node;

    node->set_id(nodeId);

    const auto nodeName = getNodeName();
    node->set_name(nodeName);

    switch (opType) {
    case OpType::Send:
        node->set_type(NodeType::COMM_SEND_NODE);
        break;
    case OpType::Recv:
        node->set_type(NodeType::COMM_RECV_NODE);
        break;
    default:
        assert(false);
        exit(-1);
    }

    for (const auto dep : dependencies) {
        node->add_data_deps(dep);
    }

    return node;
}
