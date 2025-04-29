//
// Created by dailin on 25-4-1.
//

#ifndef ROOM_H
#define ROOM_H

#include <iostream>

#include "GameItems.h"
#include "Info.h"
#include "Map.h"
class GameThread;
class Player;

enum class RoomState {
    Readying,
    InitGaming,
    Playing,
};

class Room {
public:
    [[nodiscard]] int getRoomId() const {
        return roomId;
    }

    [[nodiscard]] RoomState getState() const {
        return state;
    }

    [[nodiscard]] GameThread* getGameThread() const {
        return gameThread;
    }

    Map getMap() const {
        return map;
    }

    void setGameThread(GameThread* gameThread) {
        this->gameThread = gameThread;
    }

    std::vector<ReceivedInfo> getOperationsList() {
        std::vector<ReceivedInfo> operationsList;
        while (!operationsQueue.empty()) {
            operationsList.push_back(operationsQueue.front());
            this->operationsQueue.pop();
        }
        return operationsList;
    }

    void pushOperations(const ReceivedInfo& operation) {
        this->operationsQueue.push(operation);
    }

    void addPlayer(Player* player) {
        this -> players.push_back(player);
    }

    void removePlayer(Player* player) {
        players.erase(std::ranges::remove(this->players, player).begin(), this->players.end());
    }

    void initMap(int width = 25, int height =25) {
        this->map = Map(width, height);
    }

    void initGameItems() {
        this->gameItems = GameItems(&map);
    }

    void initWall() {
        this->gameItems.addBarrier(map.getEdges());
        this->gameItems.update();
    }

    void initSnake() {
        for (const auto & player : this->players) {
            this->gameItems.addSnake(player);
        }
        this->gameItems.update();
    }

    Room():  map{25,25},gameItems{&map} {
        roomId = maxRoomId++;
        state = RoomState::Readying;
    };

private:
    static int maxRoomId;
    int roomId;
    RoomState state;
    std::vector<Player*> players = std::vector<Player*>();
    GameThread* gameThread = nullptr;
    // TODO: create a new operation instance queue which replaces playerID with player pointer.
    std::queue<ReceivedInfo> operationsQueue;
    GameItems gameItems;
    Map map;
};



#endif //ROOM_H
