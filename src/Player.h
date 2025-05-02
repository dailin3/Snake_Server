//
// Created by dailin on 25-4-1.
//

#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include "Proxy.h"

class Room;

enum class PlayerState {
    Playing = 1,
    Ready = 2,
    InRoom = 3,
    Dissociated = 4,
};

class Player {
public:
    static int maxId;

    Player(const std::string &name, PlayerState state, int ws);
    Player(const std::string &name, int ws);

    [[nodiscard]] std::string getName() const;
    void setName(const std::string &name);

    void setState(PlayerState state);
    void setWs(int ws);
    void setRoom(Room* room);

    void joinRoom(Room* room);
    void leaveRoom();

    int ready();
    int unready();

    void joinGame();
    void overGame();

    [[nodiscard]] int getId() const;
    [[nodiscard]] PlayerState getState() const;
    [[nodiscard]] int getWSId() const;

    [[nodiscard]] json getPlayerJson() const;

private:
    std::string name;
    int id;
    PlayerState state;
    int wsId;
    Room* room = nullptr;
};

#endif //PLAYER_H