#pragma once
#include "metrics.hpp"
#include <atomic>
#include <thread>
#include <chrono>

class WorldLoop {
public:
    explicit WorldLoop(Metrics& m) : metrics_(m) {}
    void set_tick_hz(int hz) { metrics_.tick_hz.store(hz); }
    void start() {
        running_ = true;
        thr_ = std::thread([this]{
            using namespace std::chrono;
            while (running_) {
                // Simulate world tick generating events
                metrics_.events.fetch_add(1);
                std::this_thread::sleep_for(milliseconds(1000 / std::max(1, metrics_.tick_hz.load())));
            }
        });
    }
    void stop() {
        running_ = false;
        if (thr_.joinable()) thr_.join();
    }
    ~WorldLoop(){ stop(); }
private:
    Metrics& metrics_;
    std::atomic<bool> running_{false};
    std::thread thr_;
};
