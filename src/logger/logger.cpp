/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.

Copyright (c) 2022 Intel Corporation
Copyright (c) 2022 Georgia Institute of Technology
*******************************************************************************/

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <tacos/logger/logger.h>

using namespace tacos;

std::once_flag Logger::initFlag;

void Logger::init(const std::string& filename) noexcept {
    std::call_once(initFlag, [&]() {
        assert(!filename.empty());

        // get full file path
        const auto filePath = prepareFile(filename);
        assert(filePath.extension() == ".log");

        // console sink
        auto console_sink =
            std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(
            filePath.string(), true);

        // Combine sinks into a tee sink
        auto sinks = std::vector<spdlog::sink_ptr>{console_sink, file_sink};
        auto logger =
            std::make_shared<spdlog::logger>("TACOS", begin(sinks), end(sinks));

        // Set the global logger
        spdlog::set_default_logger(logger);

        // Print log file path
        Logger::info("TACOS");
        Logger::info("Log file path: ", filePath.string());
        Logger::info();
    });
}

void Logger::info() noexcept {
    spdlog::info("");
}

boost::filesystem::path Logger::prepareFile(
    const std::string& filename) noexcept {
    assert(!filename.empty());

    // prepare the logger
    auto currentPath = boost::filesystem::current_path();
    auto directoryPath = currentPath.append("results");

    // create directory, if not already exist
    if (!boost::filesystem::exists(directoryPath)) {
        boost::filesystem::create_directory(directoryPath);
    }

    // get filepath
    const auto filePath = directoryPath.append(filename);
    assert(filePath.has_extension());

    return filePath;
}
