#!/usr/bin/env bash
set -euo pipefail
mkdir -p build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DBUILD_GUI=ON
cmake --build . -j
