## Use Ubuntu
FROM ubuntu:24.04
LABEL maintainer="Will Won <william.won@gatech.edu>"


### ================== System Setups ======================
## Install System Dependencies
ENV DEBIAN_FRONTEND=noninteractive
RUN apt -y update
RUN apt -y install \
    coreutils vim git \
    gcc g++ clang-format \
    make cmake
### ======================================================


### ================== Finalize ==========================
## Move to the application directory
WORKDIR /app
### ======================================================
