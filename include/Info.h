//
// Created by dailin on 25-4-2.
//

#ifndef INFO_H
#define INFO_H

#include <boost/asio.hpp>
#include <boost/beast/websocket.hpp>
#include <nlohmann/json.hpp>

namespace asio = boost::asio;
namespace websocket = boost::beast::websocket;

using json = nlohmann::json;

enum class InfoType {
    GameOperation = 1,          // to do game operation
    RoomOperation = 2,          // to start game or join room and so on
    RoomKeeperOperation = 3,    // to create room or delete room and so on
};

class ReceivedInfo {
public:
    [[nodiscard]] InfoType getType() const {
        return type;
    }

    [[nodiscard]] int getRoomId() const {
        return roomId;
    }

    [[nodiscard]] json getPayload() const {
        return payload;
    }

    [[nodiscard]] json getPlayerId() const {
        return playerId;
    }

    [[nodiscard]] const websocket::stream<asio::ip::tcp::socket>* getWS() {
        return ws;
    }

    ReceivedInfo(const json& j, const websocket::stream<asio::ip::tcp::socket>* _ws) : ws(_ws) {
        type = static_cast<InfoType>(j["type"].get<int>());
        roomId = j.value("roomId", -1);
        playerId = j.value("playerId", -1);
        payload = j["payload"];
    }

private:
    InfoType type;
    int roomId;
    int playerId;
    json payload;
    const websocket::stream<asio::ip::tcp::socket>* ws;
};

#endif //INFO_H
