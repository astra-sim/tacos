[//]: # (This source code is licensed under the MIT license found in the)
[//]: # (LICENSE file in the root directory of this source tree.)

# 🌮 TACOS
## [T]opology-[A]ware [Co]llective Algorithm [S]ynthesizer for Distributed Machine Learning

## Latest Release
[Latest Release](https://github.com/astra-sim/tacos/releases)

## Project Status
| branch | macOS | Ubuntu | Format | Coverage |
|:---:|:---:|:---:|:---:|:---:|
| **main** | TBA | TBA | [![format](https://github.com/astra-sim/tacos/actions/workflows/check-clang-format.yml/badge.svg?branch=main)](https://github.com/astra-sim/tacos/actions/workflows/check-clang-format.yml) | TBA |
| **develop** | TBA | TBA | [![format](https://github.com/astra-sim/tacos/actions/workflows/check-clang-format.yml/badge.svg?branch=develop)](https://github.com/astra-sim/tacos/actions/workflows/check-clang-format.yml) | TBA |

## Overview
TACOS receives an arbitrary point-to-point network topology and autonomously synthesizes the topology-aware All-Reduce (Reduce-Scatter and All-Gather) collective communication algorithm. TACOS is powered by the Time-expanded Network (TEN) representation and Utilization Maximizing Link-Chunk Matching algorithm, thereby resulting in greater scalability to large networks.

Below figure summarizes the TACOS framework:
![TACOS Abstraction](https://github.com/astra-sim/tacos/blob/main/docs/images/tacos_overview.png)

Please find more information about TACOS in [this paper](https://arxiv.org/abs/2304.05301).
- William Won, Midhilesh Elavazhagan, Sudarshan Srinivasan, Swati Gupta, and Tushar Krishna, "TACOS: Topology-Aware Collective Algorithm Synthesizer for Distributed Machine Learning," arXiv:2304.05301 [cs.DC]

## Getting Started
We highly recommend using the provided Docker image as the runtime environment, since TACOS requires several dependencies including protobuf and boost. You can either download the Docker image from the Docker Hub, or you may build one locally using the provided script.

1. Download the TACOS project.
```sh
git clone --recurse-submodules https://github.com/astra-sim/tacos.git
```

2. Pull the TACOS Docker Image.
```sh
docker pull astrasim/tacos:latest

# Instead, you may consider building this Docker Image locally.
./utils/build_docker_image.sh
```

3. Start the Docker Container (which becomes your TACOS runtime environment).
```sh
./utils/start_docker_container.sh
```

4. Run TACOS with the provided script.
```sh
[docker] ./tacos.sh
```

If you'd like to analyze the codebase, `src/main.cpp` is the main entry point.

## Contact Us
For any questions about TACOS, please contact [Will Won](mailto:william.won@gatech.edu)
or [Tushar Krishna](mailto:tushar@ece.gatech.edu). You may also find or open a GitHub Issue in this repository.
