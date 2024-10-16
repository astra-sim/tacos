/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include <iostream>
#include <sstream>
#include <tacos/writer/chakra_writer.h>

using namespace tacos;

ChakraWriter::ChakraWriter(std::shared_ptr<Topology> topology,
                           std::shared_ptr<Collective> collective,
                           SynthesisResult synthesisResult) noexcept
    : Writer(topology, collective, synthesisResult) {}

void ChakraWriter::write(const std::string& filename) const noexcept {
    for (auto npu = 0; npu < npusCount; npu++) {
        const auto ingressChakraNodes = constructIngressChakraNodes(npu);
        const auto egressChakraNodes = constructEgressChakraNodes(npu);
    }
}

std::string ChakraWriter::getNodeName(const ChakraNodeID chakraNodeID,
                                      const ChunkID chunk,
                                      const NpuID src,
                                      const NpuID dest,
                                      const OpType opType) const noexcept {
    std::ostringstream nodeName;

    nodeName << "Node " << chakraNodeID << ", ";
    nodeName << "Chunk: " << chunk << ", ";

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

std::unique_ptr<ChakraWriter::ChakraNode> ChakraWriter::generateChakraNode(
    const ChunkID chunk, const NpuID src, const NpuID dest, const OpType opType) noexcept {
    // create a new node
    auto node = std::make_unique<ChakraNode>();

    // assign node id
    const auto chakraNodeID = newChakraNodeID;
    newChakraNodeID++;

    node->set_id(chakraNodeID);

    // assign node name
    const auto nodeName = getNodeName(chakraNodeID, chunk, src, dest, opType);
    node->set_name(nodeName);

    // assign node type
    switch (opType) {
    case OpType::Send:
        node->set_type(ChakraNodeType::COMM_SEND_NODE);
        break;
    case OpType::Recv:
        node->set_type(ChakraNodeType::COMM_RECV_NODE);
        break;
    default:
        assert(false);
        exit(-1);
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
        attrCommType->set_int32_val(ChakraNodeType::COMM_SEND_NODE);

        attrNpu->set_name("comm_dst");
        attrNpu->set_int32_val(dest);
        break;
    case OpType::Recv:
        attrCommType->set_int32_val(ChakraNodeType::COMM_RECV_NODE);

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
