/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.

Copyright (c) 2022-2025 Intel Corporation
Copyright (c) 2022-2025 Georgia Institute of Technology
*******************************************************************************/

#pragma once

#include <gtest/gtest.h>

class TestConfig : public ::testing::Test {
  protected:
    static constexpr double tolerance = 0.01;
    static constexpr int repeat = 20;
};
