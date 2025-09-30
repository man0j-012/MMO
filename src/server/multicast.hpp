#pragma once
#include <string>
#include <thread>
#include <atomic>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

class MulticastBroadcaster {
public:
    MulticastBroadcaster(std::string group, int port, std::string iface)
        : group_(std::move(group)), port_(port), iface_(std::move(iface)) {}

    bool start() {
        sock_ = socket(AF_INET, SOCK_DGRAM, 0);
        if (sock_ < 0) return false;

        int ttl = 1;
        setsockopt(sock_, IPPROTO_IP, IP_MULTICAST_TTL, &ttl, sizeof(ttl));

        in_addr iaddr{};
        iaddr.s_addr = inet_addr(iface_.c_str());
        setsockopt(sock_, IPPROTO_IP, IP_MULTICAST_IF, &iaddr, sizeof(iaddr));

        addr_.sin_family = AF_INET;
        addr_.sin_addr.s_addr = inet_addr(group_.c_str());
        addr_.sin_port = htons(port_);

        running_ = true;
        th_ = std::thread([this]{ run(); });
        return true;
    }

    void send(const std::string& payload) {
        // non-blocking best-effort
        sendto(sock_, payload.data(), payload.size(), MSG_DONTWAIT,
               (sockaddr*)&addr_, sizeof(addr_));
    }

    void stop() {
        running_ = false;
        if (th_.joinable()) th_.join();
        if (sock_ >= 0) close(sock_);
        sock_ = -1;
    }

    ~MulticastBroadcaster(){ stop(); }

private:
    void run() {
        while (running_) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

    std::string group_;
    int port_;
    std::string iface_;
    int sock_{-1};
    sockaddr_in addr_{};
    std::atomic<bool> running_{false};
    std::thread th_;
};
