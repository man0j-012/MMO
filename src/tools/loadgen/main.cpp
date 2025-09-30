#include "../../common/util.hpp"
#include <boost/asio.hpp>
#include <iostream>
#include <vector>
#include <thread>

using boost::asio::ip::tcp;

struct Conn {
    std::unique_ptr<tcp::socket> sock;
};

int main(int argc, char** argv){
    std::string host="127.0.0.1"; int port=4000;
    int clients=1000; int rate=10; // pings per second per client
    for (int i=1;i<argc;i++){
        std::string a=argv[i];
        auto nexts=[&](std::string& v){ if(i+1<argc) v=argv[++i]; };
        auto next=[&](int& v){ if(i+1<argc) v=std::stoi(argv[++i]); };
        if (a=="--host") nexts(host);
        else if (a=="--port") next(port);
        else if (a=="--clients") next(clients);
        else if (a=="--rate") next(rate);
    }

    boost::asio::io_context io;
    tcp::resolver r(io);
    auto endpoints = r.resolve(host, std::to_string(port));

    std::vector<std::unique_ptr<tcp::socket>> socks;
    socks.reserve(clients);

    std::cout << "Connecting " << clients << " clients...\n";
    for (int i=0;i<clients;i++){
        auto s = std::make_unique<tcp::socket>(io);
        boost::asio::connect(*s, endpoints);
        socks.emplace_back(std::move(s));
        if ((i+1)%100==0) std::cout << (i+1) << " connected\n";
    }
    std::cout << "All connected.\n";

    auto worker = [&socks, rate](){
        std::string msg = "PING\n";
        char buf[16];
        while (true){
            for (auto& s : socks){
                boost::system::error_code ec;
                boost::asio::write(*s, boost::asio::buffer(msg), ec);
                s->read_some(boost::asio::buffer(buf, sizeof(buf)), ec);
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(1000/std::max(1,rate)));
        }
    };
    std::thread t(worker);
    io.run();
    t.join();
    return 0;
}
