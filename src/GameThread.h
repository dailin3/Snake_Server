//
// Created by dailin on 25-4-8.
//

#ifndef GAMETHREAD_H
#define GAMETHREAD_H
#include <thread>

#include "Room.h"


class GameThread {
private:
    std::thread thread;
    Room* room;
};



#endif //GAMETHREAD_H
