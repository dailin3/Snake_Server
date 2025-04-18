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
            if (!infoQueue.empty()) {
                auto info = infoQueue.front();
                _infoHandler(info);
                infoQueue.pop();
            }
        }
    }

    void createRoom();
    void removeRoom();
    void createPlayer();
    void removePlayer();
    void createGameThread(Room* room);
    void removeGameThread();
    void allocatePlayerToRoom(Player* player, Room* room);
    void removePlayerFromRoom(Player* player, Room* room);

private:
    std::vector<Room*> rooms;
    std::vector<GameThread*> threads;
    std::vector<Player*> players;
    std::queue<ReceivedInfo> infoQueue;


    void _infoHandler(ReceivedInfo& info) {
        // Todo: add info dispatch logic.
        return;
    }
};



#endif //ROOMKEEPER_H
