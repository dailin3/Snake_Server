#include "Proxy.h"

std::queue<ReceivedInfo> Proxy::infoQueue;

int main() {
    asio::io_context io;
    Proxy p{io,8080};
    p.begin();
}