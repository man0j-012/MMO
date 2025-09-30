#pragma once
#include "../common/message.hpp"
#include "../common/util.hpp"
#include "metrics.hpp"
#include <boost/asio.hpp>
#include <memory>

using boost::asio::ip::tcp;

class Session : public std::enable_shared_from_this<Session> {
public:
    Session(tcp::socket socket, Metrics& m) : socket_(std::move(socket)), metrics_(m) {}
    void start() { metrics_.connected.fetch_add(1); do_read(); }
    ~Session() { metrics_.connected.fetch_sub(1); }
private:
    void do_read() {
        auto self = shared_from_this();
        boost::asio::async_read_until(socket_, boost::asio::dynamic_buffer(buf_), '\n',
            [this,self](boost::system::error_code ec, std::size_t n){
                if (!ec) {
                    std::string line = buf_.substr(0, n);
                    buf_.erase(0, n);
                    if (line == "PING\n") {
                        do_write("PONG\n");
                    } else {
                        do_write("OK\n");
                    }
                    do_read();
                } else {
                    // closed
                }
            });
    }
    void do_write(const std::string& s) {
        auto self = shared_from_this();
        boost::asio::async_write(socket_, boost::asio::buffer(s),
            [this,self](boost::system::error_code, std::size_t){ /* ignore */ });
    }
    tcp::socket socket_;
    std::string buf_;
    Metrics& metrics_;
};
