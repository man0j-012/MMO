#pragma once
#include <string>
#include <chrono>
#include <atomic>

inline uint64_t now_ms() {
    using namespace std::chrono;
    return duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
}

struct Args {
    std::string host = "0.0.0.0";
    int tcp_port = 4000;
    std::string mgroup = "239.255.0.1";
    int mport = 30001;
    std::string iface = "0.0.0.0";
};
