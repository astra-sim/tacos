## ******************************************************************************
## This source code is licensed under the MIT license found in the
## LICENSE file in the root directory of this source tree.
##
## Copyright (c) 2022-2025 Intel Corporation
## Copyright (c) 2022-2025 Georgia Institute of Technology
## ******************************************************************************

# Base Image: Ubuntu
FROM ubuntu:25.04

# Project Metadata
LABEL maintainer="Will Won <william.won@gatech.edu>" \
    version="1.0" \
    description="TACOS"

# environment variables
ENV DEBIAN_FRONTEND=noninteractive
ENV LANG=C.UTF-8
ENV LC_ALL=C.UTF-8

# Install packages
RUN apt update
RUN apt install -y --no-install-recommends \
    build-essential coreutils vim \
    git ca-certificates \
    clang clang-format cmake \
    locales
RUN apt clean

# Set UTF-8 locale
RUN locale-gen en_US.UTF-8

# Update CA certificates
RUN update-ca-certificates

# Set working directory
WORKDIR /app/tacos
COPY . .

# default shell
CMD ["/bin/bash"]
