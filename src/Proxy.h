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
#include <utility>
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

    Proxy(asio::io_context& _io_context, const unsigned short _port) : acceptor(nullptr), io_context(_io_context), port(_port) {}

    void start() {
        if (!isRunning) {
            isRunning = true;
            acceptor = std::make_unique<asio::ip::tcp::acceptor>(io_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port));
            acceptThread = std::thread(&Proxy::acceptfunction, this);
            sendThread = std::thread(&Proxy::sendfunction, this);
            std::cout << "Proxy started at port: "<< port << std::endl;
        }
    }

    void stop() {
        if (isRunning) {
            isRunning = false;
            acceptor->close();
            acceptThread.join();
            sendThread.join();
            std::cout << "Proxy stopped!" << std::endl;
        }
    }

private:
    bool isRunning = false;
    asio::io_context& io_context;
    std::unique_ptr<asio::ip::tcp::acceptor> acceptor;
    const unsigned short port;
    std::thread acceptThread;
    std::thread sendThread;

    static int maxConnectionId;
    std::unordered_map<int, WebSocketPtr> connectionsMap;

    int registerConnection(WebSocketPtr wsptr) {
        maxConnectionId++;
        connectionsMap[maxConnectionId] = wsptr;
        return maxConnectionId;
    };

    WebSocketPtr getConnnection(int id) {
        try {
            return connectionsMap[id];
        }catch (std::out_of_range& e) {
            return nullptr;
        }
    }

    void removeConnection(int id) {
        auto it = connectionsMap.find(id);
        if (it != connectionsMap.end()) {
            auto ws = it->second;
            if (ws && ws->is_open()) {  // 检查连接是否仍然打开
                boost::system::error_code ec;
                ws->close(websocket::close_code::normal, ec);
                if (ec && ec != boost::asio::error::operation_aborted) {
                    std::cerr << "Error closing connection: " << ec.message() << std::endl;
                }
            }
            connectionsMap.erase(it);
        }
    }


    void _handleInfo(const json& jsonInfo, int connectionId) {
        bool isGoodInfo = true; // TODO: think about whether the json is in right form and complete it.
        if (isGoodInfo) {
            const ReceivedInfo info{jsonInfo, connectionId};
            receivedQueue.push(info);
        }
    }

    void _websocket_session(asio::ip::tcp::socket socket) {
        auto ws = std::make_shared<websocket::stream<asio::ip::tcp::socket>>(std::move(socket));
        int connectionId = registerConnection(ws);
        ws->accept();
        std::cout << "Client connected!\n";

        beast::flat_buffer buffer;
        while (true) {
            try {
                ws->read(buffer);
                std::string msg = beast::buffers_to_string(buffer.data());

                // TODO: all judge add into isGoodInfo()
                // parse the json result and give the feedback.
                try {
                    json j = json::parse(msg);
                    std::cout << "WS_Session Received:" <<j.dump() << std::endl;
                    _handleInfo(j, connectionId);
                }
                catch (const json::parse_error& e) {
                    std::cerr << "JSON parse error: " << e.what() << std::endl;
                    std::cerr << "Invalid JSON content: " << msg << std::endl;

                    // if the client use unstringified object
                    if (msg == "[object Object]") {
                        SendInfo err_info {connectionId,Responsecode::failed, "Client sent unstringified object"};
                        send(err_info);
                    }
                }

            }catch (const beast::system_error& e) {
                if (e.code() == websocket::error::closed || e.code () == asio::error::operation_aborted || e.code() == boost::asio::error::eof) {
                    std::cout << "Client disconnect!" << std::endl;
                    removeConnection(connectionId);
                    break;
                }else {
                    std::cerr << "websocket error: " <<e.what() << std::endl;
                    break;
                }
            }
            buffer.consume(buffer.size());
        }
    }

    void acceptfunction() {
        while (isRunning) {
            try {
                asio::ip::tcp::socket socket(io_context);
                acceptor->accept(socket);
                std::thread(&Proxy::_websocket_session, this, std::move(socket)).detach();
            } catch (const std::exception& e) {
                if (!isRunning) break; // 正常退出
                std::cerr << "accept error: " << e.what() << std::endl;
            }
        }
    }

    void sendfunction() {
        while (isRunning) {
            // clear the send message list
            if (!sendQueue.empty()) {
                SendInfo sendInfo = sendQueue.front();
                send(sendInfo);
                sendQueue.pop();
            }
        }
    }

    void send(SendInfo& info) {
        auto objIdlst = info.getObjectIdList();
        for (auto objId : objIdlst) {
            try {
                auto obj = getConnnection(objId);
                if (obj != nullptr) {
                    obj -> write(asio::buffer(info.getJson().dump()));
                    std::cout << "Send Info:" << info.getJson().dump() << std::endl;
                }
            }catch (const beast::system_error& e) {
                if (e.code() == websocket::error::closed || e.code () == asio::error::operation_aborted || e.code() == boost::asio::error::eof) {
                    std::cout << "Client disconnect!" << std::endl;
                    removeConnection(objId);
                    continue;
                }else {
                    std::cerr << "websocket error: " <<e.what() << std::endl;
                    break;
                }
            }
        }
    }
};


#endif //PROXY_H
