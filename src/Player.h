//
// Created by dailin on 25-4-1.
//

#ifndef PLAYER_H
#define PLAYER_H
#include <string>

#include "Proxy.h"

enum class PlayerState {
    Playing = 1,
    Leave = 4,
    Ready = 2,
    InRoom = 3,
    Dissociated = 4,
};

class Player {
public:
    Player(const std::string &name, int id, PlayerState state, const websocket::stream<asio::ip::tcp::socket> *ws)
        : name(name),
          id(id),
          state(state),
          ws(ws) {
    }

    [[nodiscard]] std::string getName() const {
        return name;
    }

    [[nodiscard]] int getId() const {
        return id;
    }

    [[nodiscard]] PlayerState getState() const {
        return state;
    }

    [[nodiscard]] const websocket::stream<asio::ip::tcp::socket> * getWS() const {
        return ws;
    }

private:
    std::string name;
    int id;
    PlayerState state;
    const websocket::stream<asio::ip::tcp::socket>* ws;
};



#endif //PLAYER_H
