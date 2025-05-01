//
// Created by dailin on 25-4-8.
//

#include "GameThread.h"


// core logic
void GameThread::handleEachOperation(ReceivedInfo& received_info) {
    GameOperationPayload payload = received_info.getGamePayload();
    int playerId = received_info.getPlayerId();
    auto player = room->getPlayerById(playerId);
    auto ws = received_info.getWS();

    if (payload.type == GameOperationType::born) {
        room->getGameItems().addSnake(player);
        SendInfo info{ws};
        Proxy::sendQueue.push(info);
    }else if (payload.type == GameOperationType::changeDirection) {
        auto snake = room->getGameItems().getSnakeByPlayerId(playerId);
        snake->changeDirection(payload.newDirection);
        SendInfo info{ws};
        Proxy::sendQueue.push(info);
    }
    else if (payload.type == GameOperationType::getGameInfo) {
        json gameInfo{};
        gameInfo = room->getGameInfo();
        SendInfo info{ws,Responsecode::success,"",gameInfo};
        Proxy::sendQueue.push(info);
    }
    else {
        std::cout<< "RoomKeeper error unknown type : " <<static_cast<int>(payload.type) << std::endl;
        SendInfo err_info{ws,Responsecode::failed,"Unknown type"};
        Proxy::sendQueue.push(err_info);
    }
}

void GameThread::snakeMove() {
    for (const auto& snake : room->getGameItems().getSnakes()) {
        snake->move();
    }
}

void GameThread::handleCollision() {
    for (const auto& snake : room->getGameItems().getSnakes()) {
        // TODO: not good strategy.
        snake->judge();
    }
}

void GameThread::bornAndDead() {
    // snake dead or grow
    for (const auto& snake : room->getGameItems().getSnakes()) {
        snake->react();
    }
    // per 5s generate food;
    if (room->frame % 500 == 0) {
        room->getGameItems().addFood();
    }
}


void GameThread::sendResult() {
    // make ws list
    std::vector<websocket::stream<asio::ip::tcp::socket>*> ws_list;
    for (const auto& player : room->getPlayers()) ws_list.push_back(player->getWS());

    //make payload
    auto gameJson = this->room->getGameJson();
    // send info
    SendInfo send_info{ws_list,Responsecode::update,"",gameJson};
    Proxy::sendQueue.push(send_info);
}

void GameThread::gameEnd() {
    // TODO

}

void GameThread::start() {
    thread = std::thread(&GameThread::gameLoop, this);
}

void GameThread::join() {
    thread.join();
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
    // move snake.
    snakeMove();
    // make a judgement for the collision.
    handleCollision();
    // born and dead
    bornAndDead();
    // update game
    room->getGameItems().update();
    // send result to players.
    sendResult();
    // game frame ++
    room->frame += 1;
}

void GameThread::gameLoop() {
    room->setRoomState(RoomState::InitGaming);
    initGame();
    room->setRoomState(RoomState::Playing);
    while (room->getState()==RoomState::Playing) {
        gameEachLoop();
        // game end.
        if (room->frame == 3000) {
            room->setRoomState(RoomState::Playing);
            gameEnd();
        }
        // wait for 0.1s
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}


