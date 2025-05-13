//
// Created by dailin on 25-4-1.
//

#ifndef ROOMKEEPER_H
#define ROOMKEEPER_H
#include <vector>
#include <iostream>

#include "Player.h"
#include "GameThread.h"

using json = nlohmann::json;


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

    void runningLoop();

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


    void _infoHandler(ReceivedInfo& info);
};



#endif //ROOMKEEPER_H
