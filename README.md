<!--
## This source code is licensed under the MIT license found in the
## LICENSE file in the root directory of this source tree.
##
## Copyright (c) 2022-2025 Intel Corporation
## Copyright (c) 2022-2025 Georgia Institute of Technology
-->

# 🌮 TACOS
### [T]opology-[A]ware [Co]llective Algorithm [S]ynthesizer for Distributed Machine Learning

## Latest Release
[Latest Release](https://github.com/astra-sim/tacos/releases)

## Project Status
| branch | Build | Test | Format |
|:---:|:---:|:---:|:---:|
| **main** | [![build-ubuntu](https://github.com/astra-sim/tacos/actions/workflows/build-ubuntu.yml/badge.svg?branch=main)](https://github.com/astra-sim/tacos/actions/workflows/build-ubuntu.yml) [![build-macos](https://github.com/astra-sim/tacos/actions/workflows/build-macos.yml/badge.svg?branch=main)](https://github.com/astra-sim/tacos/actions/workflows/build-macos.yml) | [![test-ubuntu](https://github.com/astra-sim/tacos/actions/workflows/test-ubuntu.yml/badge.svg?branch=main)](https://github.com/astra-sim/tacos/actions/workflows/test-ubuntu.yml) [![test-macos](https://github.com/astra-sim/tacos/actions/workflows/test-macos.yml/badge.svg?branch=main)](https://github.com/astra-sim/tacos/actions/workflows/test-macos.yml) | [![format](https://github.com/astra-sim/tacos/actions/workflows/check-clang-format.yml/badge.svg?branch=main)](https://github.com/astra-sim/tacos/actions/workflows/check-clang-format.yml) |
| **develop** | [![build-ubuntu](https://github.com/astra-sim/tacos/actions/workflows/build-ubuntu.yml/badge.svg?branch=develop)](https://github.com/astra-sim/tacos/actions/workflows/build-ubuntu.yml) [![build-macos](https://github.com/astra-sim/tacos/actions/workflows/build-macos.yml/badge.svg?branch=develop)](https://github.com/astra-sim/tacos/actions/workflows/build-macos.yml) | [![test-ubuntu](https://github.com/astra-sim/tacos/actions/workflows/test-ubuntu.yml/badge.svg?branch=develop)](https://github.com/astra-sim/tacos/actions/workflows/test-ubuntu.yml) [![test-macos](https://github.com/astra-sim/tacos/actions/workflows/test-macos.yml/badge.svg?branch=develop)](https://github.com/astra-sim/tacos/actions/workflows/test-macos.yml) | [![format](https://github.com/astra-sim/tacos/actions/workflows/check-clang-format.yml/badge.svg?branch=develop)](https://github.com/astra-sim/tacos/actions/workflows/check-clang-format.yml) |

# Overview
TACOS is a topology-aware collective algorithm synthesizer:
- TACOS receives a network topology description and target collective patterns.
- Then, TACOS autonomously analyzes provided inputs and synthesizes topology-aware collective algorithms.

The below figure summarizes the TACOS framework:
![TACOS Abstraction](https://github.com/astra-sim/tacos/blob/main/docs/tacos_overview.png)

## Constraints
TACOS currently supports:
- Network topology: point-to-point (direct-connect) only. Networks can be asymmetric and heterogeneous.
  - Switch should be unwound to point-to-point connections.
- Target collective pattern: All-Gather
  - Although Reduce-Scatter and All-Reduce can be supported by TACOS, their implementations are currently in progress.
  - All-to-All is not supported in TACOS.
- Output: TACOS currently reports the estimated collective time of the synthesized collective algorithm.
  - MSCCL-XML generation is currently in progress so that the TACOS algorithm can run on real systems via MSCCL (see [MSCCLang Paper](https://dl.acm.org/doi/10.1145/3575693.3575724)).

## Paper
Please find more information about the framework in the TACOS paper [[IEEExplorer]](https://arxiv.org/abs/2304.05301) [[arXiv]](https://arxiv.org/abs/2304.05301).
- You can cite the paper (BibTeX) by clicking the *Cite this repository* button (on the right side toolbar below the *About* tab).

# Getting Started
## Prerequisite
TACOS is a C++17-based project, using CMake as the build manager.
- `g++` or `clang`
- `cmake >= 3.26`

### Docker
We also deploy TACOS in Docker Image. You can pull this image via Docker Hub.
```sh
docker pull astrasim/tacos:latest
docker run -it astrasim/tacos:latest
```

Instead, you may build the Docker Image locally via this repository itself.
```sh
./utils/build_docker_image.sh
./utils/start_docker_container.sh
```

## Compiling TACOS
You can compile TACOS using the provided script (`tacos.sh`)
```sh
./tacos.sh configure  # Running CMake to configure the build
./tacos.sh build  # Compiles TACOS
```

After a successful build, the project will compile TACOS as a library file (`libtacos.a`) as well as an example executable file from `src/main.cpp`
```text
build
├── bin
│   └── tacos
└── lib
    └── libtacos.a
```

## Executing TACOS
You can run the compiled example binary (`build/bin/tacos`) either directly or via the provided script.
```sh
./tacos.sh run
```

## Regression Tests
TACOS is also equipped with a small set of simple regression tests (inside the `tests/` directory). You can compile and run these tests via the script.
```sh
./tacos.sh configure --with-tests  # Enables Debug mode and tests/ compilation
./tacos.sh build
./tacos.sh test  # Runs ctest for regression tests
```
Since this builds TACOS with debug mode (which is significantly slower), we recommend to re-compile TACOS without passing the `--with-tests` option to enable compiler optimizations.

# Contact Us
For any questions about TACOS, please contact [Will Won](mailto:william.won@gatech.edu)
or [Tushar Krishna](mailto:tushar@ece.gatech.edu). You can also search for or create new GitHub issues.

## Contributing
We sincerely appreciate your contribution to the TACOS project! Please see [CONTRIBUTING.md]() for contribution guidelines.
