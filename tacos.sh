#!/bin/bash
set -euo pipefail

## ******************************************************************************
## This source code is licensed under the MIT license found in the
## LICENSE file in the root directory of this source tree.
##
## Copyright (c) 2022-2025 Intel Corporation
## Copyright (c) 2022-2025 Georgia Institute of Technology
## ******************************************************************************

# path to the project directory
PROJECT_DIR="$(dirname "$(realpath "$0")")"

# useful directories
BUILD_DIR="$PROJECT_DIR/build"

# number of THREADS to use for building
THREADS=$((command -v nproc > /dev/null && nproc) || sysctl -n hw.logicalcpu)
if [[ $THREADS -gt 16 ]]; then
    THREADS=16  # limit to 16 THREADS
fi

# functions to compile and run TACOS
configure() {
    local test_flag="OFF"
    local build_type="Release"
    if [[ "${1:-}" == "--with-tests" ]]; then
        test_flag="ON"
        build_type="Debug"
    fi

    echo "[TACOS] Configuring project..."
    cmake -S "$PROJECT_DIR" \
        -B "$BUILD_DIR" \
        -DCMAKE_BUILD_TYPE="$build_type" \
        -DBUILD_TESTING="$test_flag" 
}

build() {
    echo "[TACOS] Building project (with $THREADS threads)..."
    cmake --build "$BUILD_DIR" --parallel "$THREADS"
}

run() {
    echo "[TACOS] Running TACOS..."
    "$BUILD_DIR/bin/tacos" "$@"
}

test() {
    echo "[TACOS] Running tests (with $THREADS threads)..."
    ctest --test-dir "$BUILD_DIR" --output-on-failure --parallel "$THREADS"
}

clean() {
    echo "[TACOS] Cleaning build directory..."
    rm -rf "$BUILD_DIR"
    rm -rf "$PROJECT_DIR/Testing"
}

usage() {
    local script_name=$(basename "$0")
    echo "Usage:"
    printf "\t%s configure [--with-tests]\t%s\n" "$script_name" "Configure the build system."
    printf "\t%s build\t%s\n" "$script_name" "Build the project."
    printf "\t%s run\t%s\n" "$script_name" "Run the compiled binary."
    printf "\t%s test\t%s\n" "$script_name" "Run tests."
    printf "\t%s clean\t%s\n" "$script_name" "Clean the build directory."
    printf "\t%s\t%s\n" "$script_name" "Run configure-build-run sequence."

    exit 1
}

# dispatch commands
case "${1:-}" in
    "")
        configure
        build
        run;;
    configure)
        configure "${2:-}";;
    build)
        build;;
    run)
        run "${@:2}";;
    test)
        test;;
    clean)
        clean;;
    *)
        usage;;
esac
