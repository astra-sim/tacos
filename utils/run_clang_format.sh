#!/bin/bash
set -e

## ******************************************************************************
## This source code is licensed under the MIT license found in the
## LICENSE file in the root directory of this source tree.
## ******************************************************************************

# find the absolute path to this script
SCRIPT_DIR=$(dirname "$(realpath "$0")")
TARGET_DIR="$SCRIPT_DIR/../src"

# start:
echo "Formatting TACOS Codebase..."

# format everything inside `src` directory
find "$TARGET_DIR" \( -name "*.cpp" -o -name "*.h" \) -exec \
    clang-format -style=file -i {} \;

# finalize
echo "Formatting Done."
