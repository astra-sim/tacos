/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.

Copyright (c) 2022 Intel Corporation
Copyright (c) 2022 Georgia Institute of Technology
*******************************************************************************/

#include <cassert>
#include <tacos/logger/logger.h>
#include <tacos/writer/xml_writer.h>

using namespace tacos;

XmlWriter::XmlWriter(const std::string& filename,
                     const std::shared_ptr<Topology> topology,
                     const std::shared_ptr<Collective> collective,
                     SynthesisResult& synthesisResult) noexcept
    : topology_(topology),
      collective_(collective),
      synthesisResult_(synthesisResult) {
    path = Logger::prepareFile(filename).string();
}

void XmlWriter::write() noexcept {
    // write algo
    writeAlgo();

    // write each NPU
    for (auto npu = 0; npu < topology_->getNpusCount(); npu++) {
        writeNpu(npu);
    }

    save();
}

void XmlWriter::writeAlgo() noexcept {
    algo = xml.append_child("algo");

    algo.append_attribute("name") = "tacos";
    algo.append_attribute("proto") = "LL128";
    algo.append_attribute("nchannels") = 1;
    algo.append_attribute("nchunksperloop") = collective_->chunksPerNpu();
    algo.append_attribute("ngpus") = topology_->getNpusCount();
    algo.append_attribute("coll") = "allgather";  // FIXME: can be others
    algo.append_attribute("inplace") = 1;
}

void XmlWriter::writeNpu(NpuID npuID) noexcept {
    // write all ingress threadblocks
    auto npu = algo.append_child("gpu");
    npu.append_attribute("id") = npuID;
    npu.append_attribute("i_chunks") = 0;
    npu.append_attribute("o_chunks") = collective_->chunksPerNpu();
    npu.append_attribute("s_chunks") = 0;

    // write all ingress threadblocks
    for (const auto& [src, link] : synthesisResult_.npu(npuID).ingressLinks()) {
        writeIngressLink(npu, npuID, src, link);
    }

    // write all egress threadblocks
    for (const auto& [dest, link] : synthesisResult_.npu(npuID).egressLinks()) {
        writeEgressLink(npu, npuID, dest, link);
    }
}

void XmlWriter::save() noexcept {
    if (xml.save_file(path.c_str(), PUGIXML_TEXT("\t"),
                      pugi::format_no_declaration)) {
        Logger::info("XML file written at: ", path);
    } else {
        Logger::info("XML file writing failed");
    }
}

void XmlWriter::writeIngressLink(pugi::xml_node& gpu,
                          NpuID npu,
                          NpuID src,
                          const LinkResult& link) noexcept {
    auto tb = gpu.append_child("tb");
    tb.append_attribute("id") = link.id();
    tb.append_attribute("send") = -1;
    tb.append_attribute("recv") = src;
    tb.append_attribute("chan") = 0;  // FIXME: multi-channel

    // iterate over all steps
    for (const auto& [opID, op] : link.ops()) {
        const auto chunkID = op.chunkID();

        auto step = tb.append_child("step");

        step.append_attribute("s") = opID;
        step.append_attribute("type") = "r";
        step.append_attribute("srcbuf") = "o";
        step.append_attribute("srcoff") = chunkID;
        step.append_attribute("dstbuf") = "o";
        step.append_attribute("dstoff") = chunkID;
        step.append_attribute("cnt") = 1;
        step.append_attribute("depid") = -1;
        step.append_attribute("deps") = -1;
        step.append_attribute("hasdep") = 0;
    }
}

void XmlWriter::writeEgressLink(pugi::xml_node& gpu,
                          NpuID npu,
                          NpuID dest,
                          const LinkResult& link) noexcept {
    auto tb = gpu.append_child("tb");
    tb.append_attribute("id") = link.id();
    tb.append_attribute("send") = dest;
    tb.append_attribute("recv") = -1;
    tb.append_attribute("chan") = 0;  // FIXME: multi-channel

    // iterate over all steps
    for (const auto& [opID, op] : link.ops()) {
        const auto chunkID = op.chunkID();

        auto step = tb.append_child("step");


        step.append_attribute("s") = opID;
        step.append_attribute("type") = "s";
        step.append_attribute("srcbuf") = "o";
        step.append_attribute("srcoff") = chunkID;
        step.append_attribute("dstbuf") = "o";
        step.append_attribute("dstoff") = chunkID;
        step.append_attribute("cnt") = 1;

        // check for dependency
        if (op.hasDep()) {
            step.append_attribute("depid") = op.depOp()->linkID();;
            step.append_attribute("deps") = op.depOp()->opID();
            step.append_attribute("hasdep") = 1;
        } else {
            step.append_attribute("depid") = -1;
            step.append_attribute("deps") = -1;
            step.append_attribute("hasdep") = 0;
        }

    }
}
