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

    Player(const std::string &name, PlayerState state, websocket::stream<asio::ip::tcp::socket> *ws);
    Player(const std::string &name, websocket::stream<asio::ip::tcp::socket> *ws);

    [[nodiscard]] std::string getName() const;
    void setName(const std::string &name);

    void setState(PlayerState state);
    void setWs(websocket::stream<asio::ip::tcp::socket> *ws);
    void setRoom(Room* room);

    void joinRoom(Room* room);
    void leaveRoom();

    int ready();
    int unready();

    void joinGame();
    void overGame();

    [[nodiscard]] int getId() const;
    [[nodiscard]] PlayerState getState() const;
    [[nodiscard]] websocket::stream<asio::ip::tcp::socket> * getWS() const;

    json getPlayerJson() const;

private:
    std::string name;
    int id;
    PlayerState state;
    websocket::stream<asio::ip::tcp::socket>* ws;
    Room* room = nullptr;
};

#endif //PLAYER_H