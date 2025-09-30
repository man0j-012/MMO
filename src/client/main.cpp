#include "../common/util.hpp"
#include <boost/asio.hpp>
#include <iostream>
#include <thread>
using boost::asio::ip::tcp;

int main(int argc, char** argv){
    std::string host="127.0.0.1"; int port=4000; int seconds=10; int pps=5;
    for (int i=1;i<argc;i++){
        std::string a=argv[i];
        auto nexts=[&](std::string& v){ if(i+1<argc) v=argv[++i]; };
        auto next=[&](int& v){ if(i+1<argc) v=std::stoi(argv[++i]); };
        if (a=="--host") nexts(host);
        else if (a=="--port") next(port);
        else if (a=="--seconds") next(seconds);
        else if (a=="--pps") next(pps);
    }
    try {
        boost::asio::io_context io;
        tcp::socket sock(io);
        tcp::resolver r(io);
        auto res = r.resolve(host, std::to_string(port));
        boost::asio::connect(sock, res);
        std::string line;
        auto start = std::chrono::steady_clock::now();
        while (std::chrono::steady_clock::now() - start < std::chrono::seconds(seconds)) {
            line = "PING\n";
            boost::asio::write(sock, boost::asio::buffer(line));
            char buf[16]; size_t n = sock.read_some(boost::asio::buffer(buf, sizeof(buf)));
            std::this_thread::sleep_for(std::chrono::milliseconds(1000/std::max(1,pps)));
        }
    } catch (const std::exception& e){
        std::cerr<<"client error: "<<e.what()<<std::endl;
        return 1;
    }
    return 0;
}
