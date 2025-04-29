//
// Created by dailin on 25-4-8.
//

#include "GameThread.h"

GameThread::GameThread(Room *room): room(room) {
}

Room * GameThread::getRoom() const{
    return room;
}

void GameThread::initGame() {
    // init map and gameItems
}

void GameThread::gameLoop() {
}
