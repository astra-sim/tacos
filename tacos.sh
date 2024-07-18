#!/bin/sh
set -e

## ******************************************************************************
## This source code is licensed under the MIT license found in the
## LICENSE file in the root directory of this source tree.
## ******************************************************************************

# find the absolute path to this script
PROJECT_DIR=$(dirname "$(realpath "$0")")
BUILD_DIR="$PROJECT_DIR/build"

# compile TACOS
function compile {
    cmake -S "$PROJECT_DIR" -B "$BUILD_DIR" -DCMAKE_BUILD_TYPE=Release
    cmake --build "$BUILD_DIR" --parallel $(nproc)
}

# run TACOS
function run {
    ./build/bin/tacos
}

# cleanup build
function cleanup {
    rm -rf "$BUILD_DIR"
}

# help message
function print_help {
    echo "TACOS:"
    printf "\t--help (-h): Print this message\n"
    printf "\t--compile (-c): Compile TACOS\n"
    printf "\t--run (-r): Run the compiled TACOS executable\n"
    printf "\t--clean (-l): Remove the TACOS build directory\n"
    printf "\t(noflag): Compile and execute TACOS\n"
}

# execute
# no flag: compile and run
if [ $# -eq 0 ]; then
    compile
    run
    exit $?
fi

# flag given
case "$1" in
    -h|--help)
        print_help
        ;;
    -l|--clean)
        cleanup
        ;;
    -c|--compile)
        compile
        ;;
    -r|--run)
        run
        ;;
    *)
        echo "[TACOS] Unknown flag: $1"
        print_help
        exit 1
        ;;
esac
