# Check CMake version
cmake_minimum_required(VERSION 3.25)

# Create Project
project(TACOS)

# Setup compilation options
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set build type
set(CMAKE_BUILD_TYPE Release)

# Debug option
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")

# find Protobuf
find_package(protobuf CONFIG REQUIRED)

# Compile external libraries
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/extern/yaml-cpp yaml-cpp)

# Project include option
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/tacos)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/extern/chakra/schema/protobuf)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/extern/chakra/src/third_party/utils)

# Target (common) compilation files
file(GLOB srcs
        ${CMAKE_CURRENT_SOURCE_DIR}/helper/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/topology/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/collective/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/synthesizer/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/writer/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/extern/chakra/schema/protobuf/*.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/extern/chakra/src/third_party/utils/*.cc
)

add_executable(tacos ${srcs})
target_sources(tacos PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/runner/main.cpp)
target_link_libraries(tacos PUBLIC yaml-cpp)
target_link_libraries(tacos PUBLIC protobuf::libprotobuf)
set_target_properties(tacos PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)
