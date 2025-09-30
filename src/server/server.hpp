#pragma once
#include "session.hpp"
#include "metrics.hpp"
#include "world.hpp"
#include "multicast.hpp"
#include "../common/message.hpp"
#include <boost/asio.hpp>
#include <thread>
#include <atomic>

class ServerApp {
public:
    ServerApp(int tcp_port, const std::string& mgroup, int mport, const std::string& iface)
        : ioc_(), acceptor_(ioc_), tcp_port_(tcp_port),
          metrics_(), world_(metrics_), mcast_(mgroup, mport, iface) {}

    void run() {
        using boost::asio::ip::tcp;
        tcp::endpoint ep(tcp::v4(), tcp_port_);
        acceptor_.open(ep.protocol());
        acceptor_.set_option(tcp::acceptor::reuse_address(true));
        acceptor_.bind(ep);
        acceptor_.listen();
        do_accept();

        world_.set_tick_hz(20);
        world_.start();
        mcast_.start();

        // metrics broadcaster
        metrics_thread_ = std::thread([this]{
            while (running_) {
                auto js = make_json_metrics(metrics_.connected.load(), metrics_.events.exchange(0), 20);
                mcast_.send(js);
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
        });

        ioc_.run();
    }

    void stop() {
        running_ = false;
        ioc_.stop();
        world_.stop();
        mcast_.stop();
        if (metrics_thread_.joinable()) metrics_thread_.join();
    }

private:
    void do_accept() {
        acceptor_.async_accept([this](boost::system::error_code ec, boost::asio::ip::tcp::socket socket){
            if (!ec) {
                std::make_shared<Session>(std::move(socket), metrics_)->start();
            }
            do_accept();
        });
    }

    boost::asio::io_context ioc_;
    boost::asio::ip::tcp::acceptor acceptor_;
    int tcp_port_;
    Metrics metrics_;
    WorldLoop world_;
    MulticastBroadcaster mcast_;
    std::thread metrics_thread_;
    std::atomic<bool> running_{true};
};
