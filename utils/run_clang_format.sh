#!/bin/bash
set -e

## ******************************************************************************
## This source code is licensed under the MIT license found in the
## LICENSE file in the root directory of this source tree.
## ******************************************************************************

# find the absolute path to this script
SCRIPT_DIR=$(dirname "$(realpath "$0")")
PROJECT_DIR="${SCRIPT_DIR:?}/.."

# start:
echo "Formatting TACOS Codebase..."

# format everything inside `src` directory
printf "\tFormatting src directory...\n"
find "${PROJECT_DIR:?}"/src \( -name "*.cpp" -o -name "*.h" \) -exec \
    clang-format -style=file -i {} \;

  printf "\tFormatting include directory...\n"
  find "${PROJECT_DIR:?}"/include \( -name "*.cpp" -o -name "*.h" \) -exec \
      clang-format -style=file -i {} \;

# finalize
echo "Formatting Done."
