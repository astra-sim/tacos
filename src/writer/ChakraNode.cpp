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

ChakraProtoMsg::Node* ChakraNode::generateChakraNode(const NpuId src, const NpuId dest, const ChunkSize chunkSize) const noexcept {
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

    // add attributes
    const auto chunkSizeInt = static_cast<int>(chunkSize);
    auto* attrCommSize = node->add_attr();
    attrCommSize->set_name("comm_size");
    attrCommSize->set_int32_val(chunkSizeInt);

    auto* attrCommType = node->add_attr();
    attrCommType->set_name("comm_type");
    auto* attrNpu = node->add_attr();

    switch (opType) {
    case OpType::Send:
        attrCommType->set_int32_val(NodeType::COMM_SEND_NODE);

        attrNpu->set_name("comm_dst");
        attrNpu->set_int32_val(dest);
        break;
    case OpType::Recv:
        attrCommType->set_int32_val(NodeType::COMM_RECV_NODE);

        attrNpu->set_name("comm_src");
        attrNpu->set_int32_val(src);
        break;
    default:
        assert(false);
        exit(-1);
    }

    auto* attrCommTag = node->add_attr();
    attrCommTag->set_name("comm_tag");
    attrCommTag->set_int32_val(0);

    auto* attrCommCpu = node->add_attr();
    attrCommCpu->set_name("is_cpu_op");
    attrCommCpu->set_int32_val(0);

    return node;
}
