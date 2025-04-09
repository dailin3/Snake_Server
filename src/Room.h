//
// Created by dailin on 25-4-1.
//

#ifndef ROOM_H
#define ROOM_H
#include "GameThread.h"
#include "Player.h"

enum class RoomState {
    Playing,
    NotPlaying,
    Other,
};

class Room {
public:
    Room();
    ~Room();
private:
    int roomId;
    RoomState state;
    Player* players[];
    GameThread thread;
};



#endif //ROOM_H
