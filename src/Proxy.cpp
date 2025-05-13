//
// Created by dailin on 25-4-1.
//

#include "Proxy.h"

int Proxy::maxConnectionId = 0;
std::queue<ReceivedInfo> Proxy::receivedQueue;
std::queue<SendInfo> Proxy::sendQueue;


Proxy::Proxy(asio::io_context& io_context, unsigned short port)
    : io_context(io_context), port(port), acceptor(nullptr) {}

Proxy::~Proxy() {
    stop();
}

void Proxy::start() {
    if (!isRunning) {
        isRunning = true;
        acceptor = std::make_unique<asio::ip::tcp::acceptor>(
            io_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port));
        acceptThread = std::thread(&Proxy::acceptfunction, this);
        sendThread = std::thread(&Proxy::sendfunction, this);
        std::cout << "Proxy started at port: " << port << std::endl;
    }
}

void Proxy::stop() {
    if (isRunning) {
        isRunning = false;
        acceptor->close();
        acceptThread.join();
        sendThread.join();
        std::cout << "Proxy stopped!" << std::endl;
    }
}

int Proxy::registerConnection(WebSocketPtr wsptr) {
    maxConnectionId++;
    connectionsMap[maxConnectionId] = wsptr;
    return maxConnectionId;
}

WebSocketPtr Proxy::getConnection(int id) {
    try {
        return connectionsMap[id];
    } catch (std::out_of_range& e) {
        return nullptr;
    }
}

void Proxy::removeConnection(int id) {
    auto it = connectionsMap.find(id);
    if (it != connectionsMap.end()) {
        auto ws = it->second;
        if (ws && ws->is_open()) {
            boost::system::error_code ec;
            ws->close(websocket::close_code::normal, ec);
            if (ec && ec != boost::asio::error::operation_aborted) {
                std::cerr << "Error closing connection: " << ec.message() << std::endl;
            }
        }
        connectionsMap.erase(it);
    }
}

void Proxy::_handleInfo(const json& jsonInfo, int connectionId) {
    bool isGoodInfo = true; // TODO: think about whether the json is in right form and complete it.
    if (isGoodInfo) {
        const ReceivedInfo info{jsonInfo, connectionId};
        receivedQueue.push(info);
    }
}

void Proxy::_websocket_session(asio::ip::tcp::socket socket) {
    auto ws = std::make_shared<websocket::stream<asio::ip::tcp::socket>>(std::move(socket));
    int connectionId = registerConnection(ws);
    ws->accept();
    std::cout << "Client connected!\n";

    beast::flat_buffer buffer;
    while (true) {
        try {
            ws->read(buffer);
            std::string msg = beast::buffers_to_string(buffer.data());

            try {
                json j = json::parse(msg);
                std::cout << "WS_Session Received:" << j.dump() << std::endl;
                _handleInfo(j, connectionId);
            } catch (const json::parse_error& e) {
                std::cerr << "JSON parse error: " << e.what() << std::endl;
                std::cerr << "Invalid JSON content: " << msg << std::endl;

                if (msg == "[object Object]") {
                    SendInfo err_info{connectionId, Responsecode::failed, "Client sent unstringified object"};
                    send(err_info);
                }
            }
        } catch (const beast::system_error& e) {
            if (e.code() == websocket::error::closed || 
                e.code() == asio::error::operation_aborted || 
                e.code() == boost::asio::error::eof) {
                std::cout << "Client disconnect!" << std::endl;
                removeConnection(connectionId);
                break;
            } else {
                std::cerr << "websocket error: " << e.what() << std::endl;
                break;
            }
        }
        buffer.consume(buffer.size());
    }
}

void Proxy::acceptfunction() {
    while (isRunning) {
        try {
            asio::ip::tcp::socket socket(io_context);
            acceptor->accept(socket);
            std::thread(&Proxy::_websocket_session, this, std::move(socket)).detach();
        } catch (const std::exception& e) {
            if (!isRunning) break;
            std::cerr << "accept error: " << e.what() << std::endl;
        }
    }
}

void Proxy::sendfunction() {
    while (isRunning) {
        if (!sendQueue.empty()) {
            try{
                auto sendInfo = sendQueue.front();
                send(sendInfo);
                sendQueue.pop();
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
            }catch (const std::exception& e) {
                std::cerr << "send error: " << e.what() << std::endl;
            }
        }
    }
}

void Proxy::send(SendInfo& info) {
    const auto& objIdlst = info.getObjectIdList();
    for (auto objId : objIdlst) {
        try {
            auto obj = getConnection(objId);
            if (obj != nullptr) {
                obj->write(asio::buffer(info.getJson().dump()));
            }
        } catch (const beast::system_error& e) {
            if (e.code() == websocket::error::closed || 
                e.code() == asio::error::operation_aborted || 
                e.code() == boost::asio::error::eof) {
                std::cout << "Client disconnect!" << std::endl;
                removeConnection(objId);
                continue;
            } else {
                std::cerr << "websocket error: " << e.what() << std::endl;
                break;
            }
        }
    }
}
