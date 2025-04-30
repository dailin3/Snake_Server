//
// Created by dailin on 25-4-1.
//

#ifndef ROOMKEEPER_H
#define ROOMKEEPER_H
#include <vector>

#include "Player.h"
#include "GameThread.h"


class RoomKeeper {
public:
    RoomKeeper();
    ~RoomKeeper();

    void runningLoop() {
        while (true) {
            // dispatch info
            if (!Proxy::receivedQueue.empty()) {
                auto info = Proxy::receivedQueue.front();
                _infoHandler(info);
                Proxy::receivedQueue.pop();
            }

            // clear empty room
            for (auto &room : rooms) {
                if (room->getPlayers().empty()) {
                    removeRoom(room->getId());
                }
            }
        }
    }

    int createRoom();
    int removeRoom(int id);
    int createPlayer(const std::string& name, websocket::stream<asio::ip::tcp::socket> *ws);
    int removePlayer(int id);
    int createGameThread(Room* room);
    int removeGameThread(Room* room);
    int bind(Player* player, Room* room);
    int bind(Room* room, GameThread* thread);
    int unbind(Player* player, Room* room);
    int unbind(Room* room, GameThread* thread);
    int startGame(Room* room, Player* player);
    int stopGame(Room* room, Player* player);
    int readyForGame( Player* player);
    int unreadyForGame( Player* player);

    Player* getPlayerById(int id);
    Room* getRoomById(int id);

private:
    std::vector<Room*> rooms;
    std::vector<GameThread*> threads;
    std::vector<Player*> players;


    void _infoHandler(ReceivedInfo& info) {
        std::cout << "RoomKeeper::_infoHandler Recieved:"<< info.getRawPayload().dump() <<std::endl;
        // dispatch game logic.
        if (info.getType() == InfoType::GameOperation) {
            auto roomId = info.getRoomId();
            auto room = getRoomById(roomId);
            if (room != nullptr) {
                room->pushOperations(info);
            }else {
                std::cout<< "Cannot find room with id " << roomId << std::endl;
            }
        }else if (info.getType() == InfoType::RoomOperation) {
            auto roomId = info.getRoomId();
            auto playerId = info.getPlayerId();
            auto payload = info.getRoomPayload();
            auto type = payload.type;
            auto ws = info.getWS();

            // TODO: feedback
            if (type == RoomOperationType::createPlayer) {
                int id = createPlayer(payload.name,ws);
                std::cout << id << std::endl;

            }else if (type == RoomOperationType::removePlayer) {
                int code = removePlayer(playerId);
                std::cout << code << std::endl;

            }else if (type == RoomOperationType::joinRoom) {
                auto room = getRoomById(roomId);
                auto player = getPlayerById(playerId);
                int code = bind(player, room);
                std::cout << code << std::endl;

            }else if (type == RoomOperationType::leaveRoom) {
                auto room = getRoomById(roomId);
                auto player = getPlayerById(playerId);
                int code = unbind(player, room);
                std::cout << code << std::endl;

            }else if (type == RoomOperationType::readyRoom) {
                auto player = getPlayerById(playerId);
                auto code = readyForGame(player);
                std::cout << code << std::endl;

            }else if (type == RoomOperationType::unreadyRoom) {
                auto player = getPlayerById(playerId);
                auto code = unreadyForGame(player);
                std::cout << code << std::endl;

            }else if (type == RoomOperationType::createRoom) {
                auto id = createRoom();
                auto room = getRoomById(id);
                auto player = getPlayerById(playerId);
                bind(player, room);
                std::cout << id << std::endl;
            }
            else if (type == RoomOperationType::startGame) {
                auto player = getPlayerById(playerId);
                auto room = getRoomById(roomId);
                if (room != nullptr) {
                    auto code = startGame(room, player);
                    std::cout << code << std::endl;
                }

            }else if (type == RoomOperationType::roomInfo) {

            }else if (type == RoomOperationType::roomKeeperInfo) {

            }else if (type == RoomOperationType::playerInfo) {

            }else {
                std::cout<< "RoomKeeper error unkown type : " <<static_cast<int>(type) << std::endl;
            }
        }
    }
};



#endif //ROOMKEEPER_H
