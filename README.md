# Distributed Multicast MMO Server – System Design and Scalability (C++17)

This is a **clean, upload-ready** starter implementation for your repo **`man0j-012/MMO`**.
It demonstrates a scalable architecture with:

- **Boost.Asio (TCP)** for client sessions
- **UDP Multicast** for world-state fan-out (zone updates / metrics)
- **Qt GUI** monitor to visualize live metrics
- **CMake** build, **Docker** image, **Docker Swarm** stack
- **Load generator** to hit 1000s of lightweight simulated clients (threads or async)


---

## Build (Linux)

Dependencies:
- CMake ≥ 3.16
- g++ / clang with C++17
- Boost (headers + system)
- Qt5 (or Qt6) Widgets

### Ubuntu example
```bash
sudo apt-get update
sudo apt-get install -y build-essential cmake libboost-all-dev qtbase5-dev
```

### Configure & build
```bash
mkdir -p build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DBUILD_GUI=ON
cmake --build . -j
```

Outputs:
- `mmo_server` (TCP server + UDP multicast broadcaster)
- `mmo_client` (simple client; also used by loadgen baseline)
- `mmo_loadgen` (multi-connection load generator)
- `mmo_gui_monitor` (Qt GUI showing live multicast metrics)

---

## Run locally

### 1) Start server
```bash
./build/src/server/mmo_server --tcp 4000 --mgroup 239.255.0.1 --mport 30001 --iface 0.0.0.0
```

### 2) Start load generator (e.g., 2000 connections)
```bash
./build/src/tools/loadgen/mmo_loadgen --host 127.0.0.1 --port 4000 --clients 2000 --rate 10
```

### 3) Start GUI Monitor
```bash
./build/gui/mmo_gui_monitor --mgroup 239.255.0.1 --mport 30001
```

You should see connections ramp up and the GUI showing **connected_clients**, **events/sec**, etc.

---

## Docker (server only)

```bash
docker build -t mmo-server -f docker/Dockerfile.server .
docker run --net=host mmo-server --tcp 4000 --mgroup 239.255.0.1 --mport 30001 --iface 0.0.0.0
```

> `--net=host` is convenient for multicast on Linux. Adjust for your network.

### Swarm (example)
```bash
docker swarm init
docker stack deploy -c docker/swarm-stack.yml mmo_stack
```

---

## Project Layout

```
src/
  common/           # shared types and helpers
  server/           # server, sessions, multicast broadcaster
  client/           # simple client
  tools/loadgen/    # load generator
gui/                # Qt monitor app (subscribes to multicast)
docker/             # Dockerfile + swarm stack
config/             # sample config
scripts/            # helper scripts
```

---

MIT License.
