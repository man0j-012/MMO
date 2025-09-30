#include "server.hpp"
#include "../common/util.hpp"
#include <iostream>

int main(int argc, char** argv) {
    int tcp = 4000, mport = 30001;
    std::string mgroup = "239.255.0.1", iface = "0.0.0.0";
    for (int i=1;i<argc;i++){
        std::string a = argv[i];
        auto next = [&](int& v){ if (i+1<argc) v = std::stoi(argv[++i]); };
        auto nexts = [&](std::string& v){ if (i+1<argc) v = argv[++i]; };
        if (a=="--tcp") next(tcp);
        else if (a=="--mgroup") nexts(mgroup);
        else if (a=="--mport") next(mport);
        else if (a=="--iface") nexts(iface);
    }
    try {
        ServerApp app(tcp, mgroup, mport, iface);
        app.run();
    } catch (const std::exception& e) {
        std::cerr << "server error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
