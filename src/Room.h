//
// Created by dailin on 25-4-1.
//

#ifndef ROOM_H
#define ROOM_H
#include "GameItems.h"
#include "GameThread.h"
#include "Player.h"

enum class RoomState {
    Playing,
    NotPlaying,
    Other,
};

class Room {
public:
    [[nodiscard]] int getRoomId() const {
        return roomId;
    }

    [[nodiscard]] RoomState getState() const {
        return state;
    }

    [[nodiscard]] GameThread getGameThread() const {
        return thread;
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

    Room();
    ~Room();

private:
    int roomId;
    RoomState state;
    std::vector<Player*> players;
    GameThread thread;
    // TODO: create a new operation instance queue which replaces playerID with player pointer.
    std::queue<ReceivedInfo> operationsQueue;
};



#endif //ROOM_H
