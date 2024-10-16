#!/bin/bash
set -e

## ******************************************************************************
## This source code is licensed under the MIT license found in the
## LICENSE file in the root directory of this source tree.
## ******************************************************************************

# find the absolute path to this script
SCRIPT_DIR=$(dirname "$(realpath "$0")")
PROJECT_DIR="${SCRIPT_DIR:?}/.."

# start
echo "[TACOS] Starting a Docker Container from astrasim/tacos:latest..."

# build docker image
docker run -i -t \
    -v "${PROJECT_DIR:?}:/app/tacos" \
    astrasim/tacos:latest \
    /bin/bash
