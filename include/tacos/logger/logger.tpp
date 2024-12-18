/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.

Copyright (c) 2022 Intel Corporation
Copyright (c) 2022 Georgia Institute of Technology
*******************************************************************************/

#pragma once

#include <sstream>
#include <tacos/logger/logger.h>

using namespace tacos;

template <typename... Args>
void Logger::info(Args&&... args) noexcept {
    auto log = std::ostringstream();
    log << std::fixed;
    log.precision(2);

    // concatenate all logs
    (log << ... << std::forward<Args>(args));
    spdlog::info(log.str());
}
