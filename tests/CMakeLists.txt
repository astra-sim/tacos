## ******************************************************************************
## This source code is licensed under the MIT license found in the
## LICENSE file in the root directory of this source tree.
##
## Copyright (c) 2022-2025 Intel Corporation
## Copyright (c) 2022-2025 Georgia Institute of Technology
## ******************************************************************************

# TACOS tests
add_executable(tacos_tests
    test_config.h
    test_tacos_mesh_2d.cpp
    test_tacos_mesh_2d_hetero.cpp
    test_tacos_hypercube_3d.cpp
    test_tacos_torus_3d.cpp
)
target_link_libraries(tacos_tests PRIVATE tacos)
target_include_directories(tacos_tests PRIVATE ${CMAKE_SOURCE_DIR}/tests)

# Include googletest
target_link_libraries(tacos_tests PRIVATE gtest_main)

# Enable CTest integration
include(GoogleTest)
gtest_discover_tests(tacos_tests)
