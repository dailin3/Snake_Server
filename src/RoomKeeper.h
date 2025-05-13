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

    // 启动线程
    void start() {
        if (isRunning==false) {
            isRunning = true;
            thread = std::thread(&RoomKeeper::runningLoop, this);
            std::cout<<"Starting RoomKeeper"<<std::endl;
        }

    }

    // 停止线程
    void stop() {
        if (isRunning==true) {
            isRunning = false;
            if (thread.joinable()) {
                thread.join();
            }
            std::cout<<"Stopping RoomKeeper"<<std::endl;
        }
    }

    void runningLoop() {
        while (isRunning) {
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
    int createPlayer(const std::string& name, int ws);
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

    [[nodiscard]] std::vector<json> getPlayersJson() const;
    [[nodiscard]] std::vector<json> getRoomsJson() const;
    [[nodiscard]] json getRoomKeeperJson() const;

private:
    std::vector<Room*> rooms;
    std::vector<GameThread*> threads;
    std::vector<Player*> players;
    bool isRunning = false;
    std::thread thread;


    void _infoHandler(ReceivedInfo& info) {
        // dispatch game logic.
        if (info.getType() == InfoType::GameOperation) {
            auto roomId = info.getRoomId();
            auto room = getRoomById(roomId);
            auto ws = info.getWSId();
            if (room != nullptr) {
                room->pushOperations(info);
            }else {
                SendInfo err_info{ws,Responsecode::failed,"Cannot find room with id " + std::to_string(roomId)};
                Proxy::sendQueue.push(err_info);
            }
            //handle room logic
        }else if (info.getType() == InfoType::RoomOperation) {
            auto roomId = info.getRoomId();
            auto playerId = info.getPlayerId();
            auto payload = info.getRoomPayload();
            auto type = payload.type;
            auto ws = info.getWSId();

            // TODO: feedback
            if (type == RoomOperationType::createPlayer) {
                int id = createPlayer(payload.name,ws);
                if (id != -1) {
                    json data {
                        {"playerId",id}
                    };
                    SendInfo res{ws,Responsecode::success,"",data};
                    Proxy::sendQueue.push(res);
                }else {
                    SendInfo err_info{ws,Responsecode::failed,"cannot create player(maybe name conflict)"};
                    Proxy::sendQueue.push(err_info);
                }

            }else if (type == RoomOperationType::removePlayer) {
                int code = removePlayer(playerId);
                if (code == 1) {
                    SendInfo res{ws};
                    Proxy::sendQueue.push(res);
                }else {
                    SendInfo err_info{ws,Responsecode::failed, "cannot remove player with id " + std::to_string(playerId)};
                    Proxy::sendQueue.push(err_info);
                }


            }else if (type == RoomOperationType::joinRoom) {
                auto room = getRoomById(roomId);
                auto player = getPlayerById(playerId);
                int code = bind(player, room);
                if (code == 1) {
                    SendInfo res{ws};
                    Proxy::sendQueue.push(res);
                }else {
                    SendInfo err_info{ws,Responsecode::failed, "cannot join room with id " + std::to_string(roomId)};
                    Proxy::sendQueue.push(err_info);
                }

            }else if (type == RoomOperationType::leaveRoom) {
                auto room = getRoomById(roomId);
                auto player = getPlayerById(playerId);
                int code = unbind(player, room);
                if (code == 1) {
                    SendInfo res{ws};
                    Proxy::sendQueue.push(res);
                }else {
                    SendInfo err_info{ws,Responsecode::failed, "cannot leave room with id " + std::to_string(roomId)};
                    Proxy::sendQueue.push(err_info);
                }

            }else if (type == RoomOperationType::readyRoom) {
                auto player = getPlayerById(playerId);
                auto code = readyForGame(player);
                if (code == 1) {
                    SendInfo res{ws};
                    Proxy::sendQueue.push(res);
                }else {
                    SendInfo err_info{ws,Responsecode::failed, "cannot ready room"};
                    Proxy::sendQueue.push(err_info);
                }

            }else if (type == RoomOperationType::unreadyRoom) {
                auto player = getPlayerById(playerId);
                auto code = unreadyForGame(player);
                if (code == 1) {
                    SendInfo res{ws};
                    Proxy::sendQueue.push(res);
                }else {
                    SendInfo err_info{ws,Responsecode::failed};
                    Proxy::sendQueue.push(err_info);
                }

            }else if (type == RoomOperationType::createRoom) {
                auto id = createRoom();
                auto room = getRoomById(id);
                auto player = getPlayerById(playerId);
                auto code = bind(player, room);
                if (code == 1) {
                    json data = {
                    {"roomId",id}
                    };
                    SendInfo res{ws,Responsecode::success,"",data};
                    Proxy::sendQueue.push(res);
                }else {
                    SendInfo err_info{ws,Responsecode::failed};
                    Proxy::sendQueue.push(err_info);
                }

            }
            else if (type == RoomOperationType::startGame) {
                auto player = getPlayerById(playerId);
                auto room = getRoomById(roomId);
                if (room != nullptr) {
                    auto code = startGame(room, player);
                    if (code == 1) {
                        SendInfo res{ws};
                        Proxy::sendQueue.push(res);
                    }else {
                        SendInfo err_info{ws,Responsecode::failed};
                        Proxy::sendQueue.push(err_info);
                    }
                }

            }else if (type == RoomOperationType::roomInfo) {
                auto room = getRoomById(payload.roomId);

                if (room != nullptr) {
                    SendInfo res{ws,Responsecode::success,"",room->getRoomJson()};
                    Proxy::sendQueue.push(res);
                }else {
                    SendInfo err_info{ws,Responsecode::failed};
                    Proxy::sendQueue.push(err_info);
                }

            }else if (type == RoomOperationType::roomKeeperInfo) {
                auto data = getRoomKeeperJson();
                SendInfo res{ws,Responsecode::success,"",data};
                Proxy::sendQueue.push(res);


            }else if (type == RoomOperationType::playerInfo) {
                auto player = getPlayerById(payload.playerId);

                if (player != nullptr) {
                    SendInfo res{ws,Responsecode::success,"",player->getPlayerJson()};
                    Proxy::sendQueue.push(res);
                }else {
                    SendInfo err_info{ws,Responsecode::failed};
                    Proxy::sendQueue.push(err_info);
                }

            }else {
                std::cout<< "RoomKeeper error unknown type : " <<static_cast<int>(type) << std::endl;
                SendInfo err_info{ws,Responsecode::failed,"Unknown type"};
                Proxy::sendQueue.push(err_info);
            }
        }
    }
};



#endif //ROOMKEEPER_H
