#pragma once
#include <string>
#include <sstream>

// Tiny line-oriented protocol for demo.
// Client sends: "PING\n" or "SUB <topic>\n"
// Server replies: "PONG\n" or status lines.
// World state is multicast over UDP as JSON.

inline std::string make_json_metrics(int connected, int events_sec, int tick_hz) {
    std::ostringstream oss;
    oss << "{\"connected_clients\":" << connected
        << ",\"events_per_sec\":" << events_sec
        << ",\"tick_hz\":" << tick_hz << "}";
    return oss.str();
}
