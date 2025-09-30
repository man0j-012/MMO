#!/usr/bin/env bash
set -euo pipefail
./build/src/server/mmo_server --tcp 4000 --mgroup 239.255.0.1 --mport 30001 --iface 0.0.0.0
