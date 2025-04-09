#include "Proxy.h"

std::queue<ReceivedInfo> Proxy::receivedQueue;
std::queue<SendInfo> Proxy::sendQueue;

int main() {
    asio::io_context io;
    Proxy p{io,8080};
    p.begin();
}