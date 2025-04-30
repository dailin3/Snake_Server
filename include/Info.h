//
// Created by dailin on 25-4-2.
//

#ifndef INFO_H
#define INFO_H

#include <boost/asio.hpp>
#include <boost/beast/websocket.hpp>
#include <nlohmann/json.hpp>
#include <utility>

#include "../src/GameObject.h"

namespace asio = boost::asio;
namespace websocket = boost::beast::websocket;

using json = nlohmann::json;

enum class InfoType {
    GameOperation = 1,          // to do game operation
    RoomOperation = 2,          // to start game or join room and so on
};

enum class RoomOperationType {
    createPlayer = 1,    // 创建玩家
    removePlayer = 2,    // 移除玩家
    createRoom = 3,      // 创建房间
    removeRoom = 4,      // 移除房间
    joinRoom = 5,        // 加入房间
    leaveRoom = 6,       // 离开房间
    readyRoom = 7,       // 准备
    unreadyRoom = 8,     // 取消准备
    startGame = 9,       // 开始游戏
    roomInfo = 10,       // 房间信息
    roomKeeperInfo = 11, // 管理信息
    playerInfo = 12,     // 玩家信息
};

enum class GameOperationType {
    changeDirection = 1, // 改变方向
    born = 2,            // 出生
    getGameInfo = 3,     // 获取游戏信息
};

class OperationPayload{};

class RoomOperationPayload : public OperationPayload {
public:
    static RoomOperationPayload fromJson(const json& j) {
        RoomOperationPayload payload{};
        payload.type = static_cast<RoomOperationType>(j.at("type").get<int>());

        if (payload.type == RoomOperationType::joinRoom) payload.roomId = j["data"]["roomId"].get<int>();

        return payload;
    }
    RoomOperationType type;
    int roomId {-1};
};

class GameOperationPayload : public OperationPayload {
public:
    static GameOperationPayload fromJson(const json& j) {
        GameOperationPayload payload{};
        payload.type = static_cast<GameOperationType>(j["type"].get<int>());

        if (payload.type == GameOperationType::changeDirection) payload.newDirection = static_cast<Direction>(j["data"]["newDirection"].get<int>());

        return payload;
    }
    GameOperationType type;
    Direction newDirection;
};

using AnyPayload = std::variant<RoomOperationPayload, GameOperationPayload>;

class ReceivedInfo {
public:
    [[nodiscard]] InfoType getType() const {
        return type;
    }

    [[nodiscard]] int getRoomId() const {
        return roomId;
    }

    [[nodiscard]] json getRawPayload() const {
        return rawPayload;
    }

    [[nodiscard]] GameOperationPayload getGamePayload() const{
        return gamePayload;
    };

    [[nodiscard]] RoomOperationPayload getRoomPayload() const {
        return roomPayload;
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
        rawPayload = j["payload"];
        if (type == InfoType::GameOperation) {
            gamePayload = GameOperationPayload::fromJson(rawPayload);
        }else if (type == InfoType::RoomOperation) {
            roomPayload = RoomOperationPayload::fromJson(rawPayload);
        }
    }

private:
    InfoType type;
    int roomId;
    int playerId;
    json rawPayload;
    GameOperationPayload gamePayload = {};
    RoomOperationPayload roomPayload = {};
    const websocket::stream<asio::ip::tcp::socket>* ws;
};

enum class Responsecode {
    success = 0,
    failed = 1,
};

class SendInfo {
public:
    SendInfo(std::vector<websocket::stream<asio::ip::tcp::socket>*> _object_list, json _payload, Responsecode _responsecode = Responsecode::success, std::string _msg = "")
    : object_list(std::move(_object_list)), payload(std::move(_payload)), code(_responsecode), msg(std::move(_msg)) {

    }

    [[nodiscard]] std::vector<websocket::stream<asio::ip::tcp::socket>*> getObjectList() const {
        return object_list;
    }
    [[nodiscard]] json getPayload() const {
        return payload;
    }
private:
    std::vector<websocket::stream<asio::ip::tcp::socket>*> object_list;
    json payload;
    Responsecode code;
    std::string msg;
};

#endif //INFO_H
