//
// Created by dailin on 25-4-1.
//

#include "RoomKeeper.h"

RoomKeeper::RoomKeeper() {
}

RoomKeeper::~RoomKeeper() {
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

int RoomKeeper::createPlayer(const std::string& name, websocket::stream<asio::ip::tcp::socket> *ws) {
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
    if (room != nullptr && player != nullptr && player->getState() == PlayerState::Dissociated) {
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
    return player->ready();
}

int RoomKeeper::unreadyForGame( Player *player) {
    return player->unready();
}

Player * RoomKeeper::getPlayerById(int id) {
    for (int i = 0; i < players.size(); i++) {
        if (players[i]->getId() == id) {
            return players[i];
        }
    }
    return nullptr;
}

Room * RoomKeeper::getRoomById(int id) {
    for (int i = 0; i < rooms.size(); i++) {
        if (rooms[i]->getId() == id) {
            return rooms[i];
        }
    }
    return nullptr;
}
