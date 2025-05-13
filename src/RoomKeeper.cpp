//
// Created by dailin on 25-4-1.
//

#include "RoomKeeper.h"

RoomKeeper::RoomKeeper() {
}

RoomKeeper::~RoomKeeper() {
}

void RoomKeeper::runningLoop() {
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

int RoomKeeper::createRoom() {
    auto room = new Room();
    rooms.push_back(room);
    return room->getId();
}

int RoomKeeper::removeRoom(int id) {
    for (auto it = rooms.begin(); it != rooms.end(); ++it) {
        if ((*it)->getId() == id && (*it)->getPlayers().empty()) {
            delete *it;
            removeGameThread((*it));
            rooms.erase(it);
            return id;
        }
    }
    return -1;
}

int RoomKeeper::createPlayer(const std::string& name, int ws) {
    auto player = new Player(name, ws);
    players.push_back(player);
    return player->getId();
}

int RoomKeeper::removePlayer(int id) {
    for (auto it = players.begin(); it != players.end(); ++it) {
        if ((*it)->getId() == id && (*it)->getState() == PlayerState::Dissociated) {
            delete *it;
            players.erase(it);
            return id;
        }
    }
    return -1;
}

int RoomKeeper::createGameThread(Room* room) {
    auto thread = new GameThread(room);
    threads.push_back(thread);
    room->setGameThread(thread);
    return room->getId();
}

int RoomKeeper::removeGameThread(Room* room) {
    for (auto it = threads.begin(); it != threads.end(); ++it) {
        if ((*it)->getRoom() == room) {
            room->setGameThread(nullptr);
            delete *it;
            threads.erase(it);
            return room->getId();
        }
    }
    return -1;
}

int RoomKeeper::bind(Player *player, Room *room) {
    if (room != nullptr && player != nullptr && player->getState() == PlayerState::Dissociated) {
        player->joinRoom(room);
        room->addPlayer(player);
        return 1;
    }
    return -1;
}

int RoomKeeper::bind(Room *room, GameThread *thread) {
    // TODO
    return -1;
}

int RoomKeeper::unbind(Player *player, Room *room) {
    if (room != nullptr && player != nullptr && player->getState() == PlayerState::InRoom) {
        player->setState(PlayerState::Dissociated);
        player->setRoom(nullptr);
        room->removePlayer(player);
        return 1;
    }
    return -1;
}

int RoomKeeper::unbind(Room *room, GameThread *thread) {
    // TODO
    return -1;
}

int RoomKeeper::startGame(Room* room, Player* player) {
    if (room->getGameThread() == nullptr) {
        createGameThread(room);
    }
    return room->startGame(player);
}

int RoomKeeper::stopGame(Room *room, Player* player) {
    return room->stopGame(player);
}

int RoomKeeper::readyForGame( Player *player) {
    if (player != nullptr) {
        return player->ready();
    }else {
        return -1;
    }
}

int RoomKeeper::unreadyForGame( Player *player) {
    return player->unready();
}

Player * RoomKeeper::getPlayerById(int id) {
    for (size_t i = 0; i < players.size(); i++) {
        if (players[i]->getId() == id) {
            return players[i];
        }
    }
    return nullptr;
}

Room * RoomKeeper::getRoomById(int id) {
    for (size_t i = 0; i < rooms.size(); i++) {
        if (rooms[i]->getId() == id) {
            return rooms[i];
        }
    }
    return nullptr;
}

std::vector<json> RoomKeeper::getPlayersJson() const {
    std::vector<json> playersJson;
    playersJson.reserve(players.size());
    for (Player* player : players) {
        playersJson.push_back(player->getPlayerJson());
    }
    return playersJson;
}

std::vector<json> RoomKeeper::getRoomsJson() const {
    std::vector<json> roomsJson;
    roomsJson.reserve(rooms.size());
    for (Room* room : rooms) {
        roomsJson.push_back(room->getRoomJson());
    }
    return roomsJson;
}

json RoomKeeper::getRoomKeeperJson() const {
    json roomKeeperJson;
    roomKeeperJson["isRunning"] = isRunning;
    roomKeeperJson["players"] = getPlayersJson();
    roomKeeperJson["rooms"] = getRoomsJson();
    return roomKeeperJson;
}

void RoomKeeper::_infoHandler(ReceivedInfo& info) {
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
