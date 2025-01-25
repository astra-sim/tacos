/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.

Copyright (c) 2022 Intel Corporation
Copyright (c) 2022 Georgia Institute of Technology
*******************************************************************************/

#pragma once

#include <memory>
#include <pugixml.hpp>
#include <tacos/collective/collective.h>
#include <tacos/topology/topology.h>
#include <tacos/writer/synthesis_result.h>

namespace tacos {

class XmlWriter {
  public:
    XmlWriter(const std::string& filename,
              std::shared_ptr<Topology> topology,
              std::shared_ptr<Collective> collective,
              SynthesisResult& synthesisResult) noexcept;

    void write() noexcept;

  private:
    using NpuID = Topology::NpuID;

    pugi::xml_document xml;
    pugi::xml_node algo;
    std::string path;

    std::shared_ptr<Topology> topology_;
    std::shared_ptr<Collective> collective_;
    SynthesisResult& synthesisResult_;

    void writeAlgo() noexcept;
    void writeNpu(NpuID npuID) noexcept;

    void writeIngressLink(pugi::xml_node& gpu,
                          NpuID npu,
                          NpuID src,
                          const LinkResult& link) noexcept;

    void writeEgressLink(pugi::xml_node& gpu,
                         NpuID npu,
                         NpuID dest,
                         const LinkResult& link) noexcept;

    void save() noexcept;
};

}  // namespace tacos
