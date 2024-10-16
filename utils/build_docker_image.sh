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
echo "[TACOS] Building Docker Image..."

# build docker image
docker build -t astrasim/tacos:latest ${PROJECT_DIR:?}

# finalize
echo "[TACOS] Docker Image built: astrasim/tacos:latest"
echo "[TACOS] Consider running utils/start_docker_container.sh to start the container."
