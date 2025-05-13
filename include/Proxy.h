//
// Created by dailin on 25-4-1.
//

#ifndef SNAKE_SERVER_PROXY_H
#define SNAKE_SERVER_PROXY_H

#include <iostream>
#include <queue>
#include <unordered_map>
#include <thread>
#include <memory>
#include <utility>

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/beast/websocket.hpp>
#include <nlohmann/json.hpp>

#include "Info.h"

namespace asio = boost::asio;
namespace beast = boost::beast;
namespace websocket = beast::websocket;

using json = nlohmann::json;
using WebSocketPtr = std::shared_ptr<websocket::stream<asio::ip::tcp::socket>>;

class Proxy {
public:
    static std::queue<ReceivedInfo> receivedQueue;
    static std::queue<SendInfo> sendQueue;

    explicit Proxy(asio::io_context& io_context, unsigned short port);
    ~Proxy();

    void start();
    void stop();

private:
    bool isRunning = false;
    asio::io_context& io_context;
    std::unique_ptr<asio::ip::tcp::acceptor> acceptor;
    const unsigned short port;
    std::thread acceptThread;
    std::thread sendThread;
    std::unordered_map<int, WebSocketPtr> connectionsMap;

    static int maxConnectionId;

    int registerConnection(WebSocketPtr wsptr);
    WebSocketPtr getConnection(int id);
    void removeConnection(int id);
    void _handleInfo(const json& jsonInfo, int connectionId);
    void _websocket_session(asio::ip::tcp::socket socket);
    void acceptfunction();
    void sendfunction();
    void send(SendInfo& info);
};

#endif // SNAKE_SERVER_PROXY_H
