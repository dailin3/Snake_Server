#include "Proxy.h"
#include "RoomKeeper.h"
#include <iostream>
#include <cstdlib> // for std::atoi

int main(int argc, char* argv[]) {
    unsigned short port;

    if (argc < 2) port = 9999;
    else port = std::atoi(argv[1]);

    if (port <= 0 || port > 65535) {
        std::cerr << "Invalid port number. Please provide a port between 1 and 65535." << std::endl;
        return 1;
    }

    asio::io_context io;
    Proxy proxy{io, port};

    auto roomkeeper = RoomKeeper();
    roomkeeper.start();
    proxy.start();

    while (true);
    return 0;
}
