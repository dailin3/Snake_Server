//
// Created by dailin on 25-4-1.
//

#include "Player.h"
#include "Room.h"

int Player::maxId = 0;

Player::Player(const std::string &name, PlayerState state, websocket::stream<asio::ip::tcp::socket> *ws)
    : name(name), state(state), ws(ws) {
    id = maxId++;
}

Player::Player(const std::string &name, websocket::stream<asio::ip::tcp::socket> *ws)
    : name(name), ws(ws), state(PlayerState::Dissociated) {
    id = maxId++;
}

std::string Player::getName() const {
    return name;
}

void Player::setName(const std::string &name) {
    this->name = name;
}

void Player::setState(PlayerState state) {
    this->state = state;
}

void Player::setWs(websocket::stream<asio::ip::tcp::socket> *ws) {
    this->ws = ws;
}

void Player::setRoom(Room* room) {
    this->room = room;
}

void Player::joinRoom(Room* room) {
    setRoom(room);
    setState(PlayerState::InRoom);
}

void Player::leaveRoom() {
    setRoom(nullptr);
    setState(PlayerState::Dissociated);
}

int Player::ready() {
    if (getState() == PlayerState::InRoom) {
        setState(PlayerState::Ready);
        return 1;
    }
    return -1;
}

int Player::unready() {
    if (getState() == PlayerState::Ready) {
        setState(PlayerState::InRoom);
        return 1;
    }
    return -1;
}

void Player::joinGame() {
    if (getState() == PlayerState::Ready) {
        setState(PlayerState::Playing);
    }
}

void Player::overGame() {
    if (getState() == PlayerState::Playing) {
        setState(PlayerState::InRoom);
    }
}

int Player::getId() const {
    return id;
}

PlayerState Player::getState() const {
    return state;
}

websocket::stream<asio::ip::tcp::socket> * Player::getWS() const {
    return ws;
}

json Player::getPlayerJson() const {
    json playerJson;
    playerJson["name"] = name;
    playerJson["id"] = id;
    playerJson["state"] = state;
    if (getState() == PlayerState::InRoom) {
        playerJson["roomId"] = room->getId();
    }
    return playerJson;
}
