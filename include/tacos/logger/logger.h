/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.

Copyright (c) 2022 Intel Corporation
Copyright (c) 2022 Georgia Institute of Technology
*******************************************************************************/

#pragma once

#include <boost/filesystem.hpp>
#include <spdlog/spdlog.h>

namespace tacos {

class Logger {
  public:
    static void init(const std::string& filename) noexcept;

    static void info() noexcept;

    template <typename... Args>
    static void info(Args&&... args) noexcept;

    [[nodiscard]] static boost::filesystem::path prepareFile(
        const std::string& filename) noexcept;

  private:
    static std::once_flag initFlag;
};

}  // namespace tacos

#include <tacos/logger/logger.tpp>
