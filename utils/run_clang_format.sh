#!/bin/sh
set -euo pipefail

## ******************************************************************************
## This source code is licensed under the MIT license found in the
## LICENSE file in the root directory of this source tree.
##
## Copyright (c) 2022-2025 Intel Corporation
## Copyright (c) 2022-2025 Georgia Institute of Technology
## ******************************************************************************

# find the absolute path to this script
SCRIPT_DIR=$(dirname "$(realpath "$0")")
PROJECT_DIR="${SCRIPT_DIR:?}/.."

# function to format a given directory
function run_clang_format {
  find "${PROJECT_DIR:?}/$1" \( -name "*.cpp" -o -name "*.h" \) \
    -exec clang-format -style=file -i {} \;
}

# start:
echo "[TACOS] Formatting codebase using clang-format."

# format directories
targets=("src" "include" "tests")
for dir in ${targets[@]}; do
  printf "\tFormatting ${dir} directory...\n"
  run_clang_format "${dir}"
done

# finalize
echo "[TACOS] Formatting Done."
