//
// Created by dailin on 25-4-1.
//

// Room.cpp
#include "Room.h"
#include "Player.h"

int Room::maxRoomId = 0;

Room::Room() : map{25, 25}, gameItems{&map} {
    roomId = maxRoomId++;
    state = RoomState::Readying;
}

int Room::getId() const {
    return roomId;
}

RoomState Room::getState() const {
    return state;
}

GameThread* Room::getGameThread() const {
    return gameThread;
}

GameItems Room::getGameItems() const {
    return gameItems;
}

Map Room::getMap() const {
    return map;
}

std::vector<Player*> Room::getPlayers() const {
    return players;
}

Player* Room::getPlayerById(int id) {
    for (Player* player : players) {
        if (player->getId() == id) {
            return player;
        }
    }
    return nullptr;
}


int Room::getGameAllFrames() const {
    return gameAllFrames;
}

int Room::getFreshMiliSeconds() const {
    return freshMiliSeconds;
}

void Room::setGameThread(GameThread* gameThread) {
    this->gameThread = gameThread;
}

void Room::setRoomState(RoomState roomState) {
    this->state = roomState;
}

void Room::addPlayer(Player* player) {
    players.push_back(player);
}

void Room::removePlayer(Player* player) {
    players.erase(std::ranges::remove(players, player).begin(), players.end());
}

void Room::pushOperations(const ReceivedInfo& operation) {
    operationsQueue.push(operation);
}

std::vector<ReceivedInfo> Room::getOperationsList() {
    std::vector<ReceivedInfo> operationsList;
    while (!operationsQueue.empty()) {
        operationsList.push_back(operationsQueue.front());
        operationsQueue.pop();
    }
    return operationsList;
}

void Room::initMap(int width, int height) {
    map = Map(width, height);
}

void Room::initGameItems() {
    gameItems = GameItems(&map);
}

void Room::initWall() {
    gameItems.addBarrier(map.getEdges());
    gameItems.update();
}

void Room::initSnake() {
    for (Player* player : players) {
        gameItems.addSnake(player);
    }
    gameItems.update();
}

int Room::startGame(Player* player) {
    if (!players.empty() && players.front() == player && gameThread != nullptr) {

        for (Player* player : players) {
            if (!(player->getState() == PlayerState::Ready)) return -1;
        }

        setRoomState(RoomState::Playing);
        gameThread->start();
        for (Player* p : players) {
            p->joinGame();
        }
        return 1;
    }
    return -1;
}

int Room::stopGame(Player* player) {
    if (!players.empty() && players.front() == player && state == RoomState::Playing) {
        overGame();
        return 1;
    }
    return -1;
}

void Room::overGame() {
    if (state == RoomState::Playing) {
        setRoomState(RoomState::Readying);
        gameThread->join();
        for (Player* player : players) {
            player->overGame();
        }
    }
}

json Room::getGameJson() const {
    json gameJson{};
    gameJson["frame"] = frame;
    gameJson["roomId"] = roomId;
    gameJson["state"] = state;
    gameJson["players"] = getPlayersJson();
    gameJson["map"] = map.getMapJson();
    gameJson["gameItems"]= gameItems.getGameItemsJson();
    return gameJson;
}

std::vector<json> Room::getPlayersJson() const {
    std::vector<json> playersJson;
    playersJson.reserve(players.size());
    for (Player* player : players) {
        playersJson.push_back(player->getPlayerJson());
    }
    return playersJson;
}

json Room::getRoomJson() const {
    json roomJson{};
    roomJson["id"] = roomId;
    roomJson["state"] = state;
    roomJson["players"] = getPlayersJson();
    return roomJson;
}
