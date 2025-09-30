#pragma once
#include <atomic>

struct Metrics {
    std::atomic<int> connected{0};
    std::atomic<int> events{0};
    std::atomic<int> tick_hz{20};
};
