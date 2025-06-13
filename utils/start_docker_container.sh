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
SCRIPT_DIR="$(dirname "$(realpath "$0")")"
PROJECT_DIR="${SCRIPT_DIR:?}"/..

# start
echo "[TACOS] Starting a Docker Container fromastra-sim/tacos:latest..."

# build docker image
docker run -it astra-sim/tacos:latest /bin/bash
