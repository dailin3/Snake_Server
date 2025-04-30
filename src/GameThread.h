//
// Created by dailin on 25-4-8.
//

#ifndef GAMETHREAD_H
#define GAMETHREAD_H
#include <thread>

#include "Room.h"


// firstly we let room to gameThread one to one, then we consider many rooms to one gameThread.
class GameThread {
private:
    std::thread thread;
    Room* room;

    void handleEachOperation(ReceivedInfo& received_info);
    void snakeMove();
    void bornAndDead();
    void handleCollision();
    void sendResult();

public:
    GameThread(Room* room);
    Room* getRoom() const;

    void initGame();
    void gameEachLoop();
    void gameLoop();
    void gameEnd();
};



#endif //GAMETHREAD_H
