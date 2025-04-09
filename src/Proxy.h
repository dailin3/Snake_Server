//
// Created by dailin on 25-4-1.
//

#ifndef PROXY_H
#define PROXY_H

#include <iostream>
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/beast/websocket.hpp>
#include <nlohmann/json.hpp>
#include "Info.h"

namespace asio = boost::asio;
namespace beast = boost::beast;
namespace websocket = beast::websocket;

using json = nlohmann::json;


class Proxy {
    public:
    static std::queue<ReceivedInfo> receivedQueue;
    static std::queue<SendInfo> sendQueue;

    Proxy(asio::io_context& _io_context, const unsigned short _port) : acceptor(_io_context,{asio::ip::tcp::v4(),_port}), io_context(_io_context) {}

    [[noreturn]] void begin() {
        while (true) {
            if (isRunning) {
                // handle receive message
                asio::ip::tcp::socket socket(io_context);
                acceptor.accept(socket);
                // Confused: why have to write in such format?
                std::thread([this](asio::ip::tcp::socket s){this->_websocket_session(std::move(s));}, std::move(socket)).detach();

                // clear the send message list
                if (!sendQueue.empty()) {
                    SendInfo sendInfo = sendQueue.front();
                    send(sendInfo);
                    sendQueue.pop();
                }
                // clear the received message list
                if (!receivedQueue.empty()) {
                    ReceivedInfo receivedInfo = receivedQueue.front();
                    distributeInfo(receivedInfo);
                    receivedQueue.pop();
                }
            }
        }
    }

    void start() {isRunning = true;}
    void stop() {isRunning = false;}

    void send(SendInfo& info) {
        auto objlst = info.getObjectList();
        for (auto obj : objlst) {
            obj -> write(asio::buffer(info.getPayload().dump()));
        }
    }

    void distributeInfo(ReceivedInfo& info){}  // TODO: to distribute the received info to other queue

    private:
    bool isRunning = true;
    asio::io_context& io_context;
    asio::ip::tcp::acceptor acceptor;

    static void _handleInfo(const json& jsonInfo, const websocket::stream<asio::ip::tcp::socket>* ws) {
        bool isGoodInfo = true; // TODO: think about whether the json is in right form and complete it.
        if (isGoodInfo) {
            const ReceivedInfo info{jsonInfo, ws};
            receivedQueue.push(info);
        }
    }

    static void _websocket_session(asio::ip::tcp::socket socket) {
        websocket::stream<asio::ip::tcp::socket> ws(std::move(socket));
        ws.accept();
        std::cout << "Client connected!\n";

        beast::flat_buffer buffer;
        while (true) {
            try {
                ws.read(buffer);
                std::string msg = beast::buffers_to_string(buffer.data());

                // parse the json result and give the feedback.
                try {
                    json j = json::parse(msg);
                    std::cout << "WS_Session Received:" <<j.dump() << std::endl;
                    _handleInfo(j, &ws);
                }
                catch (const json::parse_error& e) {
                    std::cerr << "JSON parse error: " << e.what() << std::endl;
                    std::cerr << "Invalid JSON content: " << msg << std::endl;

                    // if the client use unstringified object
                    if (msg == "[object Object]") {
                        ws.write(asio::buffer(R"({"error":"Client sent unstringified object."})"));
                    }
                }
            }catch (const beast::system_error& e) {
                if (e.code() == websocket::error::closed) {
                    std::cout << "Client disconnect!" << std::endl;
                    break;
                }else {
                    std::cerr << "websocket error: " <<e.what() << std::endl;
                    break;
                }
            }
            buffer.consume(buffer.size());
        }
    }
};


#endif //PROXY_H
