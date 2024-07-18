#!/bin/sh
set -e

## ******************************************************************************
## This source code is licensed under the MIT license found in the
## LICENSE file in the root directory of this source tree.
## ******************************************************************************

# find the absolute path to this script
SCRIPT_DIR=$(dirname "$(realpath "$0")")
TARGET_DIR="$SCRIPT_DIR/.."

# folders to run clang-format
folders=("collective" "helper" "include" "runner" "synthesizer" "topology")

# start:
echo "Formatting TACOS Codebase:"

for folder in "${folders[@]}"; do
    printf "\tFormatting $folder:\n"
    find "$TARGET_DIR/$folder" \( -name "*.cpp" -o -name "*.h" \) -exec \
        clang-format -style=file -i {} \;
done

# run clang-format for `co# # finalize
echo "Formatting Done."
