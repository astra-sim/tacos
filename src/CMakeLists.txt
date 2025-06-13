## ******************************************************************************
## This source code is licensed under the MIT license found in the
## LICENSE file in the root directory of this source tree.
##
## Copyright (c) 2022-2025 Intel Corporation
## Copyright (c) 2022-2025 Georgia Institute of Technology
## ******************************************************************************

# TACOS library
add_library(tacos
    topology/topology.cpp ${CMAKE_SOURCE_DIR}/include/tacos/topology/topology.h
    topology/mesh_2d.cpp ${CMAKE_SOURCE_DIR}/include/tacos/topology/mesh_2d.h
    topology/mesh_2d_hetero.cpp ${CMAKE_SOURCE_DIR}/include/tacos/topology/mesh_2d_hetero.h
    topology/torus_2d.cpp ${CMAKE_SOURCE_DIR}/include/tacos/topology/torus_2d.h
    topology/torus_3d.cpp ${CMAKE_SOURCE_DIR}/include/tacos/topology/torus_3d.h
    topology/hypercube_3d.cpp ${CMAKE_SOURCE_DIR}/include/tacos/topology/hypercube_3d.h
    collective/collective.cpp ${CMAKE_SOURCE_DIR}/include/tacos/collective/collective.h
    collective/all_gather.cpp ${CMAKE_SOURCE_DIR}/include/tacos/collective/all_gather.h
    event_queue/event_queue.cpp ${CMAKE_SOURCE_DIR}/include/tacos/event_queue/event_queue.h
    event_queue/timer.cpp ${CMAKE_SOURCE_DIR}/include/tacos/event_queue/timer.h
    synthesizer/time_expanded_network.cpp ${CMAKE_SOURCE_DIR}/include/tacos/synthesizer/time_expanded_network.h
    synthesizer/synthesizer.cpp ${CMAKE_SOURCE_DIR}/include/tacos/synthesizer/synthesizer.h
)
target_include_directories(tacos
    PUBLIC ${CMAKE_SOURCE_DIR}/include
    PUBLIC ${CMAKE_SOURCE_DIR}/include/tacos
)

# TACOS executable
add_executable(tacos_exec
    main.cpp
)
target_link_libraries(tacos_exec PRIVATE tacos)
set_target_properties(tacos_exec PROPERTIES OUTPUT_NAME tacos)
