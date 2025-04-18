#include "Proxy.h"
#include "GameObject.h"
#include "Food.h"
#include "Player.h"
#include "Barrier.h"
#include "GameItems.h"

std::queue<ReceivedInfo> Proxy::receivedQueue;
std::queue<SendInfo> Proxy::sendQueue;

int main() {
    asio::io_context io;
    Proxy p{io,8080};

}