//
// Created by dailin on 25-4-8.
//

#include "GameThread.h"


// core logic
void GameThread::handleEachOperation(ReceivedInfo& received_info) {
    GameOperationPayload payload = received_info.getGamePayload();
    int playerId = received_info.getPlayerId();
    auto player = room->getPlayerById(playerId);

    // TODO: give user feedback in ws
    if (payload.type == GameOperationType::born) {
        room->getGameItems().addSnake(player);

    }else if (payload.type == GameOperationType::changeDirection) {
        auto snake = room->getGameItems().getSnakeByPlayerId(playerId);
        snake->changeDirection(payload.newDirection);
    }
    else if (payload.type == GameOperationType::getGameInfo) {
        json gameInfo{};
        gameInfo = room->getGameInfo();
    }
    else {
        std::cout << (int)payload.type << std::endl;
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
    auto map = this->room->getMap().toString();  // TODO: add more details for render
    nlohmann::json payload{
        {"map",map}
    };

    // send info
    SendInfo send_info{ws_list,payload};
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


