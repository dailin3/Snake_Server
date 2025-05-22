#include "Proxy.h"
#include "RoomKeeper.h"

int main() {
    // test Proxy
    asio::io_context io;
    Proxy proxy{io,9999};

    auto roomkeeper = RoomKeeper();
    roomkeeper.start();
    proxy.start();
    while (true);
    return 0;
}
