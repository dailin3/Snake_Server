//
// Created by dailin on 25-4-8.
//

#include "GameThread.h"


// core logic
void GameThread::handleEachOperation(ReceivedInfo& received_info) {
}

// core logic
void GameThread::handleCollision() {
}

void GameThread::sendResult() {
    // make ws list
    std::vector<websocket::stream<asio::ip::tcp::socket>*> ws_list;
    for (const auto& player : room->getPlayers()) ws_list.push_back(player->getWS());

    //make payload
    auto map = this->room->getMap().toString();  // TODO: add more details for render
    nlohmann::json payload{
        {"map",map}
    };

    // send info
    SendInfo send_info{ws_list,payload};
    Proxy::sendQueue.push(send_info);
}


GameThread::GameThread(Room *room): room(room) {
}

Room * GameThread::getRoom() const{
    return room;
}

void GameThread::initGame() {
    // init map and gameItems
    room->initMap();
    room->initWall();
    room->initSnake();
}

void GameThread::gameEachLoop() {
    // handle the info from player.
    for (auto& operation : room->getOperationsList()) handleEachOperation(operation);
    // make a judgement for the collision.
    handleCollision();
    // send result to players.
    sendResult();
}
